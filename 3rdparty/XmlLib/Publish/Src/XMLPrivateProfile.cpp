///////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   XMLPrivateProfile.cpp
//  Version     :   1.0
//  Creator     :   YangXD(yxdh)
//  Date        :   2005-8-26 11:39:53
//  Comment     :   Implementation of the XMLPrivateProfile class.
//
///////////////////////////////////////////////////////////////////////////////
#include "XMLPrivateProfile.h"
#include "KXMLManager.h"
#include "KAVPublic.h"


int	GetPrivateProfile(
	const std::string&  cstrAppName,
	const std::string&  cstrKeyName,
	std::string* pstrRet,
	const std::string&  cstrFileName)
{
    int nResult		= false;
    int nRetCode	= false;

	
	KXMLManager		XmlManager;
	
	nRetCode = XmlManager.LoadFile(cstrFileName.c_str());
	KAV_PROCESS_ERROR(nRetCode);

	nRetCode = XmlManager.GetAttributeValue(
		XmlManager.FirstChildElement().FirstChildElement(cstrAppName).Node(),
		cstrKeyName,
		pstrRet);
	KAV_PROCESS_ERROR(nRetCode);


    nResult = true;

Exit0:	// fail exit

    return nResult;
}

int	GetPrivateProfile(
	const std::string&  cstrAppName,
	const std::string&  cstrKeyName,
	int* pnRet,
	const std::string&  cstrFileName)
{
    int nResult		= false;
    int nRetCode	= false;

	std::string		strTmp;
	

	KXMLManager XmlManager;
	
	nRetCode = XmlManager.LoadFile(cstrFileName.c_str());
	KAV_PROCESS_ERROR(nRetCode);

	nRetCode = XmlManager.GetAttributeValue(
		XmlManager.FirstChildElement().FirstChildElement(cstrAppName).Node(),
		cstrKeyName,
		&strTmp);
	KAV_PROCESS_ERROR(nRetCode);


	*pnRet = atoi(strTmp.c_str());


    nResult = true;

Exit0:	// fail exit

    return nResult;
}

int	WritePrivateProfile(
	const std::string&  cstrAppName,
	const std::string&  cstrKeyName,
	const std::string&  cstr,
	const std::string&  cstrFileName)
{
    int nResult		= false;
    int nRetCode	= false;

	
	KXMLManager		XmlManager;
	
	nRetCode = XmlManager.LoadFile(cstrFileName.c_str());
	KAV_PROCESS_ERROR(nRetCode);

	nRetCode = XmlManager.SetAttribute(
		cstrKeyName,
		cstr,
		XmlManager.FirstChildElement().FirstChildElement(cstrAppName).Node());
	KAV_PROCESS_ERROR(nRetCode);


	XmlManager.SaveFile(cstrFileName.c_str());

    nResult = true;

Exit0:	// fail exit

    return nResult;
}

