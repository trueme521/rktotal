/************************************************************************
* @file      : ctrldevice.h
* @author    : pjj <yif09@163.com>
* @date      : 2013/01/24 17:04
* @brief     : 
*
* 
/************************************************************************/
#ifndef __CTRLDEVICE_H__
#define __CTRLDEVICE_H__
#include "stdafx.h"
#include "common.h"
#include "drvutil.h"

//////////////////////////////////////////////////////////////////////////
class ctrldevice
{
public:
    ctrldevice(void);
    ~ctrldevice(void);

public:
    BOOL Init(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath);
    BOOL Uninit(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath);
    NTSTATUS DeviceDispatch(PDEVICE_OBJECT pDevObj, PIRP  pIrp);
    NTSTATUS DeviceIoControl(PDEVICE_OBJECT pDevObj, PIRP pIrp);

private:
    NTSTATUS GetDrvMajor(CTL_CODE_FN_PARAM *pParameter);
private:
    PDEVICE_OBJECT m_pDevObj;
};
//////////////////////////////////////////////////////////////////////////
#endif


