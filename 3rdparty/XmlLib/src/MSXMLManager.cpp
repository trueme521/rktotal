//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   MSXMLManager.cpp
//  Version     :   1.0
//  Creater     :   YangXD(yxdh)
//  Date        :   2004-3-7 18:12:00
//  Comment     :   implementation of the MSXMLManager class.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32

#include "MSXMLManager.h"
#include <stdio.h>
#ifdef _DEBUG
#define DEBUG_ORIGIN
#undef _DEBUG
#endif
#include <atlcomcli.h>		// Com Variant support
#ifdef DEBUG_ORIGIN
#define _DEBUG
#endif
#include "KAVPublic.h"
#include "KConversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////////////////////////
// Helper Function: Wait for async download to complete
////////////////////////////////////////////////////////////////////////////
HRESULT WaitForCompletion(MSXML::IXMLDOMDocument *pDoc)
{
	HRESULT		lResult		= E_FAIL;
    HRESULT		lRetCode	= E_FAIL;

    long		lReady		= 0L;
    MSG			msg;


	_ASSERT(pDoc);
	KAV_PROCESS_ERROR(pDoc);

    while (READYSTATE_COMPLETE != lReady)
    { 
		if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			::GetMessage(&msg, NULL, 0, 0);
			::DispatchMessage(&msg);
		}

		
		lRetCode = pDoc->get_readyState(&lReady);
		KAV_COM_PROCESS_ERROR(lRetCode);
    }

    while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
        ::GetMessage(&msg, NULL, 0, 0);
        ::DispatchMessage(&msg);
    }


	lResult = S_OK;

Exit0:
    
    return lResult;
}

////////////////////////////////////////////////////////////////////////////
// Helper function: Report parsing error information
////////////////////////////////////////////////////////////////////////////
HRESULT ReportError(MSXML::IXMLDOMParseError *pXMLError)
{
    HRESULT			lResult		= E_FAIL;
	HRESULT			lRetCode	= E_FAIL;

	long			line		= 0;
	long			linePos		= 0;
    long			errorCode	= 0;
    BSTR			pBURL;
	BSTR			pBReason;
	KXML::KTChar	szText[MAX_PATH] = _T("");


	_ASSERT(pXMLError);
	KAV_PROCESS_ERROR(pXMLError);


    lRetCode = pXMLError->get_line(&line);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

    lRetCode = pXMLError->get_linepos(&linePos);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

	lRetCode = pXMLError->get_errorCode(&errorCode);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

    pXMLError->get_url(&pBURL);
	
    lRetCode = pXMLError->get_reason(&pBReason);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

#if !defined(_UNICODE) && !defined(UNICODE)
	OutputDebugString(CW2A(pBReason));
#else
	OutputDebugString(pBReason);
#endif


    if (line > 0)
    {
		_stprintf(
			szText, 
			_T("Error on line %d, position %d in: "), 
			line, 
			linePos);
		OutputDebugString(szText);
		OutputDebugString(pBURL);
		OutputDebugString(_T("\r\n"));
    }
	
	lResult = S_OK;

Exit0:

    return lResult;
}

