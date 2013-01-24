/////////////////////////////////////////////////////////////////////////
// Kernel32 Functions
//-----------------------------------------------------------------------
// Kernel32 functions that may not exist on some of the Windows plateforms.
// We must make sure they are there before calling them, otherwise the
// application or the remote process will crash.
//
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////////////

#ifndef __KERNEL32FUNCS_H__
#define __KERNEL32FUNCS_H__

#include <windows.h>
#include <TLHELP32.H>

typedef HMODULE (WINAPI *fnLoadLibraryW)(LPCWSTR);
typedef HMODULE (WINAPI *fnGetModuleHandleW)(LPCWSTR);
typedef DWORD (WINAPI *fnGetModuleFileNameW)(HMODULE, LPWSTR, DWORD);
typedef HANDLE (WINAPI *fnCreateToolhelp32Snapshot)(DWORD, DWORD);
typedef BOOL (WINAPI *fnModule32FirstW)(HANDLE, LPMODULEENTRY32W);
typedef BOOL (WINAPI *fnModule32NextW)(HANDLE, LPMODULEENTRY32W);

typedef struct tagKernel32Funcs
{
	fnLoadLibraryW				LoadLibraryW;				// May not exists on Win9x and NT 3.x
	fnGetModuleHandleW			GetModuleHandleW;			// May not exists on Win9x and NT 3.x
	fnGetModuleFileNameW		GetModuleFileNameW;			// May not exists on Win9x and NT 3.x
	fnCreateToolhelp32Snapshot	CreateToolhelp32Snapshot;	// Not exists on NT 3.x
	fnModule32FirstW			Module32FirstW;				// Not exists on Win9x and NT 3.x
	fnModule32NextW				Module32NextW;				// Not exists on Win9x and NT 3.x
} KERNEL32FUNCS, *LPKERNEL32FUNCS;

extern KERNEL32FUNCS g_kernel32;

#endif // __KERNEL32FUNCS_H__