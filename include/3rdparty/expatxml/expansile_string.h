/* -------------------------------------------------------------------------
//	文件名		：	xmlcfg/expansile_string.h
//	创建者		：	Jiang Wangsheng
//	创建时间	：	2008-10-29 上午 11:17:10
//	功能描述	：	对字符串开始处的特定子字符串进行扩展，在expat_reader
					对xml的属性值的解析处用到，看一个例子
					条件：存在$(home)-->c:\app的ExpansileString
					结果：$(home)\jiang 扩展为 c:\app\jiang					
//					
//					
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __XMLCFG_EXPANSILE_STRING_H__
#define __XMLCFG_EXPANSILE_STRING_H__

#ifdef _USER_MODE_
# include <string>
#else
# include "kernel_stl/string.hpp"
#endif


// -------------------------------------------------------------------------
namespace xmlcfg {
// -------------------------------------------------------------------------


struct ExpansileString 
{
	WCHAR* unexpanded;		// 未扩展的字符串
	size_t cchUnexpanded;	 
	WCHAR* expanded;	// 扩展后的字符串，NULL表示无效，但""有效
	size_t cchExpanded;
};

inline
void getExpandedString( LPCWSTR szInput, std::wstring &strOutput,
					    ExpansileString* pEStrs, ULONG nEStrs )
{
	size_t cchInput = wcslen( szInput );
	strOutput.assign( szInput, cchInput );

	for ( ULONG l = 0; l < nEStrs; ++l )
	{
		if ( pEStrs[l].expanded == NULL )
			continue;
		if ( cchInput < pEStrs[l].cchUnexpanded )
			continue;
#ifdef _USER_MODE_
		if ( memcmp( pEStrs[l].unexpanded, szInput, pEStrs[l].cchUnexpanded * sizeof(WCHAR) ) == 0 )
#else
		if ( RtlCompareMemory( pEStrs[l].unexpanded, szInput, pEStrs[l].cchUnexpanded * sizeof(WCHAR) ) == 
				pEStrs[l].cchUnexpanded * sizeof(WCHAR) )
#endif				
		{
			strOutput.assign( pEStrs[l].expanded, pEStrs[l].cchExpanded );
			if ( cchInput > pEStrs[l].cchUnexpanded )
				strOutput.append( szInput + pEStrs[l].cchUnexpanded, cchInput - pEStrs[l].cchUnexpanded );
			break;
		}							
	}
}

// -------------------------------------------------------------------------
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
//	$Log: $

#endif /* __XMLCFG_EXPANSILE_STRING_H__ */