////////////////////////////////////////////////////////////////////////////
// Helper function: Check load results
////////////////////////////////////////////////////////////////////////////
HRESULT CheckLoad(MSXML::IXMLDOMDocument* pDoc)
{
    // And since we don't have the VARIANT_BOOL from load we
    // need to check the parse Error errorCode property to see
    // if everything went ok.
	HRESULT lResult				= E_FAIL;
	HRESULT lRetCode			= E_FAIL;

    long	lErrorCode			= S_OK;
	MSXML::IXMLDOMParseError	*pXMLError = NULL;


	_ASSERT(pDoc);
	KAV_PROCESS_ERROR(pDoc);

    lRetCode = pDoc->get_parseError(&pXMLError);
	KAV_PROCESS_ERROR(S_OK == lRetCode);
	KAV_PROCESS_ERROR(pXMLError);

	lRetCode = pXMLError->get_errorCode(&lErrorCode);
	KAV_PROCESS_SUCCESS(S_FALSE == lRetCode)

    if (lErrorCode != S_OK)
    {
        ReportError(pXMLError);
        goto Exit0;
    }


Exit1:
	lResult = S_OK;

Exit0:

    KAV_COM_RELEASE(pXMLError);

    return lResult;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MSXMLManager::MSXMLManager() : MSXmlHandle(m_pXMLDoc)
{
	Initialize();
	SetNode(m_pXMLDoc);
}

MSXMLManager::~MSXMLManager()
{
	UnInitialize();
}

int MSXMLManager::Initialize()
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;
	

	m_pRootNode			= NULL;
	m_pCurNode			= NULL;

	::CoInitialize(NULL);
	
	lRetCode = ::CoCreateInstance(
				MSXML::CLSID_DOMDocument,
				NULL, 
				CLSCTX_INPROC_SERVER,
				MSXML::IID_IXMLDOMDocument, 
				(void**)&m_pXMLDoc);

	if (FAILED(lRetCode))
	{	
		_com_issue_error(lRetCode);
		goto Exit0;
	}


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::UnInitialize()
{
	::CoUninitialize();

	return true;
}

int MSXMLManager::LoadXml(const char* pcszXML)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	CA2W	wszXML(pcszXML);
	BSTR	bstrXML		= wszXML;

	VARIANT_BOOL		status;
	KXML::KTChar		szDebugString[MAX_PATH] = _T("");


	if (NULL == m_pXMLDoc)
	{
		OutputDebugString(_T("m_pXMLDoc == NULL"));
		goto Exit0;
	}	
	

	lRetCode = m_pXMLDoc->put_async(VARIANT_FALSE);
    KAV_COM_PROCESS_ERROR(lRetCode);

	
	m_pXMLDoc->loadXML(bstrXML, &status);


	CheckLoad(m_pXMLDoc);
	
	if (VARIANT_TRUE != status)
	{
		_stprintf(szDebugString, _T("loadXML status Return : %d"), status);
		OutputDebugString(szDebugString);

		goto Exit0;
	}
	
	GetRootNode();


	nResult = true;
	
Exit0:

    return nResult;
}

int MSXMLManager::LoadXml(const wchar_t* pcszXML)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	BSTR	bstrXML		= (LPWSTR)pcszXML;

	VARIANT_BOOL		status;
	KXML::KTChar		szDebugString[MAX_PATH] = _T("");


	if (NULL == m_pXMLDoc)
	{
		OutputDebugString(_T("m_pXMLDoc == NULL"));
		goto Exit0;
	}	


	lRetCode = m_pXMLDoc->put_async(VARIANT_FALSE);
	KAV_COM_PROCESS_ERROR(lRetCode);


	m_pXMLDoc->loadXML(bstrXML, &status);


	CheckLoad(m_pXMLDoc);

	if (VARIANT_TRUE != status)
	{
		_stprintf(szDebugString, _T("loadXML status Return : %d"), status);
		OutputDebugString(szDebugString);

		goto Exit0;
	}

	GetRootNode();


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::LoadFile(const char* pcszFileName)
{
	int				nResult		= false;
	HRESULT			lRetCode	= E_FAIL;

	CComVariant		ComVarURL;
    VARIANT_BOOL	vbResult;


	_ASSERT(pcszFileName);
	KAV_PROCESS_ERROR(pcszFileName);

	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);

	
	ComVarURL = pcszFileName;

    // set asynchronous loading flag
	lRetCode = m_pXMLDoc->put_async(VARIANT_TRUE);
    KAV_COM_PROCESS_ERROR(lRetCode);

    // Load xml document from the given URL
    m_pXMLDoc->load(ComVarURL, &vbResult);

    // Now wait for download to complete !
    WaitForCompletion(m_pXMLDoc);
	
	lRetCode = CheckLoad(m_pXMLDoc);
    KAV_COM_PROCESS_ERROR(lRetCode);
      
	GetRootNode();

	
	nResult = true;
	
Exit0:

    return nResult;
}

