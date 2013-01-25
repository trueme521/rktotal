#include "cctrldispatch.h"
#include "drvutil.h"
#include "AutoLog.h"



extern CCtrlDispatch* g_pCtrl;

CCtrlDispatch::CCtrlDispatch():m_pDrvObj(NULL),
m_pCtrlDev(NULL),
m_WindowVersion(enumInvalidSystem)
{
    RtlZeroMemory(m_szReg, sizeof(m_szReg));
    RtlInitEmptyUnicodeString(&m_RegPath, m_szReg, sizeof(m_szReg));
    m_pCtrlDev = new ctrldevice;
    m_pInline = new inlinehook;
}

CCtrlDispatch::~CCtrlDispatch()
{

}

BOOL CCtrlDispatch::Init(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
    CAutoPrintLog log(__FUNCTION__);
    BOOL bRet = FALSE;
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    m_pDrvObj = pDrvObj;
    UNICODE_STRING DevName = {0};
    UNICODE_STRING SymName = {0};
    RtlCopyUnicodeString(&m_RegPath, pRegPath);
    RtlInitUnicodeString(&DevName, DEVICE_NAME);
    RtlInitUnicodeString(&SymName, SYMBOL_NAME);
    BOOL bSymSucess = FALSE;
#ifdef DBG
    _asm int 3
#endif

    for (int nFunc = 0; nFunc < IRP_MJ_MAXIMUM_FUNCTION; ++nFunc)
    {
        pDrvObj->MajorFunction[nFunc] = DriverDispatch;
    }

    if (m_pCtrlDev)
    {
        bRet = m_pCtrlDev->Init(pDrvObj, pRegPath);
    }

    if (m_pInline)
    {
        bRet &= m_pInline->Init();
    }

    m_WindowVersion = DrvUtilHelper::GetSystemVersion();

    KdPrint(("system  Version:%d\n", m_WindowVersion));

    return bRet;
}

BOOL CCtrlDispatch::Uninit(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
    CAutoPrintLog log(__FUNCTION__);
    BOOL bRet = FALSE;
    return bRet;
}

NTSTATUS CCtrlDispatch::DriverDispatch(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    CAutoPrintLog log(__FUNCTION__);
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    PDEVICE_EXTENSION pDevExt =(PDEVICE_EXTENSION) pDevObj->DeviceExtension;

    if (NULL == pDevExt)
    {
        return STATUS_UNSUCCESSFUL;
    }

    switch (pDevExt->ulDevType)
    {
    case CONTRL_DEVICE_TYPE:

        if (g_pCtrl->m_pCtrlDev)
        {
            status = (g_pCtrl->m_pCtrlDev)->DeviceDispatch(pDevObj, pIrp);
        }
        break;
    case  FS_DEVICE_TYPE:
        break;
    default:
        break;
    }
    return status;
}