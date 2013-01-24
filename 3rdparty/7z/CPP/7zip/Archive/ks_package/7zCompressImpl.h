#pragma once
#include "7zInterface.h"
#include "7zCompressCB.h"
#include "Common/MyCom.h"
#include "Common/ComTry.h"
#include "Common/MyString.h"
#include "../IArchive.h"
#include "../../Common/FileStreams.h"
#include <atlstr.h>
#include <vector>

class C7zCompressImpl:public I7zCompress,
	public IArchiveUpdateCallback2,
	public CMyUnknownImp
{
public:
    C7zCompressImpl();
    ~C7zCompressImpl();

	MY_UNKNOWN_IMP1(IArchiveUpdateCallback2);
	INTERFACE_IArchiveUpdateCallback2();

	virtual void	__stdcall SetRootPath(LPCTSTR szPath);
	virtual HRESULT __stdcall AddFile(LPCTSTR szPath);
	virtual HRESULT __stdcall AddFolder(LPCTSTR szPath);
	virtual HRESULT __stdcall Compress(LPCWSTR lpArchivePath);

private:
	CString m_strRootPath;
	std::vector<CString> m_vecFiles;
};



