//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   TiXMLManager.cpp
//  Version     :   1.0
//  Creater     :   YangXD(yxdh)
//  Date        :   2004-8-7 15:43:37
//  Comment     :   implementation of the TiXMLManager class.
//
//////////////////////////////////////////////////////////////////////////////////////
#include "TiXMLManager.h"
#include "KAVPublic.h"
#include "KConversion.h"

using namespace KXML;


//////////////////////////////////////////////////////////////////////
// TiXMLManager::Construction/Destruction
//////////////////////////////////////////////////////////////////////

TiXMLManager::TiXMLManager() : TiXmlHandleEx(&m_XmlDoc)
{
	m_pRoot		= NULL;
	m_pCurNode	= NULL;
}

TiXMLManager::~TiXMLManager()
{
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::Initialize
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::Initialize()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::UnInitialize
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::UnInitialize()
{
	m_XmlDoc.Clear();

	m_pRoot		= NULL;
	m_pCurNode	= NULL;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::LoadXml
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const char* pcszXML: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::LoadXml(const char* pcszXML)
{
    int nResult		= false;
    int nRetCode	= false;


	KAV_PROCESS_ERROR(pcszXML);

	UnInitialize();

	m_XmlDoc.Parse(pcszXML);

	nRetCode = m_XmlDoc.Error();
	KAV_PROCESS_ERROR(!nRetCode);

	nRetCode = GetRootNode();
	KAV_PROCESS_ERROR(nRetCode);


    nResult = true;

Exit0:	// fail exit

    return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::LoadXml
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const wchar_t* pcszXML: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::LoadXml(const wchar_t* pcszXML)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pcszXML);

	UnInitialize();

	m_XmlDoc.Parse(CW2A(pcszXML));

	nRetCode = m_XmlDoc.Error();
	KAV_PROCESS_ERROR(!nRetCode);

	nRetCode = GetRootNode();
	KAV_PROCESS_ERROR(nRetCode);


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::LoadFile
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const char* pcszFileName: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::LoadFile(const char* pcszFileName)
{
	int nResult		= false;
	int nRetCode	= false;

	
	KAV_PROCESS_ERROR(pcszFileName);
	
	UnInitialize();

	nRetCode = m_XmlDoc.LoadFile(pcszFileName);
	KAV_PROCESS_ERROR(nRetCode);

	nRetCode = GetRootNode();
	KAV_PROCESS_ERROR(nRetCode);


	nResult = true;

Exit0:

	return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::LoadFile
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const wchar_t* pcszFileName: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::LoadFile(const wchar_t* pcszFileName)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pcszFileName);

	UnInitialize();

	nRetCode = m_XmlDoc.LoadFile(CW2A(pcszFileName));
	KAV_PROCESS_ERROR(nRetCode);

	nRetCode = GetRootNode();
	KAV_PROCESS_ERROR(nRetCode);


	nResult = true;

Exit0:

	return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SaveFile
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const char* pcszFileName: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SaveFile(const char* pcszFileName)
{
    int nResult		= false;
    int nRetCode	= false;


	KAV_PROCESS_ERROR(pcszFileName);

	nRetCode = m_XmlDoc.SaveFile(pcszFileName);
	KAV_PROCESS_ERROR(nRetCode);
	
//Exit1:	// success exit

    nResult = true;

Exit0:	// fail exit

    return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SaveFile
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const wchar_t* pcszFileName: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SaveFile(const wchar_t* pcszFileName)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pcszFileName);

	nRetCode = m_XmlDoc.SaveFile(CW2A(pcszFileName));
	KAV_PROCESS_ERROR(nRetCode);

	//Exit1:	// success exit

	nResult = true;

Exit0:	// fail exit

	return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::ToXMLString
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Retrieves a XML string representation of the doc.
// Parameters:
//		std::string* strRet: 
// Return:
//		int: error code
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::ToXMLString(std::string* pstrRet)
{
    int nResult		= false;
    int nRetCode	= false;


	KAV_PROCESS_ERROR(pstrRet);

	pstrRet->clear();
	*pstrRet << m_XmlDoc;


    nResult = true;

Exit0:
    return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::ToXMLString
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Retrieves a XML string representation of the doc.
// Parameters:
//		std::wstring* strRet: 
// Return:
//		int: error code
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::ToXMLString(std::wstring* pstrRet)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pstrRet);

	{
		std::string strTmp;
		strTmp << m_XmlDoc;

		*pstrRet = CA2W(strTmp.c_str());
	}


	nResult = true;

Exit0:
	return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::GetRootNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		TiXmlNode** ppRootNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::GetRootNode(TiXmlNode** ppRootNode /* = NULL */) 
{
    int nResult		= false;
    int nRetCode	= false;

	
	if (!m_pRoot)
	{
		m_pRoot = m_XmlDoc.RootElement();
		KAV_PROCESS_ERROR(m_pRoot);
	}

	if (ppRootNode)
	{
		*ppRootNode = m_pRoot;
	}

	m_pCurNode = m_pRoot;


// Exit1:	// success exit

    nResult = true;

Exit0:	// fail exit

    return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::GetCurrentNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		TiXmlNode** ppCurrentNode: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::GetCurrentNode(TiXmlNode** ppCurrentNode)
{
    int nResult		= false;
    int nRetCode	= false;

	
	if (m_pCurNode)
	{
		*ppCurrentNode = m_pCurNode;
	}
	else
	{
		*ppCurrentNode = NULL;
		goto Exit0;
	}


    nResult = true;

Exit0:	// fail exit

    return nResult;

}

int TiXMLManager::GetNodeName(const TiXmlNode* pNode, std::string* pstrRet)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pNode);
	KAV_PROCESS_ERROR(pstrRet);


	*pstrRet = pNode->Value();


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::GetNodeName(const TiXmlNode* pNode, std::wstring* pstrRet)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pNode);
	KAV_PROCESS_ERROR(pstrRet);


	*pstrRet = CA2W(pNode->Value());


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::GetNodeText(const TiXmlNode* pNode, std::string* pstrRet)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pNode);
	KAV_PROCESS_ERROR(pstrRet);


	for (pNode = pNode->FirstChild(); pNode; pNode = pNode->NextSibling())
	{
		if (pNode->ToText())
			break;
	}

	KAV_PROCESS_ERROR(pNode);

	*pstrRet = pNode->ToText()->Value();


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::GetNodeText(const TiXmlNode* pNode, std::wstring* pstrRet)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pNode);
	KAV_PROCESS_ERROR(pstrRet);


	for (pNode = pNode->FirstChild(); pNode; pNode = pNode->NextSibling())
	{
		if (pNode->ToText())
			break;
	}

	KAV_PROCESS_ERROR(pNode);

	*pstrRet = CA2W(pNode->ToText()->Value());


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::GetAttributeValue(
	const TiXmlNode* pNode, 
	const char* pcszAttributeName,
	std::string* pstrRet)
{
	int nResult			= false;
	int nRetCode		= false;
	
	const char*	pTmp	= NULL;

	
	KAV_PROCESS_ERROR(pNode);
	KAV_PROCESS_ERROR(pcszAttributeName);
	KAV_PROCESS_ERROR(pstrRet);


	KAV_PROCESS_ERROR(TiXmlNode::ELEMENT == pNode->Type());

	pTmp = pNode->ToElement()->Attribute(pcszAttributeName);

	KAV_PROCESS_ERROR(pTmp);

	*pstrRet = pTmp;


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::GetAttributeValue(
	const TiXmlNode* pNode, 
	const wchar_t* pcszAttributeName,
	std::wstring* pstrRet)
{
	int nResult			= false;
	int nRetCode		= false;

	const char*	pTmp	= NULL;


	KAV_PROCESS_ERROR(pNode);
	KAV_PROCESS_ERROR(pcszAttributeName);
	KAV_PROCESS_ERROR(pstrRet);


	KAV_PROCESS_ERROR(TiXmlNode::ELEMENT == pNode->Type());

	pTmp = pNode->ToElement()->Attribute(CW2A(pcszAttributeName));

	KAV_PROCESS_ERROR(pTmp);

	*pstrRet = CA2W(pTmp);


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::GetAttributeValue(
	const TiXmlNode* pNode, 
	const std::string& cstrAttributeName,
	std::string* pstrRet)
{
	return GetAttributeValue(pNode, cstrAttributeName.c_str(), pstrRet);
}

int TiXMLManager::GetAttributeValue(
	const TiXmlNode* pNode, 
	const std::wstring& cstrAttributeName,
	std::wstring* pstrRet)
{
	return GetAttributeValue(pNode, cstrAttributeName.c_str(), pstrRet);
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SetAttribute
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const char* pcszAttributeName: 
//		const char* pcszAttributeValue: 
//		const TiXmlNode* pNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SetAttribute(
	const char* pcszAttributeName, 
	const char* pcszAttributeValue, 
	TiXmlNode* pNode /* = NULL */)
{
    int nResult		= false;
    int nRetCode	= false;


	KAV_PROCESS_ERROR(pcszAttributeName);
	KAV_PROCESS_ERROR(pcszAttributeValue);
	
	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}

	KAV_PROCESS_ERROR(TiXmlNode::ELEMENT == pNode->Type());
	pNode->ToElement()->SetAttribute(pcszAttributeName, pcszAttributeValue);

    nResult = true;

Exit0:	// fail exit

    return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SetAttribute
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const wchar_t* pcszAttributeName: 
//		const wchar_t* pcszAttributeValue: 
//		const TiXmlNode* pNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SetAttribute(
		const wchar_t* pcszAttributeName, 
		const wchar_t* pcszAttributeValue, 
		TiXmlNode* pNode /* = NULL */)
{
	int nResult		= false;
	int nRetCode	= false;

	
	KAV_PROCESS_ERROR(pcszAttributeName);
	KAV_PROCESS_ERROR(pcszAttributeValue);

	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}

	KAV_PROCESS_ERROR(TiXmlNode::ELEMENT == pNode->Type());
	pNode->ToElement()->SetAttribute(CW2A(pcszAttributeName), CW2A(pcszAttributeValue));

	nResult = true;

Exit0:	// fail exit

	return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SetAttribute
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::string& cstrAttributeName: 
//		const std::string& cstrAttributeValue: 
//		const TiXmlNode* pNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SetAttribute(
		const std::string& cstrAttributeName, 
		const std::string& cstrAttributeValue, 
		TiXmlNode* pNode /* = NULL */)
{
	return SetAttribute(cstrAttributeName.c_str(), cstrAttributeValue.c_str(), pNode);
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SetAttribute
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::wstring& cstrAttributeName: 
//		const std::wstring& cstrAttributeValue: 
//		const TiXmlNode* pNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SetAttribute(
		const std::wstring& cstrAttributeName, 
		const std::wstring& cstrAttributeValue, 
		TiXmlNode* pNode /* = NULL */)
{
	return SetAttribute(cstrAttributeName.c_str(), cstrAttributeValue.c_str(), pNode);
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SetText
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const char* pcszText: 
//		const TiXmlNode* pNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SetText(
	const char* pcszText, 
	TiXmlNode* pNode /* = NULL */)
{
    int nResult		= false;
    int nRetCode	= false;

	
	KAV_PROCESS_ERROR(pcszText);

	{
		TiXmlText	text(pcszText);
		TiXmlNode*	pChildNode = NULL;

		if (!pNode)
		{
			KAV_PROCESS_ERROR(m_pCurNode);
			pNode = m_pCurNode;
		}

		for (pChildNode = pNode->FirstChild(); pChildNode; pChildNode = pChildNode->NextSibling())
		{
			if (pChildNode->ToText())
				break;
		}
		
		if (pChildNode)
		{
			pNode->ReplaceChild(pChildNode, text);
		}
		else
			pNode->InsertEndChild(text);

	}


    nResult = true;

Exit0:	// fail exit

    return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SetText
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const wchar_t* pcszText: 
//		const TiXmlNode* pNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SetText(
	const wchar_t* pcszText, 
	TiXmlNode* pNode /* = NULL */)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pcszText);

	{
		TiXmlText	text((const char*)CW2A(pcszText));
		TiXmlNode*	pChildNode = NULL;

		if (!pNode)
		{
			KAV_PROCESS_ERROR(m_pCurNode);
			pNode = m_pCurNode;
		}

		for (pChildNode = pNode->FirstChild(); pChildNode; pChildNode = pChildNode->NextSibling())
		{
			if (pChildNode->ToText())
				break;
		}

		if (pChildNode)
		{
			pNode->ReplaceChild(pChildNode, text);
		}
		else
			pNode->InsertEndChild(text);
	}


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SetText
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::string& cstrText: 
//		const TiXmlNode* pNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SetText(
	const std::string& cstrText, 
	TiXmlNode* pNode /* = NULL */)
{
	return SetText(cstrText.c_str(), pNode);
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::SetText
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::wstring& cstrText: 
//		const TiXmlNode* pNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::SetText(
	const std::wstring& cstrText, 
	TiXmlNode* pNode /* = NULL */)
{
	return SetText(cstrText.c_str(), pNode);
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::CreateRootNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const char* pcszNodeName: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::CreateRootNode(const char* pcszNodeName)
{
    int nResult				= false;
    int nRetCode			= false;

	TiXmlNode* pRootNode	= NULL;

	
	KAV_PROCESS_ERROR(pcszNodeName);

	{
		TiXmlElement root(pcszNodeName);

		KAV_PROCESS_ERROR(!m_pRoot);

		pRootNode = m_XmlDoc.InsertEndChild(root);
		KAV_PROCESS_ERROR(pRootNode);

		m_pRoot = pRootNode->ToElement();	

		m_pCurNode = m_pRoot;
	}
//Exit1:	// success exit

    nResult = true;

Exit0:	// fail exit

    return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::CreateRootNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const wchar_t* pcszNodeName: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::CreateRootNode(const wchar_t* pcszNodeName)
{
	int nResult				= false;
	int nRetCode			= false;

	TiXmlNode* pRootNode	= NULL;

	
	KAV_PROCESS_ERROR(pcszNodeName);

	{
		TiXmlElement root((const char*)CW2A(pcszNodeName));

		KAV_PROCESS_ERROR(!m_pRoot);

		pRootNode = m_XmlDoc.InsertEndChild(root);
		KAV_PROCESS_ERROR(pRootNode);

		m_pRoot = pRootNode->ToElement();	

		m_pCurNode = m_pRoot;
	}
//Exit1:	// success exit

	nResult = true;

Exit0:	// fail exit

	return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::CreateRootNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::string& cstrNodeName: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::CreateRootNode(const std::string& cstrNodeName)
{
	return CreateRootNode(cstrNodeName.c_str());
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::CreateRootNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::wstring& cstrNodeName: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::CreateRootNode(const std::wstring& cstrNodeName)
{
	return CreateRootNode(cstrNodeName.c_str());
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::CreateXMLNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const char* pcszNodeName: 
//		TiXmlNode** ppRetNode: 
//		const TiXmlNode* pParentNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::CreateXMLNode(
	const char* pcszNodeName, 
	TiXmlNode** ppRetNode /* = NULL */, 
	TiXmlNode* pParentNode /* = NULL */)
{
    int nResult			= false;
    int nRetCode		= false;

	TiXmlNode* pNode	= NULL;


	KAV_PROCESS_ERROR(pcszNodeName);

	nRetCode = AppendNode(pcszNodeName, &pNode, pParentNode);
	KAV_PROCESS_ERROR(nRetCode);

	if (ppRetNode)
		*ppRetNode = pNode;

	m_pCurNode = pNode;


    nResult = true;

Exit0:	// fail exit

    return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::CreateXMLNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const wchar_t* pcszNodeName: 
//		TiXmlNode** ppRetNode: 
//		const TiXmlNode* pParentNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::CreateXMLNode(
	const wchar_t* pcszNodeName, 
	TiXmlNode** ppRetNode /* = NULL */, 
	TiXmlNode* pParentNode /* = NULL */)
{
	int nResult			= false;
	int nRetCode		= false;

	TiXmlNode* pNode	= NULL;


	KAV_PROCESS_ERROR(pcszNodeName);

	nRetCode = AppendNode(pcszNodeName, &pNode, pParentNode);
	KAV_PROCESS_ERROR(nRetCode);

	if (ppRetNode)
		*ppRetNode = pNode;

	m_pCurNode = pNode;


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::CreateXMLNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::string& cstrNodeName: 
//		TiXmlNode** ppRetNode: 
//		const TiXmlNode* pParentNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::CreateXMLNode(
	const std::string& cstrNodeName, 
	TiXmlNode** ppRetNode /* = NULL */, 
	TiXmlNode* pParentNode /* = NULL */)
{
	return CreateXMLNode(cstrNodeName.c_str(), ppRetNode, pParentNode);
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::CreateXMLNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::wstring& cstrNodeName: 
//		TiXmlNode** ppRetNode: 
//		const TiXmlNode* pParentNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::CreateXMLNode(
	const std::wstring& cstrNodeName, 
	TiXmlNode** ppRetNode /* = NULL */, 
	TiXmlNode* pParentNode /* = NULL */)
{
	return CreateXMLNode(cstrNodeName.c_str(), ppRetNode, pParentNode);
}


///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::AppendNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const char* pcszNodeName: 
//		TiXmlNode** ppRetNode: 
//		const TiXmlNode* pParentNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::AppendNode(
	const char* pcszNodeName, 
	TiXmlNode** ppRetNode /* = NULL */, 
	TiXmlNode* pParentNode /* = NULL */)
{
    int nResult			= false;
    int nRetCode		= false;

	TiXmlNode* pNode	= NULL;


	KAV_PROCESS_ERROR(pcszNodeName);

	{
		TiXmlElement NewNode(pcszNodeName);

		if (!pParentNode)
		{
			KAV_PROCESS_ERROR(m_pCurNode);
			pParentNode = m_pCurNode;
		}


		pNode = pParentNode->InsertEndChild(NewNode);
		KAV_PROCESS_ERROR(pNode);

		if (ppRetNode)
			*ppRetNode = pNode;
	}


    nResult = true;

Exit0:	// fail exit

    return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::AppendNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const wchar_t* pcszNodeName: 
//		TiXmlNode** ppRetNode: 
//		const TiXmlNode* pParentNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::AppendNode(
	const wchar_t* pcszNodeName, 
	TiXmlNode** ppRetNode /* = NULL */, 
	TiXmlNode* pParentNode /* = NULL */)
{
	int nResult			= false;
	int nRetCode		= false;

	TiXmlNode* pNode	= NULL;


	KAV_PROCESS_ERROR(pcszNodeName);

	{
		TiXmlElement NewNode((const char*)CW2A(pcszNodeName));

		if (!pParentNode)
		{
			KAV_PROCESS_ERROR(m_pCurNode);
			pParentNode = m_pCurNode;
		}


		pNode = pParentNode->InsertEndChild(NewNode);
		KAV_PROCESS_ERROR(pNode);

		if (ppRetNode)
			*ppRetNode = pNode;
	}


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::AppendNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::string& cstrNodeName: 
//		TiXmlNode** ppRetNode: 
//		const TiXmlNode* pParentNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::AppendNode(
	const std::string& cstrNodeName, 
	TiXmlNode** ppRetNode /* = NULL */, 
	TiXmlNode* pParentNode /* = NULL */)
{
	return AppendNode(cstrNodeName.c_str(), ppRetNode, pParentNode);
}

///////////////////////////////////////////////////////////////////////////////
// TiXMLManager::AppendNode
///////////////////////////////////////////////////////////////////////////////
// Description:
//		...
// Parameters:
//		const std::wstring& cstrNodeName: 
//		TiXmlNode** ppRetNode: 
//		const TiXmlNode* pParentNode /* = NULL */: 
// Return:
//		int: 
///////////////////////////////////////////////////////////////////////////////
int TiXMLManager::AppendNode(
	const std::wstring& cstrNodeName, 
	TiXmlNode** ppRetNode /* = NULL */, 
	TiXmlNode* pParentNode /* = NULL */)
{
	return AppendNode(cstrNodeName.c_str(), ppRetNode, pParentNode);
}

int TiXMLManager::RemoveNode(TiXmlNode* pNode)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pNode);

	if (!pNode->Parent() || pNode == m_pRoot)
	{
		UnInitialize();
	}
	else
	{
		pNode->Parent()->RemoveChild(pNode);
	}


	nResult = true;

Exit0:	// fail exit

	return nResult;
	
}

int TiXMLManager::RemoveChildNodes(TiXmlNode* pParentNode)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pParentNode);

	pParentNode->Clear();


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::RemoveAttribute(TiXmlNode* pNode, const char* pcszAttributeName)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pNode);
	KAV_PROCESS_ERROR(pcszAttributeName);

	KAV_PROCESS_ERROR(TiXmlNode::ELEMENT == pNode->Type());
	pNode->ToElement()->RemoveAttribute(pcszAttributeName);


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::RemoveAttribute(TiXmlNode* pNode, const wchar_t* pcszAttributeName)
{
	int nResult		= false;
	int nRetCode	= false;


	KAV_PROCESS_ERROR(pNode);
	KAV_PROCESS_ERROR(pcszAttributeName);

	KAV_PROCESS_ERROR(TiXmlNode::ELEMENT == pNode->Type());
	pNode->ToElement()->RemoveAttribute(CW2A(pcszAttributeName));


	nResult = true;

Exit0:	// fail exit

	return nResult;

}

int TiXMLManager::RemoveAttribute(TiXmlNode* pNode, const std::string& cstrAttributeName)
{
	return RemoveAttribute(pNode, cstrAttributeName.c_str());
}

int TiXMLManager::RemoveAttribute(TiXmlNode* pNode, const std::wstring& cstrAttributeName)
{
	return RemoveAttribute(pNode, cstrAttributeName.c_str());
}
