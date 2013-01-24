#pragma once
//7z的接口 xujianxin
#include "..\IArchive.h"
//7z的压缩接口
class I7zCompress
{
public:
	virtual void	__stdcall SetRootPath(LPCTSTR szPath) = 0;
	virtual HRESULT __stdcall AddFile(LPCTSTR szPath) = 0;
	virtual HRESULT __stdcall AddFolder(LPCTSTR szPath) = 0;
	virtual HRESULT __stdcall Compress(LPCWSTR lpArchivePath) = 0;
	virtual ULONG	__stdcall Release(void) = 0;
};

class I7zExtract
{
public:
	virtual HRESULT	__stdcall ExtractFiles(void* pBuffer, DWORD dwSize, LPCTSTR szRootPath) = 0;
	virtual HRESULT __stdcall UncompressInMemory(void* pBuffer, DWORD dwSize) = 0;
	virtual HRESULT __stdcall ExtractMemoryFiles(LPCTSTR szRootPath) = 0;
	virtual ULONG	__stdcall Release(void) = 0;
};

extern "C" I7zCompress* __stdcall Get7zCompressInterface();
extern "C" I7zExtract*  __stdcall Get7zExtractInterface();
