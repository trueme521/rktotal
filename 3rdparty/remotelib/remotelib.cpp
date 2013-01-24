//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RemoteLib.h"
#include "Kernel32Funcs.h"

//////////////////////////////////////////////////////////////////////////

KERNEL32FUNCS g_kernel32 = { 0 }; // Kernel32 functions

#define KERNEL32_PROC(x)	g_kernel32.##x## = (fn##x##)::GetProcAddress(::GetModuleHandleA("Kernel32"), #x ##)


//////////////////////////////////////////////////////////////////////////

void InitRemoteLib()
{
	KERNEL32_PROC(LoadLibraryW);
	KERNEL32_PROC(GetModuleHandleW);
	KERNEL32_PROC(GetModuleFileNameW);
	KERNEL32_PROC(CreateToolhelp32Snapshot);
	KERNEL32_PROC(Module32FirstW);
	KERNEL32_PROC(Module32NextW);
}

//////////////////////////////////////////////////////////////////////////