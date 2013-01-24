#include "StdAfx.h"
#include "7zCompressImpl.h"
#include "../7z/7zHandler.h"

C7zCompressImpl::C7zCompressImpl()
{
}

C7zCompressImpl::~C7zCompressImpl()
{
}

void __stdcall C7zCompressImpl::SetRootPath(LPCTSTR szPath)
{
	m_strRootPath = szPath;

	if (m_strRootPath.Right(1) != _T("\\"))
		m_strRootPath += _T("\\");
}

HRESULT __stdcall C7zCompressImpl::AddFile(LPCTSTR szPath)
{
	if (_tcsnicmp(szPath, m_strRootPath, m_strRootPath.GetLength()) != 0)
		return E_FAIL;

	std::vector<CString>::iterator iter;
	for (iter = m_vecFiles.begin(); iter != m_vecFiles.end(); iter++)
	{
		CString& strPath = *iter;
		if (strPath.CompareNoCase(szPath) == 0)
			return S_OK;
	}

	m_vecFiles.push_back(szPath);

	return S_OK;
}

HRESULT __stdcall C7zCompressImpl::AddFolder(LPCTSTR szPath)
{
	HRESULT hReturn = E_FAIL;
	HRESULT hRetCode = E_FAIL;
	WIN32_FIND_DATAW fd = { 0 };
	CString strPath(szPath);
	CString strFindFile;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	if (strPath.Right(1) != _T("\\"))
		strPath += _T("\\");

	strFindFile = strPath + _T("*");
	
	hFind = FindFirstFile(strFindFile, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
		goto Exit0;

	do 
	{
		if (wcscmp(fd.cFileName, L".")==0 || wcscmp(fd.cFileName, L"..")==0)
			continue;

		if (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			continue;

		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{   
			hRetCode = AddFolder((LPCTSTR)(strPath + fd.cFileName));
			if (hRetCode != S_OK) goto Exit0;
		}
		else
		{
			hRetCode = AddFile((LPCTSTR)(strPath + fd.cFileName));
			if (hRetCode != S_OK) goto Exit0;
		}
	} while (FindNextFileW(hFind, &fd));

	FindClose(hFind);
	hReturn = S_OK;

Exit0:
	return hReturn;
}

HRESULT __stdcall C7zCompressImpl::Compress(LPCWSTR lpArchivePath)
{
	HRESULT hReturn = E_FAIL;
	HRESULT hRetCode = E_FAIL;

	CMyComPtr<IOutArchive> OutArchive = new NArchive::N7z::CHandler;
	COutFileStream* pOutFileStream = new COutFileStream;
	CMyComPtr<ISequentialOutStream> outStream = pOutFileStream;
	CMyComPtr<ISetProperties> property;

	if (m_vecFiles.empty()) goto Exit0;
	
    if (!pOutFileStream->Create(lpArchivePath, true))
       goto Exit0;

    static const WCHAR * g_szName[7] = { L"x", L"0", L"0d", L"0fb", L"he", L"s", L"mt"};
    static PROPVARIANT g_szProp[7];

    g_szProp[0].vt = VT_UI4;
    g_szProp[0].ulVal = 9;

    g_szProp[1].vt = VT_BSTR;
    g_szProp[1].bstrVal = L"LZMA";

    g_szProp[2].vt = VT_BSTR;
    g_szProp[2].bstrVal = L"67108864B";//64M

    g_szProp[3].vt = VT_UI4;
    g_szProp[3].ulVal = 256;

    g_szProp[4].vt = VT_BSTR;
    g_szProp[4].bstrVal = L"off";

    g_szProp[5].vt = VT_BSTR;
    g_szProp[5].bstrVal = L"18446744073709551615B";//นฬสต

    g_szProp[6].vt = VT_UI4;
    g_szProp[6].ulVal = 4;

    hRetCode = OutArchive->QueryInterface(IID_ISetProperties, (void**)&property);
	if (!SUCCEEDED(hRetCode)) goto Exit0;

	hRetCode = property->SetProperties(&g_szName[0], g_szProp, 7);
	if (!SUCCEEDED(hRetCode)) goto Exit0;

    hRetCode = OutArchive->UpdateItems(outStream, m_vecFiles.size(), this);
	if (!SUCCEEDED(hRetCode)) goto Exit0;

	hReturn = S_OK;

Exit0:
    return hReturn;
}

STDMETHODIMP C7zCompressImpl::SetTotal(UInt64 size)
{
	return S_OK;
}

STDMETHODIMP C7zCompressImpl::SetCompleted(const UInt64 *completeValue)
{
	return S_OK;  
}


STDMETHODIMP C7zCompressImpl::GetUpdateItemInfo(UInt32 index, Int32 *newData, Int32 *newProps, UInt32 *indexInArchive)
{

	if (newData != NULL) 
	{
		*newData = 1;
	}
	if (newProps != NULL)
	{
		*newProps = 1;
	}
	if (indexInArchive != NULL)
	{
		*indexInArchive = (UInt32)-1;
	}
	return S_OK;
}

STDMETHODIMP C7zCompressImpl::GetProperty(UInt32 index, PROPID propID, PROPVARIANT *value)
{
	COM_TRY_BEGIN
	HRESULT hRetsult = S_OK;
	
	switch(propID)
	{
	case kpidPath:  
		{
			value->vt = VT_BSTR;
			value->bstrVal = ::SysAllocString((LPCTSTR)m_vecFiles[index] + m_strRootPath.GetLength());
		}
		break;
	case kpidIsDir:
		{
			value->vt = VT_BOOL;
			value->boolVal = VARIANT_FALSE;
		}
		break;
	case kpidSize:  
		{
			value->vt = VT_UI8;
			HANDLE hFile = ::CreateFile(m_vecFiles[index], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				value->uhVal.LowPart = GetFileSize(hFile, &value->uhVal.HighPart);
				CloseHandle(hFile);
			}
			else 
				hRetsult = E_FAIL;
		}
		break;
	case kpidAttrib:  
		{
			value->vt = VT_UI4;
			value->ulVal = FILE_ATTRIBUTE_NORMAL;
		}
		break;
	case kpidCTime: 
		{
			value->vt = VT_FILETIME;
			value->filetime.dwHighDateTime = 0;
			value->filetime.dwLowDateTime = 0;
		}
		break;
	case kpidATime:  
		{
			value->vt = VT_FILETIME;
			value->filetime.dwHighDateTime = 0;
			value->filetime.dwLowDateTime = 0;
		}
		break;
	case kpidMTime:  
		{
			value->vt = VT_FILETIME;
			value->filetime.dwHighDateTime = 0;
			value->filetime.dwLowDateTime = 0;
		}
		break;
	}

	return hRetsult;
	COM_TRY_END
}

STDMETHODIMP C7zCompressImpl::GetStream(UInt32 index, ISequentialInStream **inStream)
{
	CInFileStream *inStreamSpec = new CInFileStream;
	CMyComPtr<ISequentialInStream> inStreamLoc(inStreamSpec);
	
	if (!inStreamSpec->OpenShared(m_vecFiles[index], false))
	{
		return E_FAIL;
	}

	*inStream = inStreamLoc.Detach();

	return S_OK;
}

STDMETHODIMP C7zCompressImpl::SetOperationResult(Int32 operationResult)
{
	return S_OK;
}

STDMETHODIMP C7zCompressImpl::GetVolumeSize(UInt32 index, UInt64 *size)
{
	return S_OK;
}

STDMETHODIMP C7zCompressImpl::GetVolumeStream(UInt32 index, ISequentialOutStream **volumeStream)
{
	return S_OK;
}