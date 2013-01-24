#pragma once
#include "Common/MyCom.h"
#include "Common/ComTry.h"
#include "Common/MyString.h"
#include "../../Common/FileStreams.h"
#include "../IArchive.h"
#include "7zInterface.h"
#include "Windows/PropVariant.h"

#include <vector>
#include <list>
#include <map>

//7z解压的回调
class I7zExtractCallBack
{
public:
	virtual HRESULT __stdcall StartOneItem(LPCWSTR lpItemPath) = 0;
	virtual HRESULT __stdcall Write(const void *data, UINT size, UINT *processedSize) = 0;
};

class CKSOutStream : public ISequentialOutStream,
                     public CMyUnknownImp
{
public:
    MY_UNKNOWN_IMP
public:
    CKSOutStream()
    {
        m_pExtractCB = NULL;

    }
    virtual ~CKSOutStream()
    {

    }
    STDMETHOD(Write)(const void *data, UInt32 size, UInt32 *processedSize)
    {
        if (m_pExtractCB)
        {
            return m_pExtractCB->Write(data, size, processedSize);
        }
        return S_OK;

    }
public:
    I7zExtractCallBack* m_pExtractCB;

};

class CKSInStream : public IInStream,
                    public IStreamGetSize,
                    public CMyUnknownImp
{
public:
    CKSInStream();
    ~CKSInStream();

    MY_UNKNOWN_IMP2(IInStream, IStreamGetSize)
public:
    void SetStream(const char* pBuffer, int nSize);
    //interface
    STDMETHOD(Read)(void *data, UInt32 size, UInt32 *processedSize);
    STDMETHOD(Seek)(Int64 offset, UInt32 seekOrigin, UInt64 *newPosition);

    STDMETHOD(GetSize)(UInt64 *size);
private:
    const char* m_pBuff;
    int   m_offset;
    int   m_nTotalSize;

};

class CKSExtractCallback : public IArchiveExtractCallback,
    public CMyUnknownImp
{
public:
    CKSExtractCallback()
    {
        m_pExtractCB = NULL;
        m_pInArchive = NULL;
    }
    MY_UNKNOWN_IMP1(IArchiveExtractCallback)
        // COM_INTERFACE_ENTRY(IArchiveVolumeExtractCallback)
        INTERFACE_IArchiveExtractCallback(;)

public:
    I7zExtractCallBack* m_pExtractCB;
    IInArchive* m_pInArchive;


};

class C7zExtract
{
public:    
    C7zExtract(void);
    ~C7zExtract(void);
    
    HRESULT __stdcall ExtractStream(const char* pBuffer, int nSize);
    void    __stdcall SetCallBack(I7zExtractCallBack* pExtractCB);
    void    __stdcall Release();

private:
    I7zExtractCallBack* m_pExtractCB;
    IInArchive* m_pInArchive;
};

struct CommonFuncs
{
	static BOOL CreateDeepDirectory(LPCTSTR szPath);
	static void PathRemoveFileSpec(CString& strPath);
	static void PathAddBackslash(CString& strPath);
	static BOOL WriteFile(HANDLE hFile, DWORD dwSize, const void* pBuffer);
	static BOOL IsDriverFile(CString& strFileName);
	static CString GetDriversPath(void);
	static CString PathGetFileName(LPCTSTR szPath);
};

class CMemFile
{
public:
	CMemFile(LPCTSTR szSubPath);
	~CMemFile(void);

	BOOL AddBlock(DWORD dwSize, const void* pBuffer);
	LPCTSTR GetSubPath(void);
	BOOL WriteFile(LPCTSTR szRootPath);

protected:
	BOOL CreateFileByMem(LPCTSTR szPath);

private:
	CString m_strSubPath;
	DWORD m_dwTotalSize;
	std::list<std::vector<BYTE>*> m_lstMemBlock;
};

class C7zExtractImpl : public I7zExtract,
	public I7zExtractCallBack
{
public:
	C7zExtractImpl();
	~C7zExtractImpl();

	virtual HRESULT	__stdcall ExtractFiles(void* pBuffer, DWORD dwSize, LPCTSTR szRootPath);
	virtual HRESULT __stdcall UncompressInMemory(void* pBuffer, DWORD dwSize);
	virtual HRESULT __stdcall ExtractMemoryFiles(LPCTSTR szRootPath);
	virtual ULONG	__stdcall Release(void);

protected:
	virtual HRESULT __stdcall StartOneItem(LPCWSTR lpItemPath);
	virtual HRESULT __stdcall Write(const void *data, UINT size, UINT *processedSize);

	BOOL CreateOneItem(LPCWSTR szPath);
	BOOL CreateOneFile(LPCWSTR szPath);
	void CloseCurFile(void);
	BOOL AddMemFile(CMemFile* pMemFile);

private:
	CString m_strRootPath;
	CString m_strSubPath;
	BOOL m_bExtractInMem;
	HANDLE m_hCurFile;
	CMemFile* m_pCurMemFile;
	std::map<CString, std::list<CMemFile*>> m_mapMemFiles;
};