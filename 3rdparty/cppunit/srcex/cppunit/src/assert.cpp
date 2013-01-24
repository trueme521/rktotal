/* -------------------------------------------------------------------------
//	文件名		：	assert.cpp
//	创建者		：	许式伟
//	创建时间	：	2003-11-8 0:07:28
//	功能描述	：	
//
//	$Id: assert.cpp,v 1.3 2005/03/25 03:31:00 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace CPPUNIT_NS;

// -------------------------------------------------------------------------

STDAPI_(void) _CppUnit_ThrowException(
							 IN LPCSTR szShortDescription,
							 IN LPCSTR szDetailMsg,
							 IN LPCSTR szFile,
							 IN int nLine) throw(Exception)
{
	throw Exception(
		Message(szShortDescription, szDetailMsg),
		SourceLine(szFile, nLine));
}

STDAPI_(void) _CppUnit_Fail(
					const char* message, 
					const char* file,
					int line
					)
{
	Asserter::fail(message, SourceLine(file, line));
}

STDAPI_(void) _CppUnit_FailNotEqual(
									const char* expected, 
									const char* actual, 
									const char* file,
									int line,
									const char* detail,
									const char* shortDescription
									)
{
	Asserter::failNotEqual(
		expected, actual, SourceLine(file, line), detail, shortDescription );
}

STDAPI_(void) _CppUnit_FailNotEqual_1(
									const char* expected, 
									const char* actual, 
									const char* file,
									int line
									)
{
	Asserter::failNotEqual(
		expected, actual, SourceLine(file, line) );
}

// -------------------------------------------------------------------------
// $Log: assert.cpp,v $
// Revision 1.3  2005/03/25 03:31:00  xushiwei
// 增加_CppUnit_ThrowException。以便让kfc的调试模块dbgcons.dll可以调用。
//
