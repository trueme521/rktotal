/************************************************************************
* @file      : AutoLog.h
* @author    : pjj <yif09@163.com>
* @date      : 2013/01/24 18:18
* @brief     : 
*
* 
/************************************************************************/
#ifndef __AUTOLOG_H__
#define __AUTOLOG_H__

//////////////////////////////////////////////////////////////////////////

class CAutoPrintLog
{
public:
    CAutoPrintLog(char *pFunc):m_PFunc(NULL)
    {
        if(pFunc)
        {
            KdPrint(("Enter:%s\n", pFunc));
        }

        m_PFunc = pFunc;
    }

    ~CAutoPrintLog()
    {
        if(m_PFunc)
        {
            KdPrint(("Leave:%s\n", m_PFunc));
        }
    }

private:
    char* m_PFunc;
};


//////////////////////////////////////////////////////////////////////////
#endif