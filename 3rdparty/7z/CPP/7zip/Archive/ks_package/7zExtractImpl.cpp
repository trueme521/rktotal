#include "StdAfx.h"
#include "7zExtractImpl.h"
#include "../7z/7zHandler.h"

#ifndef LOGA
#define LOGA
#endif

C7zExtract::C7zExtract(void)
{
    m_pExtractCB = NULL;
    m_pInArchive = new NArchive::N7z::CHandler;
}

C7zExtract::~C7zExtract(void)
{
    if (m_pInArchive)
    {
        m_pInArchive->Release();
    }
}
const UInt64 g_maxCheckStartPosition = 1<<22;
HRESULT C7zExtract::ExtractStream(const char* pBuffer, int nSize)
{
    if (m_pInArchive == NULL)
    {
        return E_FAIL;
    }
    CKSInStream* pInStream = new CKSInStream;
    pInStream->SetStream(pBuffer, nSize);
    CMyComPtr<IInStream> spInStream = pInStream;
    HRESULT hr = m_pInArchive->Open(spInStream, &g_maxCheckStartPosition, NULL);
    if (hr != S_OK)
    {
        return hr;
    }
    UInt32 numItems;
    hr = m_pInArchive->GetNumberOfItems(&numItems);
    if (hr != S_OK)
    {
        return hr;
    }
    
 
    UInt32* indices = new UInt32[numItems];
    for (int i=0; i<numItems; i++)
    {
        indices[i] = i;
    }
        
    CKSExtractCallback* pKsExactCB = new CKSExtractCallback;
    pKsExactCB->m_pExtractCB = this->m_pExtractCB;
    pKsExactCB->m_pInArchive = this->m_pInArchive;
    CMyComPtr<IArchiveExtractCallback> spExtractCB = pKsExactCB;
    
    hr = m_pInArchive->Extract(indices, numItems, 0, spExtractCB);

    delete []indices;
   
    return S_OK;
}
void  C7zExtract::SetCallBack(I7zExtractCallBack* pExtractCB)
{
    m_pExtractCB = pExtractCB;
}
void  C7zExtract::Release()
{
    delete this;
}

//
//interface
CKSInStream::CKSInStream()
{
    m_nTotalSize = NULL;
    m_pBuff = NULL;
    m_offset = 0;   

}
CKSInStream::~CKSInStream()
{
    

}
void CKSInStream::SetStream(const char* pBuffer, int nSize)
{
    m_nTotalSize = nSize;
    m_pBuff = pBuffer;
    m_offset = 0;

}

STDMETHODIMP CKSInStream::Read(void *data, UInt32 size, UInt32 *processedSize)
{
    if (m_offset >= m_nTotalSize)
    {
        return E_FAIL;
    }
    int nRemain = m_nTotalSize-m_offset;
    int nMin = min((int)size, nRemain);
    memcpy(data, m_pBuff+m_offset, nMin);
    *processedSize = nMin;
    m_offset += nMin;
   

    return S_OK;
}
STDMETHODIMP CKSInStream::Seek(Int64 offset, UInt32 seekOrigin, UInt64 *newPosition)
{
    UInt64 newVirtPos = offset;
    switch(seekOrigin)
    {
    case STREAM_SEEK_SET: break;
    case STREAM_SEEK_CUR: newVirtPos += m_offset; break;
    case STREAM_SEEK_END: newVirtPos += m_nTotalSize; break;
    default: return STG_E_INVALIDFUNCTION;
    }

    if (newPosition)
    {
        *newPosition = newVirtPos;
       
    }
     m_offset = (int)newVirtPos;
    
    return S_OK;
}

STDMETHODIMP CKSInStream::GetSize(UInt64 *size)
{
    return S_OK;
}

//callback


 STDMETHODIMP CKSExtractCallback::SetTotal(UInt64 total)
 {
     return S_OK;
 }
 STDMETHODIMP CKSExtractCallback::SetCompleted(const UInt64 *completeValue)
 {
     return S_OK;
 }

