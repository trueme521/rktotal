// -------------------------------------------------------------------------
// cppunit.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

// -------------------------------------------------------------------------
// _CppUnit_BreakAlloc

#if defined(_DEBUG)
#pragma comment(lib, "cppunitd")

#include <crtdbg.h>

EXPORTAPI_(UINT) _CppUnit_BreakAlloc(UINT nAllocBreak)
{
	return _CrtSetBreakAlloc(nAllocBreak);
}

#else
#pragma comment(lib, "cppunit")

EXPORTAPI_(UINT) _CppUnit_BreakAlloc(UINT nAllocBreak)
{
	return 0;
}

#endif

// -------------------------------------------------------------------------

#undef	DllMain

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
#if defined(_DEBUG)
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		OutputDebugStringA("----> Terminating cppunit.dll ...\n");
		break;
	}
#endif
    return TRUE;
}

// -------------------------------------------------------------------------
