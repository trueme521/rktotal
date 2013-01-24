//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KXMLManager.h
//  Version     :   1.0
//  Creator     :   YangXD(yxdh)
//  Date        :   2005-8-3 12:15:06
//  Comment     :   Interface for the KXMLManager class.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef		_K_XML_MANAGER_H_
#define		_K_XML_MANAGER_H_


#include	"KType.h"

#ifdef		XML_USE_TINY
#pragma		message("XML Lib usage XML_USE_TINY")
#ifndef		TIXML_USE_STL
#define		TIXML_USE_STL
#endif
#include	"TiXMLManager.h"
#define		KXMLManager			TiXMLManager
#define		KXMLNode			TiXmlNode
#define		KXMLHANDLE			TiXmlHandleEx
#endif		// #ifdef		XML_USE_TINY

#ifdef		XML_USE_MSXML
#pragma		message("XML Lib usage XML_USE_MSXML")
#ifndef		WIN32
			#error XML Lib usage definition "XML_USE_MSXML" must define WIN32
#endif
#include	"MSXMLManager.h"
#define		KXMLManager			MSXMLManager
#define		KXMLNode			MSXML::IXMLDOMNode
#define		KXMLHANDLE			MSXmlHandle
#pragma comment(lib, "atls.lib")
#endif		// #ifdef		XML_USE_MSXML

#if !defined(XML_USE_TINY) && !defined(XML_USE_MSXML)
#error XML Lib requires usage definition. (define XML_USE_TINY or XML_USE_MSXML)
#endif

#if defined(_UNICODE) || defined(UNICODE)
	#if !defined(_WCHAR_T_DEFINED) || !defined(_NATIVE_WCHAR_T_DEFINED)
	#error /Zc:wchar_t is not specified, the XMLLib requires you to specify it.
	#endif
#endif

#ifdef WIN32

	#ifdef _DEBUG
		#ifdef _DLL
			#pragma comment(lib, "XmlLibMD.lib")
		#else		
			#pragma comment(lib, "XmlLibD.lib")
		#endif	
	#else	
		#ifdef _DLL
			#pragma comment(lib, "XmlLibM.lib")
		#else		
			#pragma comment(lib, "XmlLib.lib")
		#endif	
	#endif

#endif	// #ifdef WIN32


#endif		// #ifdef _K_XML_MANAGER_H_