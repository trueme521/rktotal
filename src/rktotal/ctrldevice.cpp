#include "ctrldevice.h"
#include "AutoLog.h"
ctrldevice::ctrldevice(void)
{
}

ctrldevice::~ctrldevice(void)
{
}

BOOL ctrldevice::Init(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
    CAutoPrintLog log(__FUNCTION__);
    BOOL bRet = FALSE;
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    UNICODE_STRING DevName = {0};
    UNICODE_STRING SymName = {0};

    RtlInitUnicodeString(&DevName, DEVICE_NAME);
    RtlInitUnicodeString(&SymName, SYMBOL_NAME);
    BOOL bSymSucess = FALSE;

    if (NT_SUCCESS(CreateDevice(pDrvObj, CONTRL_DEVICE_TYPE, &DevName, &SymName, &m_pDevObj)))
    {
        bRet = TRUE;
    }

    return bRet;
}

BOOL ctrldevice::Uninit(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
    CAutoPrintLog log(__FUNCTION__);
    BOOL bRet = FALSE;
    return bRet;
}

NTSTATUS ctrldevice::DeviceDispatch(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    CAutoPrintLog log(__FUNCTION__);
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION pIos = IoGetCurrentIrpStackLocation(pIrp);

    if (pIrp == NULL)
    {
        return status;
    }
    
    switch(pIos->MajorFunction)
    {
    case IRP_MJ_CREATE:
        break;
    case IRP_MJ_CLOSE:
        break;
    case  IRP_MJ_DEVICE_CONTROL:
        return DeviceIoControl(pDevObj, pIrp);
        break;
    case IRP_MJ_SHUTDOWN:
        break;
    default:break;
    }

    return DenyToCallLowerDriver(pIrp, status);
}

NTSTATUS ctrldevice::DeviceIoControl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    CAutoPrintLog log(__FUNCTION__);
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION pIos = IoGetCurrentIrpStackLocation(pIrp);

    if (pIos == NULL)
    {
        return status;
    }

    PVOID pInBuffer = pIrp->AssociatedIrp.SystemBuffer;
    ULONG nInLen = pIos->Parameters.DeviceIoControl.InputBufferLength;
    LONG  nIoCode = pIos->Parameters.DeviceIoControl.IoControlCode;

    if (IOCTL_TRANSFER_TYPE(nIoCode) != METHOD_BUFFERED)
    {
        return status;
    }
    
    PVOID pOutBuffer = pIrp->AssociatedIrp.SystemBuffer;
    ULONG nOutLen = pIos->Parameters.DeviceIoControl.OutputBufferLength;
    CTL_CODE_FN_PARAM ctlparamer  ={0};
    ctlparamer.pInBuffer = pInBuffer;
    ctlparamer.ulInSize = nInLen;
    ctlparamer.pOutBuffer = pOutBuffer;
    ctlparamer.ulOutSize = nOutLen;
    
    switch(nIoCode)
    {
    case  IOCTL_GET_DRIVER_VER:
        status = GetDrvMajor(&ctlparamer);
        break;
    default:
        break;
    }

    return DenyToCallLowerDriver(pIrp, status, ctlparamer.ulRetSize);
}

NTSTATUS ctrldevice::GetDrvMajor(CTL_CODE_FN_PARAM *paramer)
{
   CAutoPrintLog log(__FUNCTION__);
    if(paramer == NULL
        || paramer->pOutBuffer== NULL
        || paramer->ulOutSize == 0)
    {
        return STATUS_UNSUCCESSFUL;
    }

    if (sizeof(DRIVER_VERSION) != paramer->ulOutSize)
    {
        return STATUS_UNSUCCESSFUL;
    }

    PDRIVER_VERSION pDrvVer = (PDRIVER_VERSION)paramer->pOutBuffer;

    pDrvVer->Major = DRVMAJOR_VER;
    pDrvVer->Minor = DRVMINOR_VER;
    paramer->ulRetSize = sizeof(DRIVER_VERSION);

    KdPrint(("Driver Verison:%d,%d\n", DRVMAJOR_VER, DRVMINOR_VER));
    return STATUS_SUCCESS;
}
