/************************************************************************
* @file      : inlinehook.h
* @author    : pjj <yif09@163.com>
* @date      : 2013/01/25 10:34
* @brief     : 
*
* 
/************************************************************************/
#ifndef __INLINEHOOK_H__
#define __INLINEHOOK_H__

//////////////////////////////////////////////////////////////////////////

class inlinehook
{
public:
    inlinehook();
    ~inlinehook();
public:
    BOOL Init();
    BOOL Uninit();

private:
    NTSTATUS StartInlineHook();
    NTSTATUS StopInineHook();
    PVOID GetInlineHookAddress();
    PVOID FindHookAddrBySign(PUCHAR pBuffer, ULONG Len);
public:
   static NTSTATUS __stdcall NewZwDisplayString(PUNICODE_STRING pDisstr);
public:

};

//////////////////////////////////////////////////////////////////////////
#endif