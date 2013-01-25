#include "stdafx.h"
#include "DrvUtilHelper.h"
#include "drvutil.h"

extern "C" __declspec(dllimport) SERVICE_TABLE KeServiceDescriptorTable[4];

ULONG DrvUtilHelper::GetSystemVersion()
{
   ULONG ulMajor = 0;
   ULONG ulMinor = 0;
   BOOL bRet = PsGetVersion(&ulMajor, &ulMinor, NULL, NULL);
   ULONG ulVersion = enumInvalidSystem;

   if (bRet)
   {
       return enumInvalidSystem;
   }

   switch(ulMajor)
   {
   case  5:
       {
           switch(ulMinor)
           {
           case 1:
               ulVersion = enumXpSystem;
               break;
           case 2:
               ulVersion = enum2003System;
               break;
           default:
               break;
           }
       }
       break;
   case 6:
       {
           switch(ulMinor)
           {
           case 1:
               ulVersion = enumVistaSystem;
               break;
           case 2:
               ulVersion = enumWin7System;
               break;
           }
       }
       break;
   case 7:
       {
           switch(ulMinor)
           {
           case 1:
               ulVersion = enumWin8System;
               break;
           default:
               break;
           }
       }
       break;
   default:
       break;
   }

   return ulVersion;
}

NTSTATUS DrvUtilHelper::HookSSDT(ULONG ulSerivceID, PVOID pNewFunc, PVOID* ppFuncOld)
{
    if (ulSerivceID == 0 || pNewFunc == NULL)
    {
        return STATUS_UNSUCCESSFUL;
    }

    WPOFF()
    *ppFuncOld = (LONG*)InterlockedExchange((LONG*)&(KeServiceDescriptorTable[0].pTable[ulSerivceID]), (LONG)pNewFunc);
    WPON();

    return STATUS_SUCCESS;
}

NTSTATUS DrvUtilHelper::UnHookSSDT(ULONG ulSerivceID, PVOID pFuncOld)
{
    if (ulSerivceID == 0 || pFuncOld == NULL)
    {
        return STATUS_UNSUCCESSFUL;
    }

    WPOFF()
    InterlockedExchange((LONG*)&KeServiceDescriptorTable[0].pTable[ulSerivceID], (LONG)pFuncOld);
    WPON();

    return STATUS_SUCCESS;
}

ULONG DrvUtilHelper::GetNtServiceID(PVOID pAddr)
{
    PUCHAR pbHandler;
    ULONG ulServiceID;

    if (pAddr == NULL)
    {
        return 0;
    }

    pbHandler = (PUCHAR)pAddr;
    if (*pbHandler != (UCHAR)0xB8)
    {
        return 0;
    }

    ulServiceID = *(PULONG)(pbHandler + 1);
    if (!VALID_ID(ulServiceID))
    {
        return 0;
    }

    return ulServiceID;
}