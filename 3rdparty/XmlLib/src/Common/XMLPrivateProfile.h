///////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   XMLPrivateProfile.h
//  Version     :   1.0
//  Creator     :   YangXD(yxdh)
//  Date        :   2005-8-26 11:39:51
//  Comment     :   Interface for the XMLPrivateProfile class.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _XML_PRIVATE_PROFILE_H_
#define _XML_PRIVATE_PROFILE_H_

#include	<string>

// 适用于采用类INI格式的XML配置文件


int			GetPrivateProfile(
				const std::string&  cstrAppName,
				const std::string&  cstrKeyName,
				std::string* pstrRet,
				const std::string&  cstrFileName);

int			GetPrivateProfile(
				const std::string&  cstrAppName,
				const std::string&  cstrKeyName,
				int* pnRet,
				const std::string&  cstrFileName);

int			WritePrivateProfile(
				const std::string&  cstrAppName,
				const std::string&  cstrKeyName,
				const std::string&  cstr,
				const std::string&  cstrFileName);


#endif // #ifndef _XML_PRIVATE_PROFILE_H_
