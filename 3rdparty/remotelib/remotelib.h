/////////////////////////////////////////////////////////////////////////
// RemoteLib.h
//-----------------------------------------------------------------------
// Definition of exported functions for "RemoteLib.dll", which is a DLL
// that allows you to load/unload your own DLL into a remote running
// processes, AKA, code injection.
//
// This library provides API's for both Windows 9x plateform (including 
// 95/98/ME) and Windows NT plateform (including NT/2000/XP/2003).
//
// In case any function of this library fails, please call "GetLastError()"
// for extended error information. 
//
// This library is UNICODE compliant.
//
// This library is provided "as is" with no expressed or implied warranties.
// The author holds no responsibilities for any possible damages or loss of
// data that are caused by use of this library. The user must assume the
// entire risk of using this library.
//-----------------------------------------------------------------------
// Version History:
//
// Jan 03, 2005, 1.00 - Initial release.
// Jan 10, 2005, 1.01 - Changed RemoteGetModuleHandle so it does not expect absolute paths.
//-----------------------------------------------------------------------
// Author:
//
// Abin (abinn32@yahoo.com)
// Homepage: http://www.wxjindu.com/abin/
/////////////////////////////////////////////////////////////////////////

#ifndef __REMOTELIB_H__
#define __REMOTELIB_H__

#include <windows.h>

/////////////////////////////////////////////////////////////////////////
// UNICODE/ANSI Function Name Definition
/////////////////////////////////////////////////////////////////////////
#ifdef UNICODE // UNICODE is defined
#define RemoteLoadLibrary			RemoteLoadLibraryW
#define RemoteLoadLibraryNT			RemoteLoadLibraryNTW
#define RemoteGetModuleHandle		RemoteGetModuleHandleW
#define RemoteGetModuleHandleNT		RemoteGetModuleHandleNTW
#define RemoteGetModuleFileName		RemoteGetModuleFileNameW
#else // #ifdef UNICODE
#define RemoteLoadLibrary			RemoteLoadLibraryA
#define RemoteLoadLibraryNT			RemoteLoadLibraryNTA
#define RemoteGetModuleHandle		RemoteGetModuleHandleA
#define RemoteGetModuleHandleNT		RemoteGetModuleHandleNTA
#define RemoteGetModuleFileName		RemoteGetModuleFileNameA
#endif // #ifdef UNICODE

#define REMOTE_TIMEOUT	2000 // Remote window response timeout(ms), also used for mutex lock.

/////////////////////////////////////////////////////////////////////////
// Loading/Unloading DLLs (Windows 95/98/ME/NT/2000/XP/2003)
//-----------------------------------------------------------------------
// For All Windows Plateforms.
// Require the remote process to have at least one valid window, except for
// RemoteGetModuleFileName which does not have such limit.
/////////////////////////////////////////////////////////////////////////
HMODULE __declspec(dllexport) RemoteLoadLibraryA(HWND hTargetWnd, LPCSTR lpszDllPath, DWORD dwTimeout = REMOTE_TIMEOUT);
HMODULE __declspec(dllexport) RemoteLoadLibraryW(HWND hTargetWnd, LPCWSTR lpszDllPath, DWORD dwTimeout = REMOTE_TIMEOUT);
BOOL __declspec(dllexport) RemoteFreeLibrary(HWND hTargetWnd, HMODULE hModule, DWORD dwTimeout = REMOTE_TIMEOUT);
HMODULE __declspec(dllexport) RemoteGetModuleHandleA(HWND hTargetWnd, LPCSTR lpszDllPath, DWORD dwTimeout = REMOTE_TIMEOUT);
HMODULE __declspec(dllexport) RemoteGetModuleHandleW(HWND hTargetWnd, LPCWSTR lpszDllPath, DWORD dwTimeout = REMOTE_TIMEOUT);
DWORD __declspec(dllexport) RemoteGetModuleFileNameA(HWND hTargetWnd, HMODULE hModule, LPSTR lpszFileName, DWORD dwSize, DWORD dwTimeout = REMOTE_TIMEOUT);
DWORD __declspec(dllexport) RemoteGetModuleFileNameW(HWND hTargetWnd, HMODULE hModule, LPWSTR lpszFileName, DWORD dwSize, DWORD dwTimeout = REMOTE_TIMEOUT);
DWORD __declspec(dllexport) RemoteGetModuleFileNameA(DWORD dwTargetProcessID, HMODULE hModule, LPSTR lpszFileName, DWORD dwSize);
DWORD __declspec(dllexport) RemoteGetModuleFileNameW(DWORD dwTargetProcessID, HMODULE hModule, LPWSTR lpszFileName, DWORD dwSize);

/////////////////////////////////////////////////////////////////////////
// Loading/Unloading DLLs (Windows NT/2000/XP/2003)
//-----------------------------------------------------------------------
// For NT Plateforms only.
// The remote process can be a window-less application.
/////////////////////////////////////////////////////////////////////////
HMODULE __declspec(dllexport) RemoteLoadLibraryNTA(DWORD dwTargetProcessID, LPCSTR lpszDllPath);
HMODULE __declspec(dllexport) RemoteLoadLibraryNTW(DWORD dwTargetProcessID, LPCWSTR lpszDllPath);
BOOL __declspec(dllexport) RemoteFreeLibraryNT(DWORD dwTargetProcessID, HMODULE hModule);
HMODULE __declspec(dllexport) RemoteGetModuleHandleNTA(DWORD dwTargetProcessID, LPCSTR lpszDllPath);
HMODULE __declspec(dllexport) RemoteGetModuleHandleNTW(DWORD dwTargetProcessID, LPCWSTR lpszDllPath);

void InitRemoteLib();

#endif // #ifndef __REMOTELIB_H__