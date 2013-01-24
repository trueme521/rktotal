//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KType.h
//  Version     :   1.0
//  Creater     :   YangXD(yxdh)
//  Date        :   2006-2-21 16:19:56
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _K_TYPE_H_
#define _K_TYPE_H_

#include <string>
#ifdef WIN32
#include <tchar.h>
#endif


namespace KXML
{
#if !defined(_UNICODE) && !defined(UNICODE)

typedef char			KTChar;
typedef std::string		KTString;

#ifndef	_INC_TCHAR
#define _T(x)			x
#endif

#else

typedef wchar_t			KTChar;
typedef std::wstring	KTString;

#ifndef	_INC_TCHAR
#define _T(x)			L ## x
#endif

#endif	// #if !defined(_UNICODE) && !defined(UNICODE)
}


#endif	// #ifndef _K_TYPE_H_

