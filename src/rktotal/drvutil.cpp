#include "drvutil.h"

NTSTATUS CreateDevice(PDRIVER_OBJECT pDrvObj,  LONG ulDevType, PUNICODE_STRING pDevName, PUNICODE_STRING pSymName, PDEVICE_OBJECT *pDevObj)
{
    if (pDrvObj == NULL
        || pDevObj == NULL
        )
    {
        return STATUS_UNSUCCESSFUL;
    }

    NTSTATUS status = STATUS_UNSUCCESSFUL;
    BOOL bSymuscess = FALSE;

    do 
    {
        status = IoCreateDevice(pDrvObj,
            sizeof(DEVICE_EXTENSION), 
            pDevName,
            FILE_DEVICE_UNKNOWN,
            FILE_DEVICE_SECURE_OPEN,
            FALSE,
            pDevObj);

        if (!NT_SUCCESS(status))
        {
            break;
        }

        PDEVICE_EXTENSION  pDevExt =(PDEVICE_EXTENSION) (*pDevObj)->DeviceExtension;

        if (pDevExt == NULL)
        {
            break;
        }

        RtlZeroMemory(pDevExt, sizeof(DEVICE_EXTENSION));
        pDevExt->ulDevType = ulDevType;

        if (pSymName && pDevName)
        {
            status = IoCreateSymbolicLink(pSymName, pDevName);

            if (!NT_SUCCESS(status))
            {
                break;
            }
        }
      
        ClearFlag((*pDevObj)->Flags, DO_DEVICE_INITIALIZING);
        bSymuscess = TRUE;
        status = STATUS_SUCCESS;
    } while (FALSE);

    if (!NT_SUCCESS(status))
    {
        if (*pDevObj)
        {
            IoDeleteDevice(*pDevObj);
            pDevObj = NULL;
        }

        if (bSymuscess)
        {
            IoDeleteSymbolicLink(pSymName);
        }
    }

    return status;
}



NTSTATUS SimpleCallLowerDriver( 
                               PDEVICE_OBJECT pDev, 
                               PIRP Irp )
{
    NTSTATUS nsStatus = STATUS_SUCCESS;

    IoSkipCurrentIrpStackLocation(Irp);

    nsStatus = IoCallDriver( pDev, Irp);
    return nsStatus;
}

//////////////////////////////////////////////////////////////////////////

NTSTATUS DenyToCallLowerDriver(
                               PIRP Irp, 
                               NTSTATUS nsStatus,
                               LONG nRetLen)
{
    Irp->IoStatus.Status = nsStatus;
    Irp->IoStatus.Information = nRetLen;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return nsStatus;
}