#include "stdafx.h"
#include "inlinehook.h"
#include "drivinit.h"
#include "common/DrvUtilHelper.h"
#include "AutoLog.h"
#include "drvutil.h"

#define MAX_FIND_INS_LEN 100

UCHAR xp_character[]
={0x8b,0x3f,0x8b,0x1c,0x87};
UNICODE_STRING	ZwDisplayStringFunctionNameU = {
    sizeof(L"ZwDisplayString") - sizeof(WCHAR), 
    sizeof(L"ZwDisplayString"), 
    L"ZwDisplayString"
};

UNICODE_STRING DrvDisplayStrU = {
    sizeof(L"rktotal") - sizeof(WCHAR),
    sizeof(L"rktotal"),
    L"rktotal"
};

typedef NTSTATUS (NTAPI*pfnNtDisplayString)(__in PUNICODE_STRING String);
extern "C" NTSTATUS NTAPI ZwDisplayString(__in PUNICODE_STRING String);


pfnNtDisplayString oldNtDisplayString = NULL;

PVOID g_HookAddr = NULL;

inlinehook::inlinehook()
{

}

inlinehook::~inlinehook()
{

}

BOOL inlinehook::Init()
{
    CAutoPrintLog log(__FUNCTION__);
    NTSTATUS st = StartInlineHook();
    BOOL bRet = NT_SUCCESS(st);
    return bRet;
}

NTSTATUS inlinehook::StartInlineHook()
{
    CAutoPrintLog log(__FUNCTION__);
    NTSTATUS status = STATUS_SUCCESS;
    PVOID pAddr = NULL;
    pAddr = GetInlineHookAddress();

    do 
    {
        if (pAddr == NULL)
        {
            break;
        }

        switch(DrvUtilHelper::GetSystemVersion())
        {
        case enumXpSystem:
            pAddr = FindHookAddrBySign((PUCHAR)&xp_character, MAX_FIND_INS_LEN);
            break;
        default:
            break;
        }

        if (pAddr == NULL)
        {
            break;
        }
    } while (FALSE);
  
    return status;
}

PVOID inlinehook::GetInlineHookAddress()
{

    CAutoPrintLog log(__FUNCTION__);
    PVOID oldFunc = NULL;

    do 
    {
        oldFunc = MmGetSystemRoutineAddress(&ZwDisplayStringFunctionNameU);

        if (oldFunc == NULL)
        {
            return NULL;
        }

        InterlockedExchange((LONG*)&oldNtDisplayString, (LONG)oldFunc);
        DrvUtilHelper::HookSSDT(DrvUtilHelper::GetNtServiceID(oldFunc), (PVOID)inlinehook::NewZwDisplayString, &oldFunc);
        ZwDisplayString(&DrvDisplayStrU);
        DrvUtilHelper::UnHookSSDT(DrvUtilHelper::GetNtServiceID(oldNtDisplayString), (PVOID)oldNtDisplayString);

        if (g_HookAddr == NULL
            || ((ULONG_PTR)g_HookAddr&0xFF000000) != ((ULONG_PTR)PsProcessType&0xFF000000))
        {
            g_HookAddr = NULL;
            break;
        }

    } while (FALSE);

    return g_HookAddr;
}

NTSTATUS inlinehook::NewZwDisplayString(PUNICODE_STRING pDisstr)
{
    CAutoPrintLog log(__FUNCTION__);
    if (pDisstr == NULL)
    {
        return STATUS_UNSUCCESSFUL;
    }

    if (RtlCompareUnicodeString(pDisstr, &DrvDisplayStrU, TRUE) == STATUS_SUCCESS)
    {
        _asm mov g_HookAddr, ebp;
        g_HookAddr = *(PVOID*)((ULONG_PTR)g_HookAddr + sizeof(ULONG));
        return STATUS_SUCCESS;
    }

    return oldNtDisplayString(pDisstr);
}

PVOID inlinehook::FindHookAddrBySign(PUCHAR pBuffer, ULONG offset)
{
    CAutoPrintLog log(__FUNCTION__);
    if (pBuffer == NULL || offset== 0)
    {
        return NULL;
    }

    PUCHAR pStartAddr = (PUCHAR)g_HookAddr;
    PUCHAR  pTempAddr = NULL;

    for (UCHAR nindex = 0; nindex < offset; ++nindex)
    {
        pTempAddr = pStartAddr - nindex - sizeof(xp_character);
        UCHAR j = 0;

        for ( j= 0; j < sizeof(xp_character); ++ j)
        {
            if ((pTempAddr+j) == NULL || !MmIsAddressValid(pTempAddr+j))
            {
                break;
            }

            if(*(UCHAR*)(pTempAddr+j) != *(UCHAR*)(pBuffer+j))
            {
                break;
            }
        }

        if (j == sizeof(xp_character))
        {
            pTempAddr = NULL;
            break;
        }
    }

    if (*pTempAddr != (UCHAR)0x8b)
    {
        pTempAddr = NULL;
    }

    KdPrint(("hook address:%x\n", pTempAddr));
    return pTempAddr;

}