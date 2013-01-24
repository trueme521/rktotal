/************************************************************************
* @file      : icmdctrl.h
* @author    : pjj <yif09@163.com>
* @date      : 2013/01/24 16:00
* @brief     : 
*
* 
/************************************************************************/
#ifndef __ICMDCTRL_H__
#define __ICMDCTRL_H__

//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifndef CTL_CODE
#define CTL_CODE(DeviceType, Function, Method, Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#endif

#ifndef FILE_ANY_ACCESS
#define FILE_ANY_ACCESS                     0
#endif

#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED                     0
#endif

#ifndef METHOD_NEITHER
#define METHOD_NEITHER                      3
#endif

/////////////////////////////////////////////////////////////////////////////
#define DEVICE_NAME     L"\\Device\\rktotal"
#define SYMBOL_NAME     L"\\DosDevices\\rktotal"
#define USER_SYM_NAME   L"\\\\.\\rktotal"

#define RK_TOTAL                       0x00008306
#define IOCTL_TRANSFER_TYPE(ioctl)	((ioctl) & 3)

#define CONTRL_DEVICE_TYPE              1
#define FS_DEVICE_TYPE                  2
#define DRVMAJOR_VER    0
#define DRVMINOR_VER    1

typedef struct _DRIVER_VERSION
{
    ULONG Major;
    ULONG Minor;
}DRIVER_VERSION, *PDRIVER_VERSION;

/////////////////////////////////////////////////////////////////////////////

#define IOCTL_GET_DRIVER_VER                  (ULONG)CTL_CODE(RK_TOTAL, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)


//////////////////////////////////////////////////////////////////////////
#endif
