//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   MSXMLHandle.cpp
//  Version     :   1.0
//  Creator     :   YangXD(yxdh)
//  Date        :   2005-8-5 16:25:52
//  Comment     :   Implementation of the MSXMLHandle class.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include "MSXMLHandle.h"
#include "KAVPublic.h"
#include "KConversion.h"

using namespace KXML;

MSXmlHandle MSXmlHandle::FirstChild() const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;


	KAV_PROCESS_ERROR(node);

	MSXML::IXMLDOMNode* pChildNode = NULL;
		
	lRetCode = node->get_firstChild(&pChildNode);
	KAV_COM_PROCESS_ERROR(lRetCode);
	KAV_PROCESS_ERROR(pChildNode);
	
	
	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}


MSXmlHandle MSXmlHandle::FirstChild( const char* value ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	std::string strValue;

	
	KAV_PROCESS_ERROR(node);

	BSTR bstrValue = NULL;			

	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeName(&bstrValue)))
			continue;

		strValue = CW2A(bstrValue);

		if (strValue == value)
			break;
	}

		
	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::FirstChild( const wchar_t* value ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	std::wstring strValue;


	KAV_PROCESS_ERROR(node);

	BSTR bstrValue = NULL;			

	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeName(&bstrValue)))
			continue;

		strValue = bstrValue;
		if (strValue == value)
			break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::FirstChildElement() const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;


	KAV_PROCESS_ERROR(node);	

	MSXML::DOMNodeType type;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeType(&type)))
			continue;

		if (MSXML::NODE_ELEMENT == type)
			break;
	}

	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::FirstChildElement( const char* value ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	std::string strValue;


	KAV_PROCESS_ERROR(node);

	BSTR bstrValue = NULL;			
	MSXML::DOMNodeType type;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeType(&type)))
			continue;

		if (MSXML::NODE_ELEMENT != type)
			continue;

		if (FAILED(pChildNode->get_nodeName(&bstrValue)))
			continue;

		strValue = CW2A(bstrValue);

		if (strValue == value)
			break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::FirstChildElement( const wchar_t* value ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	std::wstring strValue;


	KAV_PROCESS_ERROR(node);

	BSTR bstrValue = NULL;			
	MSXML::DOMNodeType type;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeType(&type)))
			continue;

		if (MSXML::NODE_ELEMENT != type)
			continue;

		if (FAILED(pChildNode->get_nodeName(&bstrValue)))
			continue;

		strValue = bstrValue;

		if (strValue == value)
			break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::Child( int index ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	
	KAV_PROCESS_ERROR(node);

	int i = 0;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	lRetCode = node->get_firstChild(&pChildNode);
	KAV_COM_PROCESS_ERROR(lRetCode);

	for (i = 0; pChildNode && i < index; i++)
	{
		lRetCode = pChildNode->get_nextSibling(&pChildNode);
		KAV_COM_PROCESS_ERROR(lRetCode);
	}

	KAV_PROCESS_ERROR(pChildNode);


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::Child( const char* value, int index ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	std::string strValue;


	KAV_PROCESS_ERROR(node);

	BSTR bstrValue = NULL;			
	int nCount = 0;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeName(&bstrValue)))
			continue;

		strValue = CW2A(bstrValue);

		if ((strValue == value) && (nCount++ == index))
			break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::Child( const wchar_t* value, int index ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	std::wstring strValue;


	KAV_PROCESS_ERROR(node);

	BSTR bstrValue = NULL;			
	int nCount = 0;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeName(&bstrValue)))
			continue;

		strValue = bstrValue;

		if ((strValue == value) && (nCount++ == index))
			break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::ChildElement( int index ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;


	KAV_PROCESS_ERROR(node);	

	int nCount = 0;
	MSXML::DOMNodeType type;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeType(&type)))
			continue;

		if ((MSXML::NODE_ELEMENT == type) && (nCount++ == index))
			break;
	}

	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::ChildElement( const char* value, int index ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	std::string strValue;


	KAV_PROCESS_ERROR(node);

	BSTR bstrValue = NULL;			
	int nCount = 0;
	MSXML::DOMNodeType type;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeType(&type)))
			continue;

		if (MSXML::NODE_ELEMENT != type)
			continue;

		if (FAILED(pChildNode->get_nodeName(&bstrValue)))
			continue;

		strValue = CW2A(bstrValue);

		if ((strValue == value) && (nCount++ == index))
			break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::ChildElement( const wchar_t* value, int index ) const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	std::wstring strValue;


	KAV_PROCESS_ERROR(node);

	BSTR bstrValue = NULL;			
	int nCount = 0;
	MSXML::DOMNodeType type;
	MSXML::IXMLDOMNode* pChildNode = NULL;

	for (node->get_firstChild(&pChildNode); pChildNode; pChildNode->get_nextSibling(&pChildNode))
	{
		if (FAILED(pChildNode->get_nodeType(&type)))
			continue;

		if (MSXML::NODE_ELEMENT != type)
			continue;

		if (FAILED(pChildNode->get_nodeName(&bstrValue)))
			continue;

		strValue = bstrValue;

		if ((strValue == value) && (nCount++ == index))
			break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pChildNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::NextElement() const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;


	KAV_PROCESS_ERROR(node);

	MSXML::DOMNodeType type;
	MSXML::IXMLDOMNode* pNode = NULL;

	for (node->get_nextSibling(&pNode); pNode; pNode->get_nextSibling(&pNode))
	{
		if (FAILED(pNode->get_nodeType(&type)))
			continue;

		if (MSXML::NODE_ELEMENT != type)
			continue;

		break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pNode);	
	else
		return MSXmlHandle(0);
}

MSXmlHandle MSXmlHandle::ParentElement() const
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;


	KAV_PROCESS_ERROR(node);

	MSXML::DOMNodeType type;
	MSXML::IXMLDOMNode* pNode = NULL;

	for (node->get_parentNode(&pNode); pNode; pNode->get_parentNode(&pNode))
	{
		if (FAILED(pNode->get_nodeType(&type)))
			continue;

		if (MSXML::NODE_ELEMENT != type)
			continue;

		break;
	}


	nResult = true;

Exit0:

	if (nResult)
		return MSXmlHandle(pNode);	
	else
		return MSXmlHandle(0);
}

const MSXML::IXMLDOMElement* MSXmlHandle::Element() const			
{
	///< Cast to a more defined type. Will return null not of the requested type.
	MSXML::DOMNodeType type;

	if (node)
	{
		node->get_nodeType(&type);
	}

	return (node && type == MSXML::NODE_ELEMENT) ? (const MSXML::IXMLDOMElement*)node : 0; 
} 

/// Return the handle as a IXMLDOMText. This may return null.
const MSXML::IXMLDOMText* MSXmlHandle::Text() const			
{
	///< Cast to a more defined type. Will return null not of the requested type.
	MSXML::DOMNodeType type;

	if (node)
	{
		node->get_nodeType(&type);
	}

	return (node && type == MSXML::NODE_TEXT) ? (const MSXML::IXMLDOMText*)node : 0; 
}

#endif // #ifdef WIN32