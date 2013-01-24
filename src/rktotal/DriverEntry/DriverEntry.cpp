#include "stdafx.h"
#include "common.h"
#include  "ctrldispatch/cctrldispatch.h"
#include  "memory/cpprt.h"
#include "AutoLog.h"

extern "C"
NTSTATUS 
DriverEntry (
	PDRIVER_OBJECT     DriverObject,
	PUNICODE_STRING    RegistryPath
	);

#pragma alloc_text ( INIT, DriverEntry )

//////////////////////////////////////////////////////////////////////////

ULONG  DefaultPoolTag = '_rkt';


CCtrlDispatch* g_pCtrl = NULL;
//////////////////////////////////////////////////////////////////////////
 
VOID
OnUnload ( PDRIVER_OBJECT DriverObject );

//////////////////////////////////////////////////////////////////////////


NTSTATUS
DriverEntry (
    PDRIVER_OBJECT     DriverObject,
    PUNICODE_STRING    RegistryPath
    )
{
	CAutoPrintLog log(__FUNCTION__);
    UNREFERENCED_PARAMETER(RegistryPath);
    NTSTATUS status = STATUS_UNSUCCESSFUL;

    do 
    {
        g_pCtrl = new CCtrlDispatch;

        if (g_pCtrl == NULL)
        {
            break;
        }

        g_pCtrl->Init(DriverObject, RegistryPath);
        status = STATUS_SUCCESS;
    } while (FALSE);
   

#ifdef _DEBUG
	DriverObject->DriverUnload = OnUnload;
#endif

    return status;
}
//////////////////////////////////////////////////////////////////////////

VOID
OnUnload (
    PDRIVER_OBJECT DriverObject
    )
{
	KdPrint(("KmBox::OnUnload()\n"));
}

//////////////////////////////////////////////////////////////////////////
