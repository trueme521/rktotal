/************************************************************************
* @file      : DrvUtilHelper.h
* @author    : pjj<yif09@163.com>
* @date      : 2013/1/25 2:03:37
* @brief     : 
*
* $Id: $
/************************************************************************/
#ifndef __DRVUTILHELPER_H__
#define __DRVUTILHELPER_H__

enum{
    enumXpSystem,
    enum2003System,
    enumVistaSystem,
    enumWin7System,
    enumWin8System,
    enumInvalidSystem,
};

#pragma pack(push, 1)

typedef struct
{
    PVOID *pTable;              // Service Table Pointer
    PULONG pulTableCounter;     // Table Item Counter. This table is only updated in checked builds.
    ULONG ulServiceCounter;     // Number of services contained in this table.
    PUCHAR rguchParamTable;     // Table containing the number of bytes of parameters the handler function takes.
} SERVICE_TABLE;

#pragma pack(pop)
// -------------------------------------------------------------------------
class DrvUtilHelper
{
public:

    static ULONG GetSystemVersion( );
    static NTSTATUS HookSSDTTable();
    static NTSTATUS HookSSDT(ULONG ulSerivceID, PVOID pNewFunc, PVOID* ppFuncOld);
    static NTSTATUS UnHookSSDT(ULONG ulSerivceID, PVOID pFuncOld);
    static ULONG GetNtServiceID(PVOID pAddr);
    
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __DRVUTILHELPER_H__ */
