/************************************************************************
* @file      : common.h
* @author    : pjj <yif09@163.com>
* @date      : 2013/01/24 15:59
* @brief     : 
*
* 
/************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__

//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "icmdctrl.h"

typedef struct
{
    PVOID pInBuffer;
    ULONG ulInSize;
    PVOID pOutBuffer;
    ULONG ulOutSize;
    ULONG ulRetSize;
} CTL_CODE_FN_PARAM;

//////////////////////////////////////////////////////////////////////////

typedef NTSTATUS (*PFN_CtrlCodeFn)(CTL_CODE_FN_PARAM *pParam);

/////////////////////////////////////////////////////////////////////////////

typedef struct
{
    ULONG ulCtrlCode;
    PFN_CtrlCodeFn pCtrlCodeFn;
} CTRL_CODE_FN;

typedef struct
{
    ULONG ulDevType;
    WCHAR cDrv;
}DEVICE_EXTENSION, *PDEVICE_EXTENSION;


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef ULONG DWORD;
typedef BYTE *PBYTE;
typedef BYTE *LPBYTE;
typedef int	BOOL;
typedef unsigned int UINT;
typedef char CHAR;
typedef char *PCHAR;
typedef void *LPVOID;
typedef PVOID POBJECT;
typedef void *LPVOID;
typedef DWORD *PDWORD;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef UINT *PUINT;

#define MAX_REG_PATH_LEN  1024


//////////////////////////////////////////////////////////////////////////
#endif
