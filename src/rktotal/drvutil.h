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

#define WPOFF() \
{   \
    _asm CLI          \
    _asm mov eax, cr0 \
    _asm and eax, NOT 10000H \
    _asm mov cr0, eax   \
}   \

/////////////////////////////////////////////////////////////////////////////

#define WPON() \
{   \
    _asm mov eax, cr0 \
    _asm or eax, 10000H \
    _asm mov cr0, eax   \
    _asm STI            \
}   \

#define GET_INDEX_ID(x)     (((ULONG) x) & 0xFFFUL)
#define VALID_ID(x)         ((((ULONG) x) & ~0x3FFFUL) == 0UL)


extern "C" POBJECT_TYPE *CmKeyObjectType;
extern "C" POBJECT_TYPE *IoFileObjectType;
extern "C" POBJECT_TYPE *ExEventObjectType;
extern "C" POBJECT_TYPE *ExSemaphoreObjectType;
extern "C" POBJECT_TYPE *TmTransactionManagerObjectType;
extern "C" POBJECT_TYPE *TmResourceManagerObjectType;
extern "C" POBJECT_TYPE *TmEnlistmentObjectType;
extern "C" POBJECT_TYPE *TmTransactionObjectType;
extern "C" POBJECT_TYPE *PsProcessType;
extern "C" POBJECT_TYPE *PsThreadType;
extern "C" POBJECT_TYPE *SeTokenObjectType;
//////////////////////////////////////////////////////////////////////////
#endif