STDMETHODIMP CKSExtractCallback::GetStream(UInt32 index, ISequentialOutStream **outStream,  Int32 askExtractMode)
{
    HRESULT hr ;
    static NWindows::NCOM::CPropVariant prop;    
    if (m_pInArchive)
    {
        hr = m_pInArchive->GetProperty(index, kpidPath, &prop);           
    }
    if (m_pExtractCB)
    {
        if (prop.vt == VT_BSTR)
        {
            m_pExtractCB->StartOneItem(prop.bstrVal);
        }
        
    }
    CKSOutStream* _outFileStreamSpec = new CKSOutStream;
    _outFileStreamSpec->m_pExtractCB = this->m_pExtractCB;

    CMyComPtr<ISequentialOutStream> outStreamLoc(_outFileStreamSpec);

    *outStream = outStreamLoc.Detach();

    return S_OK;
}

STDMETHODIMP CKSExtractCallback::PrepareOperation(Int32 askExtractMode) 
{
    return S_OK;
}

STDMETHODIMP CKSExtractCallback::SetOperationResult(Int32 resultEOperationResult)
{
    return S_OK;
}

C7zExtractImpl::C7zExtractImpl()
{
	m_bExtractInMem = FALSE;
	m_hCurFile = INVALID_HANDLE_VALUE;
	m_pCurMemFile = NULL;
}

C7zExtractImpl::~C7zExtractImpl()
{

}

HRESULT	__stdcall C7zExtractImpl::ExtractFiles(void* pBuffer, DWORD dwSize, LPCTSTR szRootPath)
{
	HRESULT hReturn = E_FAIL;
	C7zExtract extract;

	m_strRootPath = szRootPath;
	CommonFuncs::PathAddBackslash(m_strRootPath);

	m_bExtractInMem = FALSE;
	extract.SetCallBack(this);
	hReturn = extract.ExtractStream((char*)pBuffer, dwSize);
	CloseCurFile();

	return hReturn;
}

HRESULT __stdcall C7zExtractImpl::UncompressInMemory(void* pBuffer, DWORD dwSize)
{
	HRESULT hReturn = E_FAIL;
	C7zExtract extract;

	m_bExtractInMem = TRUE;
	extract.SetCallBack(this);
	hReturn = extract.ExtractStream((char*)pBuffer, dwSize);
	AddMemFile(m_pCurMemFile);

	return hReturn;
}

HRESULT __stdcall C7zExtractImpl::ExtractMemoryFiles(LPCTSTR szRootPath)
{
	HRESULT hReturn = E_FAIL;
	BOOL bRetCode = FALSE;
	CString strDir;
	CMemFile* pMemFile = NULL;
	std::list<CMemFile*>::iterator iterFile;
	std::map<CString, std::list<CMemFile*>>::iterator iterMap;

	m_strRootPath = szRootPath;
	CommonFuncs::PathAddBackslash(m_strRootPath);
	CommonFuncs::CreateDeepDirectory(m_strRootPath);

	for (iterMap = m_mapMemFiles.begin(); iterMap != m_mapMemFiles.end(); iterMap++)
	{
		const CString& strSubPath = iterMap->first;
		std::list<CMemFile*>& lstMemFile = iterMap->second;
		
		if (strSubPath != _T("."))
		{
			strDir = m_strRootPath + strSubPath;
			CommonFuncs::CreateDeepDirectory(strDir);
		}

		for (iterFile = lstMemFile.begin(); iterFile != lstMemFile.end(); iterFile++)
		{
			pMemFile = *iterFile;
			bRetCode = pMemFile->WriteFile(m_strRootPath);
			if (!bRetCode)
			{
				LOGA("ExtractMemoryFiles WriteFile fail, path:%ws", pMemFile->GetSubPath());
				goto Exit0;
			}
		}
	}

	hReturn = S_OK;

Exit0:
	return hReturn;
}

ULONG __stdcall C7zExtractImpl::Release(void)
{
	delete this;
	return 0;
}