int MSXMLManager::LoadFile(const wchar_t* pcszFileName)
{
	int				nResult		= false;
	HRESULT			lRetCode	= E_FAIL;

	CComVariant		ComVarURL;
	VARIANT_BOOL	vbResult;


	_ASSERT(pcszFileName);
	KAV_PROCESS_ERROR(pcszFileName);

	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	ComVarURL = pcszFileName;

	// set asynchronous loading flag
	lRetCode = m_pXMLDoc->put_async(VARIANT_TRUE);
	KAV_COM_PROCESS_ERROR(lRetCode);

	// Load xml document from the given URL
	m_pXMLDoc->load(ComVarURL, &vbResult);

	// Now wait for download to complete !
	WaitForCompletion(m_pXMLDoc);

	lRetCode = CheckLoad(m_pXMLDoc);
	KAV_COM_PROCESS_ERROR(lRetCode);

	GetRootNode();


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::SaveFile(const char* pcszFileName)
{
    int			nResult		= false;
	HRESULT		lRetCode	= E_FAIL;
	
    CComVariant DestFile;

	
	_ASSERT(pcszFileName);
	KAV_PROCESS_ERROR(pcszFileName);

	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	DestFile = pcszFileName;
    
	lRetCode = m_pXMLDoc->save(DestFile);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:
	
    return nResult;
}

int MSXMLManager::SaveFile(const wchar_t* pcszFileName)
{
	int			nResult		= false;
	HRESULT		lRetCode	= E_FAIL;

	CComVariant DestFile;


	_ASSERT(pcszFileName);
	KAV_PROCESS_ERROR(pcszFileName);

	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	DestFile = pcszFileName;

	lRetCode = m_pXMLDoc->save(DestFile);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::ToXMLString(std::string* pstrRet)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;


	_ASSERT(pstrRet);
	KAV_PROCESS_ERROR(pstrRet);

	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	BSTR bstrXml = NULL;

	lRetCode = m_pXMLDoc->get_xml(&bstrXml);
	KAV_COM_PROCESS_ERROR(lRetCode);

	*pstrRet = CW2A(bstrXml);


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::ToXMLString(std::wstring* pstrRet)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;


	_ASSERT(pstrRet);
	KAV_PROCESS_ERROR(pstrRet);

	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	BSTR bstrXml = NULL;

	lRetCode = m_pXMLDoc->get_xml(&bstrXml);
	KAV_COM_PROCESS_ERROR(lRetCode);

	*pstrRet = bstrXml;


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::GetRootNode(MSXML::IXMLDOMNode** ppRootNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;


	if (!m_pRootNode)
	{
		_ASSERT(m_pXMLDoc);
		KAV_PROCESS_ERROR(m_pXMLDoc);

		lRetCode = m_pXMLDoc->get_documentElement((MSXML::IXMLDOMElement**)&m_pRootNode);
		KAV_PROCESS_ERROR(S_OK == lRetCode);
	}

	if (ppRootNode)
	{
		*ppRootNode = m_pRootNode;
	}

	m_pCurNode = m_pRootNode;


	nResult = true;

Exit0:

	if (!nResult)
	{
		KAV_COM_RELEASE(m_pRootNode);
	}

	return nResult;
}

int MSXMLManager::GetCurrentNode(MSXML::IXMLDOMNode** ppCurrentNode)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;


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

Exit0:

	return nResult;
}

int MSXMLManager::GetNodeName(MSXML::IXMLDOMNode* pNode, std::string* pstrRet)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	BSTR	strName;


	_ASSERT(pNode);
	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->get_nodeName(&strName);
	KAV_COM_PROCESS_ERROR(lRetCode);

	*pstrRet = CW2A(strName);


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::GetNodeName(MSXML::IXMLDOMNode* pNode, std::wstring* pstrRet)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	BSTR	strName;


	_ASSERT(pNode);
	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->get_nodeName(&strName);
	KAV_COM_PROCESS_ERROR(lRetCode);

	*pstrRet = strName;


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::GetNodeText(MSXML::IXMLDOMNode* pNode, std::string* pstrRet)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	BSTR	bstrNodeText;


	_ASSERT(pNode);
	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->get_text(&bstrNodeText);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

	*pstrRet = CW2A(bstrNodeText);


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::GetNodeText(MSXML::IXMLDOMNode* pNode, std::wstring* pstrRet)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	BSTR	bstrNodeText;


	_ASSERT(pNode);
	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->get_text(&bstrNodeText);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

	*pstrRet = bstrNodeText;


	nResult = true;

Exit0:

	return nResult;
}

