/************************************************************************
* @file      : drvutil.h
* @author    : pjj <yif09@163.com>
* @date      : 2013/01/24 16:39
* @brief     : 
*
* 
/************************************************************************/
#ifndef __DRVUTIL_H__
#define __DRVUTIL_H__

//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

NTSTATUS CreateDevice(PDRIVER_OBJECT pDrvObj, LONG ulDevType, PUNICODE_STRING pDevName, PUNICODE_STRING pSymName, PDEVICE_OBJECT *pDevObj);


NTSTATUS SimpleCallLowerDriver( 
                               PDEVICE_OBJECT pDev, 
                               PIRP Irp);

//////////////////////////////////////////////////////////////////////////

NTSTATUS DenyToCallLowerDriver(
                               PIRP Irp, 
                               NTSTATUS nsStatus, LONG nRetLen = 0);




//////////////////////////////////////////////////////////////////////////
#endif