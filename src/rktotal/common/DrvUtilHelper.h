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
// -------------------------------------------------------------------------
class DrvUtilHelper
{
public:

    static ULONG GetSystemVersion( );
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __DRVUTILHELPER_H__ */