int MSXMLManager::GetAttributeValue(
	MSXML::IXMLDOMNode* pNode, 
	const char* pcszAttributeName,
	std::string* pstrRet)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;
	
	MSXML::IXMLDOMElement*	pEle		= NULL;
	CComVariant				ComVarValue = NULL; 

	CA2W wszAttributeName(pcszAttributeName);
	BSTR bstrAttributeName = wszAttributeName;

    
	_ASSERT(pNode);
	KAV_PROCESS_ERROR(pNode);
	
	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->getAttribute(bstrAttributeName, &ComVarValue);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

	*pstrRet = CW2A(ComVarValue.bstrVal);


	nResult = true;

Exit0:

    KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::GetAttributeValue(
	MSXML::IXMLDOMNode* pNode, 
	const wchar_t* pcszAttributeName,
	std::wstring* pstrRet)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement*	pEle		= NULL;
	CComVariant				ComVarValue = NULL; 

	BSTR bstrAttributeName = (LPWSTR)pcszAttributeName;


	_ASSERT(pNode);
	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->getAttribute(bstrAttributeName, &ComVarValue);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

	*pstrRet = ComVarValue.bstrVal;


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::GetAttributeValue(
	MSXML::IXMLDOMNode* pNode, 
	const std::string& cstrAttributeName,
	std::string* pstrRet)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement*	pEle		= NULL;
	CComVariant				ComVarValue = NULL; 

	CA2W wszAttributeName(cstrAttributeName.c_str());
	BSTR bstrAttributeName = wszAttributeName;


	_ASSERT(pNode);
	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->getAttribute(bstrAttributeName, &ComVarValue);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

	*pstrRet = CW2A(ComVarValue.bstrVal);


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::GetAttributeValue(
	MSXML::IXMLDOMNode* pNode, 
	const std::wstring& cstrAttributeName,
	std::wstring* pstrRet)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement*	pEle		= NULL;
	CComVariant				ComVarValue = NULL; 

	BSTR bstrAttributeName = (LPWSTR)cstrAttributeName.c_str();


	_ASSERT(pNode);
	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->getAttribute(bstrAttributeName, &ComVarValue);
	KAV_PROCESS_ERROR(S_OK == lRetCode);

	*pstrRet = ComVarValue.bstrVal;


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::SetAttribute(
	const char* pcszAttributeName, 
	const char* pcszAttributeValue,
	MSXML::IXMLDOMNode* pNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

    MSXML::IXMLDOMElement* pEle = NULL;

	CA2W wszAttributeName(pcszAttributeName);
	BSTR bstrAttributeName = wszAttributeName;

	CComVariant AttributeValue(pcszAttributeValue);

	
	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}

    lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
    lRetCode = pEle->setAttribute(bstrAttributeName, AttributeValue);
	KAV_COM_PROCESS_ERROR(lRetCode);

	
	nResult = true;

Exit0:

    KAV_COM_RELEASE(pEle);

    return nResult;
}

