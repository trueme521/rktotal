#include "StdAfx.h"
#include "7zCompressCB.h"

C7zCompressCallback::C7zCompressCallback()
{

}
C7zCompressCallback::~C7zCompressCallback()
{
    for (int i=0; i<m_vecKSPackItem.Size(); i++)
    {
        delete m_vecKSPackItem[i];
    }
}
HRESULT C7zCompressCallback::PushSingleFileToVector(LPCWSTR lpFilePath)
{   

    if (GetFileAttributesW(lpFilePath) == -1)
    {
        return S_OK;
    }
    KSPackItem *pInfo = new KSPackItem;     
    pInfo->m_strFilePath = lpFilePath;
    if (GetFileAttributesW(lpFilePath) & FILE_ATTRIBUTE_DIRECTORY)// is folder
    {
        WIN32_FIND_DATAW fd = { 0 };
        HANDLE hFind = FindFirstFileW(lpFilePath, &fd);
        if (INVALID_HANDLE_VALUE == hFind)
        {
            return E_FAIL;
        }

        pInfo->ByFileInfo.dwFileAttributes = fd.dwFileAttributes;
        pInfo->ByFileInfo.ftLastWriteTime = fd.ftLastWriteTime;
        FindClose(hFind);
    }
    else
    {
		HANDLE hFile = ::CreateFileW(lpFilePath, 
			GENERIC_READ,
			FILE_SHARE_READ, 
			NULL, 
			OPEN_EXISTING, 
			0, 
			NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {   
            return E_FAIL;
        }
        BY_HANDLE_FILE_INFORMATION FileInformation;
        GetFileInformationByHandle(hFile, &FileInformation);
        CloseHandle(hFile);	
        pInfo->ByFileInfo = FileInformation;	
    }

    m_vecKSPackItem.Add(pInfo);

    return S_OK;

}
HRESULT C7zCompressCallback::PushAllFilesToVector(LPCWSTR lpFilePath)
{   
    if (!(GetFileAttributesW(lpFilePath) & FILE_ATTRIBUTE_DIRECTORY))//is not directory
    {   
        PushSingleFileToVector(lpFilePath);
    }
    WCHAR szRoot[MAX_PATH] = { 0 };
    WCHAR szFileName[MAX_PATH] = { 0 };
    lstrcpyW(szRoot, lpFilePath);
    int nLen = lstrlenW(szRoot);
    if (szRoot[nLen-1] != L'\\')
    {
        lstrcatW(szRoot, L"\\");
    }
    //PathAddBackslashW(szRoot);
    wsprintfW(szFileName, L"%s%s", szRoot, L"*.*");

    WIN32_FIND_DATAW fd = { 0 };
    HANDLE h = FindFirstFileW(szFileName, &fd);
    if (h == INVALID_HANDLE_VALUE)
    {
        return E_FAIL;
    }
    do 
    {
        if(wcscmp(fd.cFileName, L".")==0 || wcscmp(fd.cFileName, L"..")==0)
        {
            continue;
        }
        WCHAR pszTemp[MAX_PATH] = { 0 };
        wsprintfW(pszTemp, L"%s%s", szRoot, fd.cFileName);
        PushSingleFileToVector(pszTemp);

        if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {   
            //如果是目录则递归一下
            PushAllFilesToVector(pszTemp);
        }

    } while ( FindNextFileW(h, &fd));

    FindClose(h);
    return S_OK;
}
BOOL C7zCompressCallback::SetCompressDir(LPCWSTR lpDir)
{
    m_vecKSPackItem.Clear();
    m_vecKSPackItem.Reserve(500);

    WCHAR szDir[MAX_PATH];
    lstrcpyW(szDir, lpDir);
    int nLen = lstrlenW(szDir);
    if (szDir[nLen-1] != L'\\')
    {
        lstrcatW(szDir, L"\\");
    }
    this->PushAllFilesToVector(lpDir);
    m_ustrDir = szDir;
    m_nPathOff = m_ustrDir.Length();
    return TRUE;
}


STDMETHODIMP C7zCompressCallback::SetTotal(UInt64 size)
{
    return S_OK;
}

STDMETHODIMP C7zCompressCallback::SetCompleted(const UInt64 *completeValue)
{
    return S_OK;  
}


STDMETHODIMP C7zCompressCallback::GetUpdateItemInfo(UInt32 index, Int32 *newData, Int32 *newProps, UInt32 *indexInArchive)
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

STDMETHODIMP C7zCompressCallback::GetProperty(UInt32 index, PROPID propID, PROPVARIANT *value)
{
    COM_TRY_BEGIN
    KSPackItem* pItem = m_vecKSPackItem[index];
    switch(propID)
    {
    case kpidPath:  
        {
            value->vt = VT_BSTR;
            value->bstrVal = ::SysAllocString(pItem->m_strFilePath+m_nPathOff);
        }
        break;
    case kpidIsDir:  
        {
            value->vt = VT_BOOL;
            if (pItem->IsDir())
            {
                value->boolVal = VARIANT_TRUE;
            }
            else
            {
                value->boolVal = VARIANT_FALSE;
            }
        }
        break;
    case kpidSize:  
        {
            value->vt = VT_UI8;
            if (pItem->IsDir())
            {
                value->uhVal.QuadPart = 0;
            }
            else
            {
                value->uhVal.HighPart = pItem->ByFileInfo.nFileSizeHigh;
                value->uhVal.LowPart = pItem->ByFileInfo.nFileSizeLow;

            }                
        }
        break;
    case kpidAttrib:  
        {
            value->vt = VT_UI4;
            value->ulVal = pItem->ByFileInfo.dwFileAttributes;
        }
        break;
    case kpidCTime: 
        {
            value->vt = VT_FILETIME;
            value->filetime = pItem->ByFileInfo.ftCreationTime;
        }
        break;
    case kpidATime:  
        {
            value->vt = VT_FILETIME;
            value->filetime = pItem->ByFileInfo.ftLastAccessTime;
        }
        break;
    case kpidMTime:  
        {
            value->vt = VT_FILETIME;
            value->filetime = pItem->ByFileInfo.ftLastWriteTime;
        }
        break;
    }

    return S_OK;
    COM_TRY_END
}

STDMETHODIMP C7zCompressCallback::GetStream(UInt32 index, ISequentialInStream **inStream)
{

    KSPackItem* pItem = m_vecKSPackItem[index];
    if (pItem->IsDir())
    {
        return S_OK;
    }
    CInFileStream *inStreamSpec = new CInFileStream;
    CMyComPtr<ISequentialInStream> inStreamLoc(inStreamSpec);
    const UString path = pItem->m_strFilePath;
    if (!inStreamSpec->OpenShared(path, false))
    {
        //  return Callback->OpenFileError(path, ::GetLastError());
        return S_OK;
    }
    *inStream = inStreamLoc.Detach();

    return S_OK;
}

STDMETHODIMP C7zCompressCallback::SetOperationResult(Int32 operationResult)
{
    return S_OK;

}

STDMETHODIMP C7zCompressCallback::GetVolumeSize(UInt32 index, UInt64 *size)
{
    return S_OK;
}

STDMETHODIMP C7zCompressCallback::GetVolumeStream(UInt32 index, ISequentialOutStream **volumeStream)
{
    return S_OK;

}


