/* -------------------------------------------------------------------------
//	文件名		：	xmlcfg.cpp
//	创建者		：	Zhuyie
//	创建时间	：	2007-11-6 20:49:44
//	功能描述	：	
//
//	$Id: $
// -----------------------------------------------------------------------*/

#ifdef _USER_MODE_
#	include <windows.h>
#ifndef ASSERT
#	include <cassert>
#	define ASSERT assert
#endif
#endif

#include "xmlcfg.h"

#define _XMLCFG_INTERNAL_

#include "expat_reader.h"

// -------------------------------------------------------------------------
namespace xmlcfg {
// -------------------------------------------------------------------------

void* StdMemMgr::allocMemory(unsigned int cb)
{
#ifdef _USER_MODE_
	return ::operator new( cb );
#else
	return ::ExAllocatePoolWithTag( PagedPool, cb, PoolTag_Xmlc );
#endif
}

void StdMemMgr::freeMemory(void* ptr)
{
	if ( ptr == NULL )
		return;
#ifdef _USER_MODE_
	::operator delete( ptr );
#else
	::ExFreePoolWithTag( ptr, PoolTag_Xmlc );
#endif
}

void StdMemMgr::copyMemory(void* dest, const void* src, unsigned int cb)
{
	memcpy( dest, src, cb );
}

int StdMemMgr::compareMemory(const void* buf1, const void* buf2, unsigned int count)
{
	if (!count)
		return(0);

	while ( --count && *(char *)buf1 == *(char *)buf2 ) {
		buf1 = (char *)buf1 + 1;
		buf2 = (char *)buf2 + 1;
	}

	return( *((unsigned char *)buf1) - *((unsigned char *)buf2) );
}

void StdMemMgr::setMemory(void* dest, int c, unsigned int count)
{
	memset( dest, c, count );
}

LPCWSTR StdMemMgr::dupString(LPCWSTR str, int &lenOfStr)
{
	if ( lenOfStr == -1 )
		lenOfStr = (int)wcslen( str );
	WCHAR* buffer = (WCHAR*) allocMemory( sizeof(WCHAR) * (lenOfStr + 1) );
	if ( buffer )
	{
		copyMemory( buffer, str, sizeof(WCHAR) * lenOfStr );
		buffer[lenOfStr] = L'\0';
	}
	return (LPCWSTR)buffer;
}

void StdMemMgr::freeString(LPCWSTR str)
{
	freeMemory( (void*)str );
}

int StdMemMgr::compareString(LPCWSTR str1, LPCWSTR str2, int len)
{
	return (len == -1) ? wcscmp(str1, str2) : wcsncmp(str1, str2, len);
}

LPCWSTR StdMemMgr::catString(LPCWSTR str1, LPCWSTR str2, int str2Len)
{
	if (str2Len == -1)
		str2Len = (int)wcslen( str2 );

	int str1Len = (int)wcslen( str1 );
	WCHAR* buffer = (WCHAR*) allocMemory( sizeof(WCHAR) * (str1Len + str2Len + 1) );
	if ( buffer )
	{
		copyMemory( buffer, str1, sizeof(WCHAR) * str1Len );
		buffer += str1Len;
		copyMemory( buffer, str2, sizeof(WCHAR) * str2Len );
		buffer -= str1Len;
		buffer[str1Len + str2Len] = L'\0';
	}

	return (LPCWSTR)buffer;
}


PMemMgr StdMemMgr::getInstance()
{
	static MemMgr s_instance = { 
		allocMemory, freeMemory, copyMemory, compareMemory, setMemory,
		dupString, freeString, compareString, catString
	};
	return &s_instance;
}

// -------------------------------------------------------------------------

Config::Config(PMemMgr mm)
{
	m_mm = mm;
	void* buffer = (*(m_mm->pfnAllocMemory))( sizeof(Setting) );
	m_root = new( buffer ) Setting( mm, L"" );
}

Config::~Config()
{
	if ( m_root )
	{
		m_root->~Setting();
		(*(m_mm->pfnFreeMemory))( m_root );
	}
}

Setting* Config::getRoot() const
{
	return m_root;
}

Setting* Config::lookupSetting(LPCWSTR path) const
{
	Setting* pSetting = m_root;

	LPCWSTR name = path;
	while ( *path )
	{
		if ( *path == L'.' )
		{
			pSetting = pSetting->lookupChild( name, (int)(path - name) );
			if ( !pSetting )
				return NULL;
			name = path + 1;
		}
		++path;
	}

	pSetting = (name < path) ? pSetting->lookupChild(name, (int)(path - name)) : NULL;
	return pSetting;
}

Attribute* Config::lookupAttribute(LPCWSTR path) const
{
	Setting* pSetting = m_root;

	LPCWSTR name = path;
	while ( *path )
	{
		if ( *path == L'.' )
		{
			pSetting = pSetting->lookupChild( name, (int)(path - name) );
			if ( !pSetting )
				return NULL;
			name = path + 1;
		}
		++path;
	}

	return (name < path) ? pSetting->lookupAttribute(name, (int)(path - name)) : NULL;
}

void Config::clear()
{
	if ( m_root )
	{
		while ( m_root->getChildCount() )
		{
			m_root->deleteChild( m_root->getChild(0) );
		}
		while ( m_root->getAttributeCount() )
		{
			m_root->deleteAttribute( m_root->getAttribute(0) );
		}
	}
}

// ----------------------------------------------------------------------------

bool Config::__writeSetting(OutputStream &os, Setting *setting)
{
	WCHAR symbols[] = { L'<', L'>', L'/', L' ', L'\"', L'=' };
	WCHAR bool_true[] = { L't', L'r', L'u', L'e' };
	WCHAR bool_false[] = { L'f', L'a', L'l', L's', L'e' };
	WCHAR LRCF[] = { L'\r', L'\n' };

	LPCWSTR name = setting->getName();
	unsigned int nameLen = (unsigned int)wcslen(name);
	os.write( &symbols[0], sizeof(WCHAR) );
	os.write( name, sizeof(WCHAR) * nameLen );

	for ( unsigned int i = 0; i < setting->getAttributeCount(); ++i )
	{
		Attribute *attr = setting->getAttribute( i );
		LPCWSTR attrName = attr->getName();
		unsigned int attrNameLen = (unsigned int)wcslen( attrName );

		os.write( &symbols[3], sizeof(WCHAR) );					// 空格
		os.write( attrName, sizeof(WCHAR) * attrNameLen );		// 属性名
		os.write( &symbols[5], sizeof(WCHAR) );					// =
		os.write( &symbols[4], sizeof(WCHAR) );					// "				
		LPCWSTR value = NULL;
		attr->getString( value );
		if ( value )
			__writeString( os, value );

		os.write( &symbols[4], sizeof(WCHAR) );					// "
	}

	os.write( &symbols[1], sizeof(WCHAR) );
	os.write( LRCF, sizeof(LRCF) );

	for ( unsigned int i = 0; i < setting->getChildCount(); ++i )
	{
		Setting *child = setting->getChild( i );
		__writeSetting( os, child );
	}

	os.write( &symbols[0], sizeof(WCHAR) );
	os.write( &symbols[2], sizeof(WCHAR) );
	os.write( name, sizeof(WCHAR) * nameLen );
	os.write( &symbols[1], sizeof(WCHAR) );
	os.write( LRCF, sizeof(LRCF) );

	return true;
}

bool Config::__writeString(OutputStream &os, LPCWSTR value)
{
	bool ret = true;
	LPCWSTR p = value;
	while ( *p )
	{
		if ( *p == L'\"' )
		{
			ret = os.write( L"&quot;", sizeof(WCHAR) * 6 );
		}
		else if ( *p == L'\'' )
		{
			ret = os.write( L"&apos;", sizeof(WCHAR) * 6 );
		}
		else if ( *p == L'<' )
		{
			ret = os.write( L"&lt;", sizeof(WCHAR) * 4 );
		}
		else if ( *p == L'>' )
		{
			ret = os.write( L"&gt;", sizeof(WCHAR) * 4 );
		}
		else if ( *p == L'&')
		{
			ret = os.write( L"&amp;", sizeof(WCHAR) * 5 );
		}
		else
		{
			ret = os.write( p, sizeof(WCHAR) );
		}

		if ( !ret )
			return ret;

		++p;
	}
	return true;
}

bool Config::writeTo(OutputStream &os)
{
	unsigned char bom[2] = { 0xFF, 0xFE }; 
	WCHAR xmlhead[] = L"<?xml version=\"1.0\" encoding=\"UTF-16\"?>";
	WCHAR LRCF[] = { L'\r', L'\n' };

	os.write( bom, sizeof(bom) );
	os.write( xmlhead, sizeof(xmlhead) - sizeof(WCHAR) );
	os.write( LRCF, sizeof(LRCF) );

	for ( unsigned int i = 0; i < m_root->getChildCount(); ++i )
	{
		Setting *child = m_root->getChild( i );
		__writeSetting( os, child );
	}

	os.flush();
	return true;
}

bool Config::readFrom(InputStream &is, ExpansileString* pEStrs, ULONG nEStrs )
{
	clear();
	XmlCfgReader aReader;
	return aReader.process( is, this->getRoot(), pEStrs, nEStrs );
}

// -------------------------------------------------------------------------
}	// End of namespace xmlcfg
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
//	$Log: $
