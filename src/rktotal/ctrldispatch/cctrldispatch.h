/************************************************************************
* @file      : cctrdlispatch.h
* @author    : pjj <yif09@16c.com>
* @date      : 2013/01/24 15:58
* @brief     : 
*
* 
/************************************************************************/
#ifndef __CCTRDLISPATCH_H__
#define __CCTRDLISPATCH_H__

#include "stdafx.h"
#include "ctrldevice.h"
#include "common/DrvUtilHelper.h"
#include "inlinehook/inlinehook.h"

//////////////////////////////////////////////////////////////////////////
#endif
class CCtrlDispatch
{
public:
    CCtrlDispatch(void);
    ~CCtrlDispatch(void);

public:
    BOOL Init(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath);
    BOOL Uninit(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath);

public: 

    static NTSTATUS DriverDispatch(PDEVICE_OBJECT pDevObj, PIRP Irp);

    ULONG GetSystemVersion();

public:
    ctrldevice *m_pCtrlDev;
    inlinehook *m_pInline;
private:
    UNICODE_STRING m_RegPath;
    WCHAR m_szReg[MAX_REG_PATH_LEN];
    PDRIVER_OBJECT m_pDrvObj;
    ULONG m_WindowVersion;
};

//////////////////////////////////////////////////////////////////////////


