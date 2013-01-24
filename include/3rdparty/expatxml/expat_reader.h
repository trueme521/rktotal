/* -------------------------------------------------------------------------
// 文件名		:	xmlcfg/expat_reader.h
// 创建人		:	Zhuyie
// 创建时间	:	2008-8-20 16:50:30
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __XMLCFG_EXPAT_READER_H__
#define __XMLCFG_EXPAT_READER_H__

#ifndef _XMLCFG_INTERNAL_
# error Internal use only!
#endif

#ifdef _USER_MODE_
# include <stack>
# include <string>
#else
# include "kernel_stl/stack.hpp"
# include "kernel_stl/string.hpp"
#endif

#define XML_UNICODE_WCHAR_T
#include "expat/expat.h"
#pragma comment(lib, "libexpatw.lib")

#include "expansile_string.h"

// -------------------------------------------------------------------------
namespace xmlcfg {
// -------------------------------------------------------------------------

class XmlCfgReader
{
	typedef std::stack<Setting*> SettingStack;
	struct ParseContext {		
		SettingStack settingStack;
		bool error;
		ExpansileString* pEStrs;
		ULONG nEStrs;
	};

public:
	XmlCfgReader()
	{
	}
	~XmlCfgReader()
	{
	}

	static bool process( InputStream &is, Setting *rootSetting, 
				  ExpansileString* pEStrs = NULL, ULONG nEStrs = 0 )
	{

		XML_Parser parser = XML_ParserCreate( NULL );
		if ( !parser )
			return false;
		
		ParseContext aContext;		
		aContext.error = false;
		aContext.pEStrs = pEStrs;
		aContext.nEStrs = nEStrs;
		
		aContext.settingStack.push( rootSetting );

		XML_SetUserData( parser, &aContext );
		XML_SetElementHandler( parser, __startElement, __endElement );
		XML_SetCharacterDataHandler( parser, __onElementData );

		bool bRet = true;
		const unsigned int nBufLen = 4096;
		char *buf = new char[nBufLen];
		unsigned int cbReaded = 0;
		while (1)
		{
			is.read( buf, nBufLen, &cbReaded );
			if ( cbReaded == 0 )
				break;
			if ( XML_Parse(parser, buf, cbReaded, cbReaded < nBufLen) == XML_STATUS_ERROR )
			{
				bRet = false;
				break;
			}
			if ( aContext.error )
			{
				bRet = false;
				break;
			}
		}

		XML_ParserFree( parser );
		delete []buf;

		aContext.settingStack.pop();
		ASSERT( aContext.settingStack.size() == 0 );
		return bRet;
	}

private:
	static void XMLCALL __startElement(
		void *userData, const XML_Char *name, const XML_Char **atts )
	{
		ParseContext *pContext = (ParseContext*)userData;

		Setting *setting = pContext->settingStack.top();
		Setting *childSetting = setting->createChild( name );
		if ( !childSetting )
		{
			ASSERT( 0 );
			pContext->error = true;
			return;
		}
		
		for ( unsigned i = 0; atts[i]; i += 2 )
		{
			const XML_Char *pAttrName = atts[i];
			const XML_Char *pAttrValue = atts[i + 1];			
			
			std::wstring strExpanded;
			getExpandedString( pAttrValue, strExpanded, pContext->pEStrs, pContext->nEStrs );

			Attribute *attr = childSetting->createAttribute( pAttrName );
			if ( !attr || !attr->putString(strExpanded.c_str(), (int)strExpanded.size()) )
			{
				ASSERT( 0 );
				pContext->error = true;
				return;
			}
		}

		pContext->settingStack.push( childSetting );
	}

	static void XMLCALL __onElementData (
		void *userData, const XML_Char *s, int len )
	{
		ParseContext *pContext = (ParseContext*)userData;
		if ( pContext->error )
			return;

		Setting *setting = pContext->settingStack.top();
		if ( setting )
		{
			setting->setData(s, len);
		}

	}

	static void XMLCALL __endElement(
		void *userData, const XML_Char *name )
	{
		ParseContext *pContext = (ParseContext*)userData;
		if ( pContext->error )
			return;

		Setting *setting = pContext->settingStack.top();
		if ( wcscmp(setting->getName(), name) != 0 )
		{
			ASSERT( 0 );
			pContext->error = true;
			return;
		}

		pContext->settingStack.pop();
	}
};

// -------------------------------------------------------------------------
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
// $Log: $

#endif /* __XMLCFG_EXPAT_READER_H__ */