HRESULT __stdcall C7zExtractImpl::StartOneItem(LPCWSTR lpItemPath)
{
	HRESULT hReturn = E_FAIL;
	
	if (m_bExtractInMem)
	{
		if (m_pCurMemFile)
			AddMemFile(m_pCurMemFile);

		m_pCurMemFile = new CMemFile(lpItemPath);
	}
	else
	{
		CloseCurFile();

		m_strSubPath = lpItemPath;
		BOOL bRetCode = CreateOneItem(m_strRootPath + lpItemPath);
		if (!bRetCode) goto Exit0;
	}
	
	hReturn = S_OK;
Exit0:
	return hReturn;
}

HRESULT __stdcall C7zExtractImpl::Write(const void *data, UINT size, UINT *processedSize)
{
	HRESULT hReturn = E_FAIL;

	if (m_bExtractInMem)
	{
		if (m_pCurMemFile)
		{
			BOOL bRetCode = m_pCurMemFile->AddBlock(size, data);
			if (bRetCode) hReturn = S_OK;
		}
	}
	else
	{
		if (m_hCurFile != INVALID_HANDLE_VALUE)
		{
			BOOL bRetCode = CommonFuncs::WriteFile(m_hCurFile, size, data);
			if (bRetCode) hReturn = S_OK;
		}
	}

Exit0:
	return hReturn;
}

BOOL C7zExtractImpl::CreateOneItem(LPCWSTR szPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	CString strPath(szPath);

	bRetCode = CreateOneFile(strPath);
	if (!bRetCode)
	{
		if (_taccess(strPath, 0) == 0)
		{
			CString strRenameFile;

			strRenameFile.Format(_T("%s_d_%x"), strPath, ::GetTickCount());
			::DeleteFile(strRenameFile);
			if (_trename(strPath, strRenameFile) != 0)
			{
				LOGA("MoveFileEx Rename file fail, error:%d", GetLastError());
				bRetCode = CreateOneFile(strPath + _T("0"));
				if (bRetCode)
					bRetCode = ::MoveFileEx(strPath + _T("0"), strPath, MOVEFILE_REPLACE_EXISTING | MOVEFILE_DELAY_UNTIL_REBOOT);
				else
					goto Exit0;
			}
			else
			{
				::MoveFileEx(strRenameFile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

				bRetCode = CreateOneFile(strPath);
				if (!bRetCode)
				{
					bRetCode = CreateOneFile(strPath + _T("0"));
					if (bRetCode)
						bRetCode = ::MoveFileEx(strPath + _T("0"), strPath, MOVEFILE_REPLACE_EXISTING | MOVEFILE_DELAY_UNTIL_REBOOT);
					else
						goto Exit0;
				}
			}
		}
		else
			goto Exit0;
	}

	bReturn = TRUE;
Exit0:
	return bReturn;
}

BOOL C7zExtractImpl::CreateOneFile(LPCWSTR szPath)
{
	BOOL bReturn = FALSE;
	CString strDir = szPath;

	CommonFuncs::PathRemoveFileSpec(strDir);
	CommonFuncs::CreateDeepDirectory(strDir);

	m_hCurFile = CreateFile(szPath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hCurFile == INVALID_HANDLE_VALUE) goto Exit0;

	bReturn = TRUE;
Exit0:
	return bReturn;
}

void C7zExtractImpl::CloseCurFile(void)
{
	if (m_hCurFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCurFile);
		
		CString strPath = m_strRootPath + m_strSubPath;
		if (CommonFuncs::IsDriverFile(strPath))
		{
			CString strFileName;
			CString strDriverPath;

			strDriverPath = CommonFuncs::GetDriversPath();
			strFileName = CommonFuncs::PathGetFileName(strPath);

			CopyFile(strPath, strDriverPath + strFileName, FALSE);
		}
	}

	m_hCurFile = INVALID_HANDLE_VALUE;
}

BOOL C7zExtractImpl::AddMemFile(CMemFile* pMemFile)
{
	if (pMemFile)
	{
		CString strSubPath = pMemFile->GetSubPath();

		if (strSubPath.Find(_T('\\')) == -1)
		{
			strSubPath = _T(".");
		}
		else
		{
			CommonFuncs::PathRemoveFileSpec(strSubPath);
		}

		std::map<CString, std::list<CMemFile*>>::iterator iterMap;
		iterMap = m_mapMemFiles.find(strSubPath);
		if (iterMap != m_mapMemFiles.end())
		{
			std::list<CMemFile*>& lstMemFile = iterMap->second;
			lstMemFile.push_back(pMemFile);
		}
		else
		{
			std::list<CMemFile*> lstMemFile;
			lstMemFile.push_back(pMemFile);
			m_mapMemFiles.insert(std::make_pair(strSubPath, lstMemFile));
		}
	}

	return TRUE;
}

