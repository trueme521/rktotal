#include "stdafx.h"

extern "C"
NTSTATUS 
DriverEntry (
	PDRIVER_OBJECT     DriverObject,
	PUNICODE_STRING    RegistryPath
	);

#pragma alloc_text ( INIT, DriverEntry )

//////////////////////////////////////////////////////////////////////////

ULONG  DefaultPoolTag = '_rkt';


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
	UNREFERENCED_PARAMETER(RegistryPath);
    NTSTATUS status = STATUS_SUCCESS;//STATUS_UNSUCCESSFUL;
	
	KdPrint(("Kdfs::DriverEntry()\n"));

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
