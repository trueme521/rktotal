//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   MSXMLManager.h
//  Version     :   1.0
//  Creater     :   YangXD(yxdh)
//  Date        :   2004-3-7 18:06:15
//  Comment     :   interface for the MSXMLManager class.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _MSXML_MANAGER_H_
#define _MSXML_MANAGER_H_


#ifdef WIN32


#include "MSXMLHandle.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class MSXMLManager : public MSXmlHandle 
{
public:

	MSXMLManager();
	virtual ~MSXMLManager();

	int		LoadXml(const char*			pcszXML);
	int		LoadXml(const wchar_t*		pcszXML);

	int		LoadFile(const char*		pcszFileName);
	int		LoadFile(const wchar_t*		pcszFileName);

	int		SaveFile(const char*		pcszFileName);
	int		SaveFile(const wchar_t*		pcszFileName);

	int		ToXMLString(std::string*	pstrRet);
	int		ToXMLString(std::wstring*	pstrRet);


	int		GetRootNode(MSXML::IXMLDOMNode**		ppRootNode = NULL);

	int		GetCurrentNode(MSXML::IXMLDOMNode**		ppCurrentNode);
	
	int		GetNodeName(MSXML::IXMLDOMNode* pNode,	std::string*	pstrRet);
	int		GetNodeName(MSXML::IXMLDOMNode* pNode,	std::wstring*	pstrRet);
	
	int		GetNodeText(MSXML::IXMLDOMNode* pNode,	std::string*	pstrRet);
	int		GetNodeText(MSXML::IXMLDOMNode* pNode,	std::wstring*	pstrRet);
	
	int		GetAttributeValue(
						MSXML::IXMLDOMNode*			pNode, 
						const char*					pcszAttributeName,
						std::string*				pstrRet);
	int		GetAttributeValue(
						MSXML::IXMLDOMNode*			pNode, 
						const wchar_t*				pcszAttributeName,
						std::wstring*				pstrRet);
	int		GetAttributeValue(
						MSXML::IXMLDOMNode*			pNode, 
						const std::string&			cstrAttributeName,
						std::string*				pstrRet);
	int		GetAttributeValue(
						MSXML::IXMLDOMNode*			pNode, 
						const std::wstring&			cstrAttributeName,
						std::wstring*				pstrRet);


	int		SetAttribute(						
						const char*					pcszAttributeName, 
						const char*					pcszAttributeValue,
						MSXML::IXMLDOMNode*			pNode = NULL);
	int		SetAttribute(						
						const wchar_t*				pcszAttributeName, 
						const wchar_t*				pcszAttributeValue,
						MSXML::IXMLDOMNode*			pNode = NULL);
	int		SetAttribute(						
						const std::string&			cstrAttributeName, 
						const std::string&			cstrAttributeValue,
						MSXML::IXMLDOMNode*			pNode = NULL);
	int		SetAttribute(						
						const std::wstring&			cstrAttributeName, 
						const std::wstring&			cstrAttributeValue,
						MSXML::IXMLDOMNode*			pNode = NULL);

	int		SetText(
						const char*					pcszText, 
						MSXML::IXMLDOMNode*			pNode = NULL);
	int		SetText(
						const wchar_t*				pcszText, 
						MSXML::IXMLDOMNode*			pNode = NULL);
	int		SetText(
						const std::string&			cstrText, 
						MSXML::IXMLDOMNode*			pNode = NULL);
	int		SetText(
						const std::wstring&			cstrText, 
						MSXML::IXMLDOMNode*			pNode = NULL);


	int		CreateRootNode(const char*				pcszNodeName);
	int		CreateRootNode(const wchar_t*			pcszNodeName);
	int		CreateRootNode(const std::string&		cstrNodeName);
	int		CreateRootNode(const std::wstring&		cstrNodeName);

	int		CreateXMLNode(
						const char*					pcszNodeName,
						MSXML::IXMLDOMNode**		ppRetNode = NULL,
						MSXML::IXMLDOMNode*			pParentNode = NULL);
	int		CreateXMLNode(
						const wchar_t*				pcszNodeName,
						MSXML::IXMLDOMNode**		ppRetNode = NULL,
						MSXML::IXMLDOMNode*			pParentNode = NULL);
	int		CreateXMLNode(
						const std::string&			cstrNodeName,
						MSXML::IXMLDOMNode**		ppRetNode = NULL,
						MSXML::IXMLDOMNode*			pParentNode = NULL);
	int		CreateXMLNode(
						const std::wstring&			cstrNodeName,
						MSXML::IXMLDOMNode**		ppRetNode = NULL,
						MSXML::IXMLDOMNode*			pParentNode = NULL);

	int		AppendNode(
						const char*					pcszNodeName,
						MSXML::IXMLDOMNode**		ppRetNode = NULL,
						MSXML::IXMLDOMNode*			pParentNode = NULL);
	int		AppendNode(
						const wchar_t*				pcszNodeName,
						MSXML::IXMLDOMNode**		ppRetNode = NULL,
						MSXML::IXMLDOMNode*			pParentNode = NULL);
	int		AppendNode(
						const std::string&			cstrNodeName,
						MSXML::IXMLDOMNode**		ppRetNode = NULL,
						MSXML::IXMLDOMNode*			pParentNode = NULL);
	int		AppendNode(
						const std::wstring&			cstrNodeName,
						MSXML::IXMLDOMNode**		ppRetNode = NULL,
						MSXML::IXMLDOMNode*			pParentNode = NULL);


	int		RemoveNode(MSXML::IXMLDOMNode*			pNode);
	int		RemoveChildNodes(MSXML::IXMLDOMNode*	pParentNode);

	int		RemoveAttribute(
						MSXML::IXMLDOMNode*			pNode, 
						const char*					pcszAttributeName);
	int		RemoveAttribute(
						MSXML::IXMLDOMNode*			pNode, 
						const wchar_t*				pcszAttributeName);
	int		RemoveAttribute(
						MSXML::IXMLDOMNode*			pNode, 
						const std::string&			cstrAttributeName);
	int		RemoveAttribute(
						MSXML::IXMLDOMNode*			pNode, 
						const std::wstring&			cstrAttributeName);

private:

	MSXML::IXMLDOMDocument*		m_pXMLDoc;

	MSXML::IXMLDOMNode*			m_pRootNode;

	int		Initialize();
	int		UnInitialize();

protected:
	MSXML::IXMLDOMNode*			m_pCurNode;


};

#endif // #ifdef WIN32


#endif // #ifndef _XML_MANAGER_H_