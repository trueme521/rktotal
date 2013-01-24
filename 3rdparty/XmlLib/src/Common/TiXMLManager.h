//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   TiXMLManager.h
//  Version     :   1.0
//  Creater     :   YangXD(yxdh)
//  Date        :   2004-8-7 15:40:34
//  Comment     :   interface for the TiXMLManager class.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _TIXML_MANAGER_H_
#define _TIXML_MANAGER_H_

#include "TiXMLHandle.h"


/**
 *	A TiXMLManager is a class that wraps TinyXml;
 */
class TiXMLManager : public TiXmlHandleEx
{
public:

	TiXMLManager();
	virtual ~TiXMLManager();

	int			LoadXml(const char*			pcszXML);
	int			LoadXml(const wchar_t*		pcszXML);

	int			LoadFile(const char*		pcszFileName);
	int			LoadFile(const wchar_t*		pcszFileName);

	int			SaveFile(const char*		pcszFileName);
	int			SaveFile(const wchar_t*		pcszFileName);
	
	int			ToXMLString(std::string*	pstrRet);
	int			ToXMLString(std::wstring*	pstrRet);


	int			GetRootNode(TiXmlNode**				ppRootNode = NULL);

	int			GetCurrentNode(TiXmlNode**			ppCurrentNode);

	int			GetNodeName(const TiXmlNode* pNode, std::string*	pstrRet);
	int			GetNodeName(const TiXmlNode* pNode, std::wstring*	pstrRet);

	int			GetNodeText(const TiXmlNode* pNode, std::string*	pstrRet);
	int			GetNodeText(const TiXmlNode* pNode, std::wstring*	pstrRet);

	int			GetAttributeValue(
						const TiXmlNode*			pNode, 
						const char*					pcszAttributeName,
						std::string*				pstrRet);
	int			GetAttributeValue(
						const TiXmlNode*			pNode, 
						const wchar_t*				pcszAttributeName,
						std::wstring*				pstrRet);
	int			GetAttributeValue(
						const TiXmlNode*			pNode, 
						const std::string&			cstrAttributeName,
						std::string*				pstrRet);
	int			GetAttributeValue(
						const TiXmlNode*			pNode, 
						const std::wstring&			cstrAttributeName,
						std::wstring*				pstrRet);


	int			SetAttribute(
						const char*					pcszAttributeName, 
						const char*					pcszAttributeValue,
						TiXmlNode*					pNode = NULL);
	int			SetAttribute(
						const wchar_t*				pcszAttributeName, 
						const wchar_t*				pcszAttributeValue,
						TiXmlNode*					pNode = NULL);
	int			SetAttribute(
						const std::string&			cstrAttributeName, 
						const std::string&			cstrAttributeValue,
						TiXmlNode*					pNode = NULL);
	int			SetAttribute(
						const std::wstring&			cstrAttributeName, 
						const std::wstring&			cstrAttributeValue,
						TiXmlNode*					pNode = NULL);

	int			SetText(
						const char*					pcszText, 
						TiXmlNode*					pNode = NULL);
	int			SetText(
						const wchar_t*				pcszText, 
						TiXmlNode*					pNode = NULL);
	int			SetText(
						const std::string&			cstrText, 
						TiXmlNode*					pNode = NULL);
	int			SetText(
						const std::wstring&			cstrText, 
						TiXmlNode*					pNode = NULL);


	int			CreateRootNode(const char*			pcszNodeName);
	int			CreateRootNode(const wchar_t*		pcszNodeName);
	int			CreateRootNode(const std::string&	cstrNodeName);
	int			CreateRootNode(const std::wstring&	cstrNodeName);
	/**
	* Function: CreateXMLNode
	* @brief 
	* @param pParentNode
	* @param cstrNodeName
	* @param ppRetNode
	* @return 
	* @remark 
	*/
	int			CreateXMLNode(
						const char*					pcszNodeName,
						TiXmlNode**					ppRetNode = NULL,
						TiXmlNode*					pParentNode = NULL);
	int			CreateXMLNode(
						const wchar_t*				pcszNodeName,
						TiXmlNode**					ppRetNode = NULL,
						TiXmlNode*					pParentNode = NULL);
	int			CreateXMLNode(
						const std::string&			cstrNodeName,
						TiXmlNode**					ppRetNode = NULL,
						TiXmlNode*					pParentNode = NULL);
	int			CreateXMLNode(
						const std::wstring&			cstrNodeName,
						TiXmlNode**					ppRetNode = NULL,
						TiXmlNode*					pParentNode = NULL);

	/**
	 * Function: AppendNode
	 * @brief 
	 * @param pParentNode
	 * @param cstrNodeName
	 * @param ppRetNode
	 * @return 
	 * @remark 
	 */
	int			AppendNode(
						const char*					pcszNodeName,
						TiXmlNode**					ppRetNode = NULL,
						TiXmlNode*					pParentNode = NULL);
	int			AppendNode(
						const wchar_t*				pcszNodeName,
						TiXmlNode**					ppRetNode = NULL,
						TiXmlNode*					pParentNode = NULL);
	int			AppendNode(
						const std::string&			cstrNodeName,
						TiXmlNode**					ppRetNode = NULL,
						TiXmlNode*					pParentNode = NULL);
	int			AppendNode(
						const std::wstring&			cstrNodeName,
						TiXmlNode**					ppRetNode = NULL,
						TiXmlNode*					pParentNode = NULL);


	int			RemoveNode(TiXmlNode*				pNode);
	int			RemoveChildNodes(TiXmlNode*			pParentNode);

	int			RemoveAttribute(
						TiXmlNode*					pNode, 
						const char*					pcszAttributeName);
	int			RemoveAttribute(
						TiXmlNode*					pNode, 
						const wchar_t*				pcszAttributeName);
	int			RemoveAttribute(
						TiXmlNode*					pNode, 
						const std::string&			cstrAttributeName);
	int			RemoveAttribute(
						TiXmlNode*					pNode, 
						const std::wstring&			cstrAttributeName);

private:
	
	TiXmlDocument	m_XmlDoc;
	TiXmlNode*		m_pRoot;

	int			Initialize();
	int			UnInitialize();


protected:
	
	TiXmlNode*		m_pCurNode;

};

#endif	// #ifndef _TIXML_MANAGER_H_