CMemFile::CMemFile(LPCTSTR szSubPath)
{
	m_dwTotalSize = 0;
	m_strSubPath = szSubPath;
}

CMemFile::~CMemFile(void)
{
	std::list<std::vector<BYTE>*>::iterator iter;

	for (iter = m_lstMemBlock.begin(); iter != m_lstMemBlock.end(); iter++)
	{
		std::vector<BYTE>* pVec = *iter;
		delete pVec;
	}

	m_lstMemBlock.clear();
}

BOOL CMemFile::AddBlock(DWORD dwSize, const void* pBuffer)
{
	if (m_dwTotalSize + dwSize > 500 * 1024 * 1024)
		return FALSE;

	std::vector<BYTE> * pVecBuffer = new std::vector<BYTE>;

	m_dwTotalSize += dwSize;
	pVecBuffer->resize(dwSize);
	memcpy(&(*pVecBuffer)[0], pBuffer, dwSize);

	m_lstMemBlock.push_back(pVecBuffer);
}

LPCTSTR CMemFile::GetSubPath(void)
{
	return (LPCTSTR)m_strSubPath;
}

BOOL CMemFile::WriteFile(LPCTSTR szRootPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	CString strPath;
	std::list<std::vector<BYTE>*>::iterator iter;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	HANDLE hFileMapping = NULL;

	strPath = szRootPath;
	strPath += m_strSubPath;

	bRetCode = CreateFileByMem(strPath);
	if (!bRetCode)
	{
		if (_taccess(strPath, 0) == 0)
		{
			CString strRenameFile;

			strRenameFile.Format(_T("%s_d_%x"), strPath, ::GetTickCount());
			::DeleteFile(strRenameFile);
			if (_trename(strPath, strRenameFile) != 0)
			{
				LOGA("MoveFileEx Rename file fail, error:%d", GetLastError());
				bRetCode = CreateFileByMem(strPath + _T("0"));
				if (bRetCode)
					bRetCode = ::MoveFileEx(strPath + _T("0"), strPath, MOVEFILE_REPLACE_EXISTING | MOVEFILE_DELAY_UNTIL_REBOOT);
				else
					goto Exit0;
			}
			else
			{
				LOGA("MoveFileEx Rename file succeed, path:%ws", strRenameFile);
				::MoveFileEx(strRenameFile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

				bRetCode = CreateFileByMem(strPath);
				if (!bRetCode)
				{
					bRetCode = CreateFileByMem(strPath + _T("0"));
					if (bRetCode)
						bRetCode = ::MoveFileEx(strPath + _T("0"), strPath, MOVEFILE_REPLACE_EXISTING | MOVEFILE_DELAY_UNTIL_REBOOT);
					else
						goto Exit0;
				}
			}
		}
		else
			goto Exit0;
	}

	if (CommonFuncs::IsDriverFile(strPath))
	{
		CString strFileName;
		CString strDriverPath;

		strDriverPath = CommonFuncs::GetDriversPath();
		strFileName = CommonFuncs::PathGetFileName(strPath);

		CreateFileByMem(strDriverPath + strFileName);
	}

	bReturn = TRUE;
Exit0:
	return bReturn;
}

BOOL CMemFile::CreateFileByMem(LPCTSTR szPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	std::list<std::vector<BYTE>*>::iterator iter;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	HANDLE hFileMapping = NULL;

	hFile = ::CreateFile(szPath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		LOGA("CreateFileByMem CreateFile error:%d, path:%ws", ::GetLastError(), szPath);
		goto Exit0;
	}

	hFileMapping = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, m_dwTotalSize, NULL);
	if (hFile == NULL) 
	{
		LOGA("CreateFileByMem CreateFileMapping error:%d, path:%ws", ::GetLastError(), szPath);
		goto Exit0;
	}

	PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, m_dwTotalSize);
	if (!pbFile) 
	{
		LOGA("CreateFileByMem MapViewOfFile error:%d, path:%ws", ::GetLastError(), szPath);
		goto Exit0;
	}

	PBYTE pOffset = pbFile;
	for (iter = m_lstMemBlock.begin(); iter != m_lstMemBlock.end(); iter++)
	{
		std::vector<BYTE>* pVec = *iter;
		memcpy(pOffset, &((*pVec)[0]), pVec->size());
		pOffset += pVec->size();
	}

	UnmapViewOfFile(pbFile);

	bReturn = TRUE;

