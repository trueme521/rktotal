/* -------------------------------------------------------------------------
// 文件名		:	xmlcfg/simple_reader.h
// 创建人		:	Zhuyie
// 创建时间	:	2008-8-8 15:50:17
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __XMLCFG_SIMPLE_READER_H__
#define __XMLCFG_SIMPLE_READER_H__

#ifndef _XMLCFG_INTERNAL_
#	error Internal use only!
#endif

#ifdef _USER_MODE_
#	include <vector>
#	include <stack>
#	include <xstring>
#else
#	include "kernel_stl/utility.hpp"
#	include "kernel_stl/string.hpp"
#	include "kernel_stl/vector.hpp"
#	include "kernel_stl/stack.hpp"
#endif

// -------------------------------------------------------------------------

#ifndef _USER_MODE_

int __cdecl zzz_iswspace(wchar_t c)
{
	if ( c == 0x20 || (c >= 0x09 && c <= 0x0D) )
		return true;
	else
		return false;
}
int __cdecl zzz_iswpunct(wchar_t c)
{
	return 0;
}
#define iswspace zzz_iswspace
#define iswpunct zzz_iswpunct

#endif

// -------------------------------------------------------------------------
namespace xmlcfg {
// -------------------------------------------------------------------------

struct AbstractParser
{
	enum ParserType { PT_Dispatcher, PT_XmlHead, PT_STag, PT_ETag };
	virtual ParserType GetType() = 0;
	virtual bool OnSpecialChar(WCHAR ch) = 0;
	virtual bool OnSpace(const WCHAR *space, unsigned int len) = 0;
	virtual bool OnString(const WCHAR *string, unsigned int len) = 0;
	virtual bool OnIdentifier(const WCHAR *id, unsigned int len) = 0;
	virtual bool IsCompleted() = 0;
	virtual ~AbstractParser(){ };
};

struct XmlHeadParser : AbstractParser
{
	// attr_list : (space)(name)[space]=[space]"(value)"
	//                1     2          3           4
	// [space]<?xml[attr_list]?>
	//        12 3            45

	unsigned int m_nStage;
	unsigned int m_nAttrStage;
	typedef std::pair<std::wstring, std::wstring> AttrItemT;
	std::vector<AttrItemT> m_vecAttrItems;

	XmlHeadParser()
	{
		m_nStage = 0;
		m_nAttrStage = 0;
	}

	AbstractParser::ParserType GetType()
	{
		return AbstractParser::PT_XmlHead;
	}
	bool OnSpecialChar(WCHAR ch)
	{
		if ( ch == L'<' && m_nStage == 0 ||
			ch == L'?' && m_nStage == 1 ||
			ch == L'?' && m_nStage == 3 ||
			ch == L'>' && m_nStage == 4 )
		{
			++m_nStage;
			return true;
		}
		else if ( ch == L'=' && m_nStage == 3 && m_nAttrStage == 2 )
		{
			++m_nAttrStage;
			return true;
		}
		return false;
	}
	bool OnSpace(const WCHAR *space, unsigned int len)
	{
		if ( m_nStage == 3 && m_nAttrStage == 0)
		{
			++m_nAttrStage;
			return true;
		}
		else if ( m_nStage == 0 || 
			m_nStage == 3 && (m_nAttrStage == 2 || m_nAttrStage == 3) )
		{
			return true;
		}
		return false;
	}
	bool OnString(const WCHAR *string, unsigned int len)
	{
		if ( m_nStage == 3 && m_nAttrStage == 3 )
		{
			if ( string && len )
			{
				AttrItemT &rItem = m_vecAttrItems.back();
				rItem.second.assign( string, len );
			}
			m_nAttrStage = 0;
			return true;
		}
		return false;
	}
	bool OnIdentifier(const WCHAR *id, unsigned int len)
	{
		if ( m_nStage == 2 && len == 3 && 
			id[0] == L'x' && id[1] == L'm' && id[2] == L'l' )
		{
			++m_nStage;
			return true;
		}
		else if ( m_nStage == 3 && m_nAttrStage == 1 )
		{
			AttrItemT aItem;
			aItem.first.assign( id, len );
			m_vecAttrItems.push_back( aItem );
			++m_nAttrStage;
			return true;
		}
		return false;
	}
	bool IsCompleted()
	{
		return (m_nStage == 5);
	}
};

struct STagParser : AbstractParser
{
	// attr_list : (space)(name)[space]=[space]"(value)"
	//                1     2          3           4
	// [space]<(name)[attr_list]>
	//        1   2             3

	unsigned int m_nStage;
	unsigned int m_nAttrStage;
	std::wstring m_szTag;
	typedef std::pair<std::wstring, std::wstring> AttrItemT;
	std::vector<AttrItemT> m_vecAttrItems;

	STagParser()
	{
		m_nStage = 0;
		m_nAttrStage = 0;
	}

	AbstractParser::ParserType GetType()
	{
		return AbstractParser::PT_STag;
	}
	bool OnSpecialChar(WCHAR ch)
	{
		if ( ch == L'<' && m_nStage == 0 ||
			ch == L'>' && m_nStage == 2 && m_nAttrStage < 2 )
		{
			++m_nStage;
			return true;
		}
		else if ( ch == L'=' && m_nStage == 2 && m_nAttrStage == 2 )
		{
			++m_nAttrStage;
			return true;
		}
		return false;
	}
	bool OnSpace(const WCHAR *space, unsigned int len)
	{
		if ( m_nStage == 2 && m_nAttrStage == 0 )
		{
			++m_nAttrStage;
			return true;
		}
		else if ( m_nStage == 0 || 
			m_nStage == 2 && (m_nAttrStage == 2 || m_nAttrStage == 3) )
		{
			return true;
		}
		return false;
	}
	bool OnString(const WCHAR *string, unsigned int len)
	{
		if ( m_nStage == 2 && m_nAttrStage == 3 )
		{
			if ( string && len )
			{
				AttrItemT &rItem = m_vecAttrItems.back();

				// 将"&quot;"重新变为"\""
				WCHAR quotChars[] = L"&quot;";
				unsigned int matchedCount = 0;
				const WCHAR *p = string;
				for ( unsigned int i = 0; i < len; ++i )
				{
					if ( *p != quotChars[matchedCount] )
					{
						rItem.second.append( p - matchedCount, matchedCount + 1 );
						matchedCount = 0;
					}
					else
					{
						if ( ++matchedCount == 6 )
						{
							rItem.second.append( L"\"", 1 );
							matchedCount = 0;
						}
					}
					++p;
				}

				//	rItem.second.assign( string, len );
			}
			m_nAttrStage = 0;
			return true;
		}
		return false;
	}
	bool OnIdentifier(const WCHAR *id, unsigned int len)
	{
		if ( m_nStage == 1 )
		{
			m_szTag.assign( id, len );
			++m_nStage;
			return true;
		}
		else if ( m_nStage == 2 && m_nAttrStage == 1 )
		{
			AttrItemT aItem;
			aItem.first.assign( id, len );
			m_vecAttrItems.push_back( aItem );
			++m_nAttrStage;
			return true;
		}
		return false;
	}
	bool IsCompleted()
	{
		return (m_nStage == 3);
	}
};

struct ETagParser : AbstractParser
{
	// [space]</(name)[space]>
	//        12   3         4

	unsigned int m_nStage;
	std::wstring m_szTag;

	ETagParser()
	{
		m_nStage = 0;
	}

	AbstractParser::ParserType GetType()
	{
		return AbstractParser::PT_ETag;
	}
	bool OnSpecialChar(WCHAR ch)
	{
		if ( ch == L'<' && m_nStage == 0 ||
			ch == L'/' && m_nStage == 1 ||
			ch == L'>' && m_nStage == 3 )
		{
			++m_nStage;
			return true;
		}
		return false;
	}
	bool OnSpace(const WCHAR *space, unsigned int len)
	{
		if ( m_nStage == 0 || m_nStage == 3 )
			return true;
		else
			return false;
	}
	bool OnString(const WCHAR *string, unsigned int len)
	{
		return false;
	}
	bool OnIdentifier(const WCHAR *id, unsigned int len)
	{
		if ( id && len && m_nStage == 2 )
		{
			m_szTag.assign(id, len);
			++m_nStage;
			return true;
		}
		return false;
	}
	bool IsCompleted()
	{
		return (m_nStage == 4);
	}
};

class XmlCfgParser : public AbstractParser
{
	std::stack<Setting*> m_SettingStack;
	std::stack<AbstractParser*> m_ParserStack;
	bool m_bLeftAngleBracket;

	XmlCfgParser(const XmlCfgParser &rhs);
	XmlCfgParser& operator=(const XmlCfgParser &rhs);

public:
	XmlCfgParser(Setting* rootSetting)
	{
		m_SettingStack.push( rootSetting );
		m_ParserStack.push( new XmlHeadParser );
		m_bLeftAngleBracket = false;
	}
	~XmlCfgParser()
	{
		ASSERT( m_ParserStack.empty() );
	}

	AbstractParser::ParserType GetType()
	{
		return AbstractParser::PT_Dispatcher;
	}
	bool OnSpecialChar(WCHAR ch)
	{
		if ( !m_ParserStack.empty() )
		{
			if ( !m_ParserStack.top()->OnSpecialChar(ch) )
				return false;

			Setting *pSetting = m_SettingStack.top();
			AbstractParser *pParser = m_ParserStack.top();
			if ( pParser->IsCompleted() )
			{
				AbstractParser::ParserType aType = pParser->GetType();
				if ( aType == AbstractParser::PT_STag )
				{
					STagParser *pSTagParser = (STagParser*)pParser;
					if ( wcscmp(pSTagParser->m_szTag.c_str(), L"xmlcfg") == 0 )
					{
						if ( wcslen(pSetting->getName()) > 0 )
							return false;
					}
					else
					{
						pSetting = pSetting->createChild( pSTagParser->m_szTag.c_str(), (int)pSTagParser->m_szTag.length() );
						if ( !pSetting )
							return false;
						m_SettingStack.push( pSetting );
					}

					for ( unsigned int i = 0; i < pSTagParser->m_vecAttrItems.size(); ++i )
					{
						std::wstring &name = pSTagParser->m_vecAttrItems[i].first;
						std::wstring &value = pSTagParser->m_vecAttrItems[i].second;
						Attribute *pAttr = pSetting->createAttribute( vtString, name.c_str(), (int)name.length() );
						if ( pAttr )
							pAttr->putString( value.c_str(), (int)value.length() );
					}
				}
				else if ( aType == AbstractParser::PT_ETag )
				{
					ETagParser *pSTagParser = (ETagParser*)pParser;
					if ( wcscmp(pSTagParser->m_szTag.c_str(), pSetting->getName()) == 0 ||
						wcslen(pSetting->getName()) == 0 && wcscmp(pSTagParser->m_szTag.c_str(), L"xmlcfg") == 0 )
					{
						m_SettingStack.pop();
					}
					else
						return false;
				}

				delete pParser;
				m_ParserStack.pop();
			}

			return true;
		}

		if ( ch == L'<' )
		{
			if ( m_bLeftAngleBracket )
				return false;
			else
				m_bLeftAngleBracket = true;
		}
		else if ( ch == L'/' && m_bLeftAngleBracket )
		{
			ETagParser *pParser = new ETagParser;
			pParser->OnSpecialChar( L'<' );
			pParser->OnSpecialChar( L'/' );
			m_ParserStack.push( pParser );
			m_bLeftAngleBracket = false;
		}

		return true;
	}
	bool OnSpace(const WCHAR *space, unsigned int len)
	{
		if ( m_bLeftAngleBracket )
		{
			STagParser *pParser = new STagParser;
			pParser->OnSpecialChar( L'<' );
			m_ParserStack.push( pParser );
			m_bLeftAngleBracket = false;
		}

		if ( !m_ParserStack.empty() && 
			!m_ParserStack.top()->OnSpace(space, len) )
		{
			return false;
		}
		return true;
	}
	bool OnString(const WCHAR *string, unsigned int len)
	{
		if ( m_bLeftAngleBracket )
		{
			STagParser *pParser = new STagParser;
			pParser->OnSpecialChar( L'<' );
			m_ParserStack.push( pParser );
			m_bLeftAngleBracket = false;
		}

		if ( !m_ParserStack.empty() && 
			!m_ParserStack.top()->OnString(string, len) )
		{
			return false;
		}
		return true;
	}
	bool OnIdentifier(const WCHAR *id, unsigned int len)
	{
		if ( m_bLeftAngleBracket )
		{
			STagParser *pParser = new STagParser;
			pParser->OnSpecialChar( L'<' );
			m_ParserStack.push( pParser );
			m_bLeftAngleBracket = false;
		}

		if ( !m_ParserStack.empty() && 
			!m_ParserStack.top()->OnIdentifier(id, len) )
		{
			return false;
		}
		return true;
	}
	bool IsCompleted()
	{
		return false;
	}
};

class XmlCfgReader
{
public:
	bool process(InputStream &is, XmlCfgParser &parser)
	{
		// 读BOM，咱们的文件是固定以UTF-16格式编码的
		unsigned char bom[2] = { 0 };
		is.read(bom, 2, NULL);
		if ( bom[0] != 0xFF || bom[1] != 0xFE )
			return false;

		bool bInString = false;
		std::vector<WCHAR> vecTag;
		unsigned int cbReaded = 0;
		bool bParseResult = true;
		do 
		{
			WCHAR szBuffer[512];
			is.read( szBuffer, sizeof(WCHAR) * 512, &cbReaded );
			cbReaded /= sizeof(WCHAR);
			for ( unsigned int i = 0; i < cbReaded && bParseResult; ++i )
			{
				if ( szBuffer[i] == L'\"' )			// 引号，可能是一个字符串的开始或结束
				{
					if ( bInString )
					{
						if ( vecTag.empty() )
							bParseResult = parser.OnString( NULL, 0 );
						else
							bParseResult = parser.OnString( &(*vecTag.begin()), (unsigned int)vecTag.size() );
						vecTag.clear();
						bInString = false;
					}
					else
						bInString = true;
				}
				else if ( bInString )				// 字符串的内容
				{
					vecTag.push_back( szBuffer[i] );
				}
				else if ( szBuffer[i] == L'<' || szBuffer[i] == L'>' ||
					szBuffer[i] == L'/' || szBuffer[i] == L'?' || 
					szBuffer[i] == L'=' )		// 特殊字符
				{
					if ( !vecTag.empty() )
					{
						if ( iswspace(*vecTag.begin()) )
							bParseResult = parser.OnSpace( &(*vecTag.begin()), (unsigned int)vecTag.size() );
						else
							bParseResult = parser.OnIdentifier( &(*vecTag.begin()), (unsigned int)vecTag.size() );
						vecTag.clear();
					}
					bParseResult = parser.OnSpecialChar( szBuffer[i] );
				}
				else if ( iswspace(szBuffer[i]) )	// 空格、回车换行等
				{
					if ( vecTag.empty() || iswspace(*vecTag.begin()) )
						vecTag.push_back( szBuffer[i] );
					else
					{
						bParseResult = parser.OnIdentifier( &(*vecTag.begin()), (unsigned int)vecTag.size() );
						vecTag.clear();
						vecTag.push_back( szBuffer[i] );
					}
				}
				else if ( !iswpunct(szBuffer[i]) )	// 其它常规字符
				{
					if ( vecTag.empty() || !iswspace(*vecTag.begin()) )
						vecTag.push_back( szBuffer[i] );
					else
					{
						bParseResult = parser.OnSpace( &(*vecTag.begin()), (unsigned int)vecTag.size() );
						vecTag.clear();
						vecTag.push_back( szBuffer[i] );
					}
				}
				else
					return false;					// 不允许的字符
			}

			if ( !bParseResult )
				return false;

		} while ( cbReaded > 0 );

		if ( bInString )
			return false;							// 字符串的引号不匹配
		else if ( !vecTag.empty() )					// 最后一个(空格)或(标志符)
		{
			if ( iswspace(*vecTag.begin()) )
				parser.OnSpace( &(*vecTag.begin()), (unsigned int)vecTag.size() );
			else
				parser.OnIdentifier( &(*vecTag.begin()), (unsigned int)vecTag.size() );
		}

		return true;
	}
};


// -------------------------------------------------------------------------
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
// $Log: $

#endif /* __XMLCFG_SIMPLE_READER_H__ */