int MSXMLManager::SetAttribute(
	const wchar_t* pcszAttributeName, 
	const wchar_t* pcszAttributeValue,
	MSXML::IXMLDOMNode* pNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;

	BSTR bstrAttributeName = (LPWSTR)pcszAttributeName;

	CComVariant AttributeValue(pcszAttributeValue);


	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->setAttribute(bstrAttributeName, AttributeValue);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::SetAttribute(
	const std::string& cstrAttributeName, 
	const std::string& cstrAttributeValue,
	MSXML::IXMLDOMNode* pNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;

	CA2W wszAttributeName(cstrAttributeName.c_str());
	BSTR bstrAttributeName = wszAttributeName;

	CComVariant AttributeValue(cstrAttributeValue.c_str());


	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->setAttribute(bstrAttributeName, AttributeValue);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::SetAttribute(
	const std::wstring& cstrAttributeName, 
	const std::wstring& cstrAttributeValue,
	MSXML::IXMLDOMNode* pNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;

	BSTR bstrAttributeName = (LPWSTR)cstrAttributeName.c_str();

	CComVariant AttributeValue(cstrAttributeValue.c_str());


	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->setAttribute(bstrAttributeName, AttributeValue);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::SetText(const char* pcszText, MSXML::IXMLDOMNode* pNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;

	CA2W wszText(pcszText);
	BSTR bstrText = wszText;


	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}


	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);

	lRetCode = pEle->put_text(bstrText);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::SetText(const wchar_t* pcszText, MSXML::IXMLDOMNode* pNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;

	BSTR bstrText = (LPWSTR)pcszText;


	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}


	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);

	lRetCode = pEle->put_text(bstrText);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::SetText(const std::string& cstrText, MSXML::IXMLDOMNode* pNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;

	CA2W wszText(cstrText.c_str());
	BSTR bstrText = wszText;


	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}


	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);

	lRetCode = pEle->put_text(bstrText);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::SetText(const std::wstring& cstrText, MSXML::IXMLDOMNode* pNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT	lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;

	BSTR bstrText = (LPWSTR)cstrText.c_str();


	if (!pNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pNode = m_pCurNode;
	}


	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);

	lRetCode = pEle->put_text(bstrText);
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::CreateRootNode(const char* pcszNodeName)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pRootElement = NULL;


	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);
	KAV_PROCESS_ERROR(!m_pRootNode);


	lRetCode = m_pXMLDoc->createElement(CComBSTR(pcszNodeName), &pRootElement);
	KAV_COM_PROCESS_ERROR(lRetCode);

	m_pXMLDoc->appendChild(pRootElement, &m_pRootNode);

	m_pCurNode = m_pRootNode;


	nResult = true;	

Exit0:

	if (!nResult)
	{
		KAV_COM_RELEASE(m_pRootNode);
	}

	return nResult;
}

int MSXMLManager::CreateRootNode(const wchar_t* pcszNodeName)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pRootElement = NULL;


	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);
	KAV_PROCESS_ERROR(!m_pRootNode);


	lRetCode = m_pXMLDoc->createElement(CComBSTR(pcszNodeName), &pRootElement);
	KAV_COM_PROCESS_ERROR(lRetCode);

	m_pXMLDoc->appendChild(pRootElement, &m_pRootNode);

	m_pCurNode = m_pRootNode;


	nResult = true;	

Exit0:

	if (!nResult)
	{
		KAV_COM_RELEASE(m_pRootNode);
	}

	return nResult;
}

int MSXMLManager::CreateRootNode(const std::string& cstrNodeName)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pRootElement = NULL;


	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);
	KAV_PROCESS_ERROR(!m_pRootNode);


	lRetCode = m_pXMLDoc->createElement(CComBSTR(cstrNodeName.c_str()), &pRootElement);
	KAV_COM_PROCESS_ERROR(lRetCode);

	m_pXMLDoc->appendChild(pRootElement, &m_pRootNode);

	m_pCurNode = m_pRootNode;


	nResult = true;	

Exit0:

	if (!nResult)
	{
		KAV_COM_RELEASE(m_pRootNode);
	}

	return nResult;
}

int MSXMLManager::CreateRootNode(const std::wstring& cstrNodeName)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pRootElement = NULL;


	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);
	KAV_PROCESS_ERROR(!m_pRootNode);


	lRetCode = m_pXMLDoc->createElement(CComBSTR(cstrNodeName.c_str()), &pRootElement);
	KAV_COM_PROCESS_ERROR(lRetCode);

	m_pXMLDoc->appendChild(pRootElement, &m_pRootNode);

	m_pCurNode = m_pRootNode;


	nResult = true;	

Exit0:

	if (!nResult)
	{
		KAV_COM_RELEASE(m_pRootNode);
	}

	return nResult;
}

int MSXMLManager::CreateXMLNode(
	const char* pcszNodeName,
	MSXML::IXMLDOMNode** ppRetNode /* = NULL */,
	MSXML::IXMLDOMNode* pParentNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMNode* pNode = NULL;


	lRetCode = AppendNode(pcszNodeName, &pNode, pParentNode);
	KAV_COM_PROCESS_ERROR(lRetCode);


	if (ppRetNode)
		*ppRetNode = pNode;

	m_pCurNode = pNode;


	nResult = true;	

Exit0:
	
	return nResult;
}