Exit0:
	if (hFileMapping) CloseHandle(hFileMapping);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	return bReturn;
}

void CommonFuncs::PathRemoveFileSpec(CString& strPath)
{
	int nPos = strPath.ReverseFind(_T('\\'));
	if (nPos == -1)
	{
		strPath.Empty();
	}
	else
	{
		strPath = strPath.Left(nPos);
	}
}

void CommonFuncs::PathAddBackslash(CString& strPath)
{
	if (strPath.IsEmpty()) return;

	TCHAR ch = strPath.GetAt(strPath.GetLength() - 1);
	if (ch != _T('\\') && ch != _T('/'))
	{
		strPath += _T("\\");
	}
}

BOOL CommonFuncs::WriteFile(HANDLE hFile, DWORD dwSize, const void* pBuffer)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	DWORD dwWrited = 0;
	DWORD dwWriteSize = dwSize;
	PBYTE pOffset = (PBYTE)pBuffer;

	do 
	{
		bRetCode = ::WriteFile(hFile, pOffset, dwWriteSize, &dwWrited, NULL);
		if (!bRetCode) goto Exit0;

		pOffset += dwWrited;
		dwWriteSize -= dwWrited;

		if (!dwWriteSize) break;

	} while (true);

	bReturn = TRUE;
Exit0:
	return bReturn;
}

BOOL CommonFuncs::CreateDeepDirectory(LPCTSTR szPath)
{
	BOOL bRetCode = FALSE;
	CString strPath(szPath);

	if (GetFileAttributes(szPath) != INVALID_FILE_ATTRIBUTES)
		return TRUE;

	bRetCode = ::CreateDirectory(szPath, NULL);
	if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
	{
		CommonFuncs::PathRemoveFileSpec(strPath);
		if (strPath.IsEmpty()) return FALSE;

		bRetCode = CommonFuncs::CreateDeepDirectory(strPath);
		if (!bRetCode) return FALSE;

		bRetCode = ::CreateDirectory(szPath, NULL);
		if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
			return FALSE;
	}

	return TRUE;
}

BOOL CommonFuncs::IsDriverFile(CString& strFileName)
{
	CString strExt;

	int nPos = strFileName.ReverseFind(_T('.'));
	if (nPos != -1)
		strExt = strFileName.Right(strFileName.GetLength() - nPos);
	else
		strExt = strFileName;

	strExt.MakeLower();
	if (strExt == _T(".sys")) 
		return TRUE;

	return FALSE;
}

CString CommonFuncs::GetDriversPath(void)
{
	static CString strDriversPath;

	if (!strDriversPath.IsEmpty())
		return strDriversPath;

	TCHAR szSysPath[MAX_PATH];
	DWORD dwRetLen = 0;

	dwRetLen = ::GetSystemDirectory(szSysPath, MAX_PATH);
	if (!dwRetLen) goto Exit0;

	strDriversPath = szSysPath;
	CommonFuncs::PathAddBackslash(strDriversPath);
	strDriversPath += _T("drivers\\");	

Exit0:
	return strDriversPath;
}

CString CommonFuncs::PathGetFileName(LPCTSTR szPath)
{
	CString strFileName;

	LPCTSTR pszFileName = _tcsrchr(szPath, _T('\\'));
	if (pszFileName == NULL)
		strFileName = szPath;
	else
		strFileName = ++pszFileName;

	return strFileName;
}