#pragma once
//回调
#include "Common/MyCom.h"
#include "Common/ComTry.h"
#include "Common/MyString.h"
#include "../../Common/FileStreams.h"
#include "../IArchive.h"
//需要压缩的文件结构体
typedef class tagPackItem
{
public:    
    tagPackItem()
    {

    }
    //是否是目录
    BOOL IsDir() const
    {
        if (ByFileInfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
        {
            return TRUE;
        }
        return FALSE;
    }
    UString m_strFilePath;
    BY_HANDLE_FILE_INFORMATION ByFileInfo;

}KSPackItem, *PKSPackItem;

//压缩回调类
class C7zCompressCallback : public IArchiveUpdateCallback2,
                      public CMyUnknownImp
{
public:
    C7zCompressCallback();
    ~C7zCompressCallback();

    MY_UNKNOWN_IMP1(
        IArchiveUpdateCallback2)

    INTERFACE_IArchiveUpdateCallback2(;)

public:
    BOOL SetCompressDir(LPCWSTR lpDir);
    int GetSize() const
    {
        return m_vecKSPackItem.Size();
    }

private:
    HRESULT PushSingleFileToVector(LPCWSTR lpFilePath);
    HRESULT PushAllFilesToVector(LPCWSTR lpFilePath);

    //需要打包的文件
    CRecordVector<KSPackItem*> m_vecKSPackItem;
    //压缩路径的偏移
    int m_nPathOff;
    UString m_ustrDir;
  
};