int MSXMLManager::CreateXMLNode(
	const wchar_t* pcszNodeName,
	MSXML::IXMLDOMNode** ppRetNode /* = NULL */,
	MSXML::IXMLDOMNode* pParentNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMNode* pNode = NULL;


	lRetCode = AppendNode(pcszNodeName, &pNode, pParentNode);
	KAV_COM_PROCESS_ERROR(lRetCode);


	if (ppRetNode)
		*ppRetNode = pNode;

	m_pCurNode = pNode;


	nResult = true;	

Exit0:

	return nResult;
}

int MSXMLManager::CreateXMLNode(
	const std::string&  cstrNodeName,
	MSXML::IXMLDOMNode** ppRetNode /* = NULL */,
	MSXML::IXMLDOMNode* pParentNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMNode* pNode = NULL;


	lRetCode = AppendNode(cstrNodeName, &pNode, pParentNode);
	KAV_COM_PROCESS_ERROR(lRetCode);


	if (ppRetNode)
		*ppRetNode = pNode;

	m_pCurNode = pNode;


	nResult = true;	

Exit0:

	return nResult;
}

int MSXMLManager::CreateXMLNode(
	const std::wstring&  cstrNodeName,
	MSXML::IXMLDOMNode** ppRetNode /* = NULL */,
	MSXML::IXMLDOMNode* pParentNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMNode* pNode = NULL;


	lRetCode = AppendNode(cstrNodeName, &pNode, pParentNode);
	KAV_COM_PROCESS_ERROR(lRetCode);


	if (ppRetNode)
		*ppRetNode = pNode;

	m_pCurNode = pNode;


	nResult = true;	

Exit0:

	return nResult;
}

int MSXMLManager::AppendNode(
	const char* pcszNodeName,
	MSXML::IXMLDOMNode** ppRetNode /* = NULL */,
	MSXML::IXMLDOMNode* pParentNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;
	
	MSXML::IXMLDOMElement*	pElement = NULL;
	MSXML::IXMLDOMNode*		pNode	 = NULL;


	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	if (!pParentNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pParentNode = m_pCurNode;
	}

	lRetCode = m_pXMLDoc->createElement(CComBSTR(pcszNodeName), &pElement);
	KAV_COM_PROCESS_ERROR(lRetCode);

	lRetCode = pParentNode->appendChild(pElement, &pNode);
	KAV_COM_PROCESS_ERROR(lRetCode);

	if (ppRetNode)
		*ppRetNode = pNode;


	nResult = true;	

Exit0:
	
	if (!nResult)
	{
		KAV_COM_RELEASE(pNode);
	}

	KAV_COM_RELEASE(pElement);


	return nResult;
}

int MSXMLManager::AppendNode(
	const wchar_t* pcszNodeName,
	MSXML::IXMLDOMNode** ppRetNode /* = NULL */,
	MSXML::IXMLDOMNode* pParentNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement*	pElement = NULL;
	MSXML::IXMLDOMNode*		pNode	 = NULL;


	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	if (!pParentNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pParentNode = m_pCurNode;
	}

	lRetCode = m_pXMLDoc->createElement(CComBSTR(pcszNodeName), &pElement);
	KAV_COM_PROCESS_ERROR(lRetCode);

	lRetCode = pParentNode->appendChild(pElement, &pNode);
	KAV_COM_PROCESS_ERROR(lRetCode);

	if (ppRetNode)
		*ppRetNode = pNode;


	nResult = true;	

Exit0:

	if (!nResult)
	{
		KAV_COM_RELEASE(pNode);
	}

	KAV_COM_RELEASE(pElement);


	return nResult;
}

int MSXMLManager::AppendNode(
	const std::string&  cstrNodeName,
	MSXML::IXMLDOMNode** ppRetNode /* = NULL */,
	MSXML::IXMLDOMNode* pParentNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement*	pElement = NULL;
	MSXML::IXMLDOMNode*		pNode	 = NULL;


	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	if (!pParentNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pParentNode = m_pCurNode;
	}

	lRetCode = m_pXMLDoc->createElement(CComBSTR(cstrNodeName.c_str()), &pElement);
	KAV_COM_PROCESS_ERROR(lRetCode);

	lRetCode = pParentNode->appendChild(pElement, &pNode);
	KAV_COM_PROCESS_ERROR(lRetCode);

	if (ppRetNode)
		*ppRetNode = pNode;


	nResult = true;	

Exit0:

	if (!nResult)
	{
		KAV_COM_RELEASE(pNode);
	}

	KAV_COM_RELEASE(pElement);


	return nResult;
}

int MSXMLManager::AppendNode(
	const std::wstring&  cstrNodeName,
	MSXML::IXMLDOMNode** ppRetNode /* = NULL */,
	MSXML::IXMLDOMNode* pParentNode /* = NULL */)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement*	pElement = NULL;
	MSXML::IXMLDOMNode*		pNode	 = NULL;


	_ASSERT(m_pXMLDoc);
	KAV_PROCESS_ERROR(m_pXMLDoc);


	if (!pParentNode)
	{
		KAV_PROCESS_ERROR(m_pCurNode);
		pParentNode = m_pCurNode;
	}

	lRetCode = m_pXMLDoc->createElement(CComBSTR(cstrNodeName.c_str()), &pElement);
	KAV_COM_PROCESS_ERROR(lRetCode);

	lRetCode = pParentNode->appendChild(pElement, &pNode);
	KAV_COM_PROCESS_ERROR(lRetCode);

	if (ppRetNode)
		*ppRetNode = pNode;


	nResult = true;	

Exit0:

	if (!nResult)
	{
		KAV_COM_RELEASE(pNode);
	}

	KAV_COM_RELEASE(pElement);


	return nResult;
}

int MSXMLManager::RemoveNode(MSXML::IXMLDOMNode* pNode)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMNode* pParentNode = NULL;


	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->get_parentNode(&pParentNode);
	KAV_COM_PROCESS_ERROR(lRetCode);

	if (!pParentNode || pNode == m_pRootNode)
	{
		lRetCode = m_pXMLDoc->removeChild(pNode, &pNode);
		KAV_COM_PROCESS_ERROR(lRetCode);

		m_pRootNode = NULL;
		m_pCurNode= NULL;
	}
	else
	{
		lRetCode = pParentNode->removeChild(pNode, &pNode);
		KAV_COM_PROCESS_ERROR(lRetCode);
	}


	nResult = true;	

Exit0:

	return nResult;
}

int MSXMLManager::RemoveChildNodes(MSXML::IXMLDOMNode* pParentNode)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMNode* pChildNode	= NULL;
	MSXML::IXMLDOMNode* pNextNode	= NULL;


	KAV_PROCESS_ERROR(pParentNode);


	for (pParentNode->get_firstChild(&pChildNode); pChildNode; )
	{
		lRetCode = pChildNode->get_nextSibling(&pNextNode);
		if (S_OK != lRetCode)
			pNextNode = NULL;

		pParentNode->removeChild(pChildNode, &pChildNode);

		pChildNode = pNextNode;
	}


	nResult = true;	

Exit0:

	return nResult;
}

int MSXMLManager::RemoveAttribute(
	MSXML::IXMLDOMNode* pNode, 
	const char* pcszAttributeName)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;


	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->removeAttribute(CComBSTR(pcszAttributeName));
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;	

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::RemoveAttribute(
	MSXML::IXMLDOMNode* pNode, 
	const wchar_t* pcszAttributeName)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;


	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->removeAttribute(CComBSTR(pcszAttributeName));
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;	

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::RemoveAttribute(
	MSXML::IXMLDOMNode* pNode, 
	const std::string& cstrAttributeName)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;


	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->removeAttribute(CComBSTR(cstrAttributeName.c_str()));
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;	

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

int MSXMLManager::RemoveAttribute(
	MSXML::IXMLDOMNode* pNode, 
	const std::wstring& cstrAttributeName)
{
	int		nResult		= false;
	HRESULT lRetCode	= E_FAIL;

	MSXML::IXMLDOMElement* pEle = NULL;


	KAV_PROCESS_ERROR(pNode);

	lRetCode = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	KAV_COM_PROCESS_ERROR(lRetCode);

	KAV_PROCESS_ERROR(pEle);
	lRetCode = pEle->removeAttribute(CComBSTR(cstrAttributeName.c_str()));
	KAV_COM_PROCESS_ERROR(lRetCode);


	nResult = true;	

Exit0:

	KAV_COM_RELEASE(pEle);

	return nResult;
}

#endif // #ifdef WIN32
