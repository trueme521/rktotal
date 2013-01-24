/* -------------------------------------------------------------------------
//	文件名		：	xmlcfg.h
//	创建者		：	Zhuyie
//	创建时间	：	2007-11-6 20:48:58
//	功能描述	：	
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __XMLCFG_H__
#define __XMLCFG_H__

#ifdef _USER_MODE_

#include <new>				// For placement new

#else

typedef const WCHAR *LPCWSTR;
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

#endif	// _USER_MODE_

#include "expansile_string.h"

// -------------------------------------------------------------------------
namespace xmlcfg {
// -------------------------------------------------------------------------


typedef void* (*FnAllocMemory)(unsigned int);
typedef void  (*FnFreeMemory)(void*);
typedef void  (*FnCopyMemory)(void*, const void*, unsigned int);
typedef int   (*FnCompareMemory)(const void*, const void*, unsigned int);
typedef void  (*FnSetMemory)(void*, int, unsigned int);
typedef LPCWSTR (*FnDupString)(LPCWSTR, int&);
typedef void  (*FnFreeString)(LPCWSTR);
typedef int   (*FnCompareString)(LPCWSTR, LPCWSTR, int);
typedef LPCWSTR   (*FnCatString)(LPCWSTR, LPCWSTR, int);

typedef struct
{
	FnAllocMemory pfnAllocMemory;
	FnFreeMemory  pfnFreeMemory;
	FnCopyMemory  pfnCopyMemory;
	FnCompareMemory pfnCompareMemory;
	FnSetMemory   pnfSetMemory;

	FnDupString   pfnDupString;
	FnFreeString  pfnFreeString;
	FnCompareString pfnCompareString;
	FnCatString		pfnCatString;
} MemMgr, *PMemMgr;

struct StdMemMgr
{
	static void* allocMemory(unsigned int cb);
	static void freeMemory(void* ptr);
	static void copyMemory(void* dest, const void* src, unsigned int cb);
	static int compareMemory(const void* buf1, const void* buf2, unsigned int count);
	static void setMemory(void* dest, int c, unsigned int count);

	static LPCWSTR dupString(LPCWSTR str, int &lenOfStr);
	static void freeString(LPCWSTR str);
	static int compareString(LPCWSTR str1, LPCWSTR str2, int len = -1);
	static LPCWSTR catString(LPCWSTR str1, LPCWSTR str2, int str2Len = -1);

	static PMemMgr getInstance();
};

// -------------------------------------------------------------------------

class OutputStream
{
public:
	virtual bool write(const void* buffer, unsigned int cb) = 0;
	virtual bool flush() = 0;
};

class InputStream
{
public:
	virtual bool read(void* buffer, unsigned int cb, unsigned int *readedBytes) = 0;
};

// -------------------------------------------------------------------------

class Attribute
{
	PMemMgr m_mm;
	LPCWSTR m_name;	
	struct {
		LPCWSTR m_buffer;
		unsigned int m_length;
	} m_string;	

	friend class Setting;

	Attribute(PMemMgr mm, LPCWSTR name, int lenOfName = -1)
	{
		m_mm = mm;
		m_name = (*(m_mm->pfnDupString))( name, lenOfName );		
		m_string.m_buffer = NULL;
		m_string.m_length = 0;
	}
	~Attribute()
	{
		if ( m_name )
			(*(m_mm->pfnFreeString))( m_name );
		if ( m_string.m_buffer )
			(*(m_mm->pfnFreeString))( m_string.m_buffer );
	}

	Attribute(const Attribute &rhs);
	Attribute& operator=(const Attribute &rhs);

public:
	LPCWSTR getName() const 
	{
		return m_name;
	}

	bool getString(LPCWSTR &value, unsigned int *length = 0) const 
	{
		value = m_string.m_buffer;
		if ( length )
			*length = m_string.m_length;
		return true;
	}
	bool putString(LPCWSTR value, int lenOfValue = -1)
	{
		if ( value )
		{
			(*(m_mm->pfnFreeString))( m_string.m_buffer );
			m_string.m_length = 0;
			m_string.m_buffer = (*(m_mm->pfnDupString))( value, lenOfValue );
			if ( m_string.m_buffer )
			{
				m_string.m_length = lenOfValue;
				return true;
			}
		}
		return false;
	}
};

// -------------------------------------------------------------------------

class Setting
{
	PMemMgr m_mm;
	LPCWSTR m_name;
	LPCWSTR m_data;

	unsigned int m_attrCount;
	unsigned int m_attrCapacity;
	Attribute**  m_attrItems;
	unsigned int m_childCount;
	unsigned int m_childCapacity;
	Setting**    m_childItems;

	friend class Config;

	Setting(PMemMgr mm, LPCWSTR name, int lenOfName = -1)
	{
		m_mm = mm;
		m_name = (*(m_mm->pfnDupString))( name, lenOfName );
		m_attrCount = m_attrCapacity = 0;
		m_attrItems = NULL;
		m_childCount = m_childCapacity = 0;
		m_childItems = NULL;
		m_data = NULL;
	}
	~Setting()
	{
		if ( m_name )
			(*(m_mm->pfnFreeString))( m_name );

		if ( m_data )
			(*(m_mm->pfnFreeString))( m_data );

		if ( m_attrItems )
		{
			for ( unsigned int i = 0; i < m_attrCount; ++i )
			{
				m_attrItems[i]->~Attribute();
				(*(m_mm->pfnFreeMemory))( m_attrItems[i] );
			}
			(*(m_mm->pfnFreeMemory))( m_attrItems );
		}

		if ( m_childItems )
		{
			for ( unsigned int i = 0; i < m_childCount; ++i )
			{
				m_childItems[i]->~Setting();
				(*(m_mm->pfnFreeMemory))( m_childItems[i] );
			}
			(*(m_mm->pfnFreeMemory))( m_childItems );
		}
	}

	Setting(const Setting &rhs);
	Setting& operator=(const Setting &rhs);

public:
	LPCWSTR getName() const 
	{
		return m_name;
	}

	unsigned int getAttributeCount() const 
	{
		return m_attrCount;
	}
	Attribute* getAttribute(unsigned int idx) const 
	{
		return (idx < m_attrCount) ? m_attrItems[idx] : NULL;
	}
	Attribute* lookupAttribute(LPCWSTR name, int lenOfName = -1) const
	{
		for ( unsigned int i = 0; i < m_attrCount; ++i )
		{
			if ( (*(m_mm->pfnCompareString))(m_attrItems[i]->getName(), name, lenOfName) == 0 )
				return m_attrItems[i];
		}
		return NULL;
	}
	Attribute* createAttribute(LPCWSTR name, int lenOfName = -1)
	{
		if ( lookupAttribute( name, lenOfName ) )
			return NULL;

		if ( m_attrCount == m_attrCapacity )
		{
			unsigned int newCapacity = m_attrCapacity + 4;
			void* newBuffer = (*(m_mm->pfnAllocMemory))(sizeof(Attribute*) * newCapacity);
			if ( !newBuffer )
				return NULL;
			(*(m_mm->pfnCopyMemory))(newBuffer, m_attrItems, sizeof(Attribute*) * m_attrCount);
			(*(m_mm->pfnFreeMemory))(m_attrItems);
			m_attrCapacity = newCapacity;
			m_attrItems = (Attribute**)newBuffer;
		}	

		void* newBuffer = (*(m_mm->pfnAllocMemory))(sizeof(Attribute));
		if ( !newBuffer )
			return NULL;
		Attribute *newAttr = new(newBuffer) Attribute(m_mm, name, lenOfName);
		m_attrItems[m_attrCount] = newAttr;
		++m_attrCount;
		return newAttr;
	}
	bool deleteAttribute(Attribute *attr)
	{
		for ( unsigned int i = 0; i < m_attrCount; ++i )
		{
			if ( m_attrItems[i] == attr )
			{
				attr->~Attribute();
				(*(m_mm->pfnFreeMemory))(attr);
				for ( unsigned int j = i + 1; j < m_attrCount; ++j )
					m_attrItems[j - 1] = m_attrItems[j];
				--m_attrCount;
				return true;
			}
		}
		return false;
	}
	void deleteAllAttributes()
	{
		if ( m_attrItems )
		{
			for ( unsigned int i = 0; i < m_attrCount; ++i )
			{
				m_attrItems[i]->~Attribute();
				(*(m_mm->pfnFreeMemory))( m_attrItems[i] );				
			}	

			m_attrCount = 0;
		}
	}

	unsigned int getChildCount() const 
	{
		return m_childCount;
	}
	Setting* getChild(unsigned int idx) const 
	{
		return (idx < m_childCount) ? m_childItems[idx] : NULL;
	}
	Setting* lookupChild(LPCWSTR name, int lenOfName = -1) const
	{
		for ( unsigned int i = 0; i < m_childCount; ++i )
		{
			if ( (*(m_mm->pfnCompareString))(m_childItems[i]->getName(), name, lenOfName) == 0 )
				return m_childItems[i];
		}
		return NULL;
	}
	Setting* createChild(LPCWSTR name, int lenOfName = -1)
	{
		if ( m_childCount == m_childCapacity )
		{
			unsigned int newCapacity = m_childCapacity + 4;
			void* newBuffer = (*(m_mm->pfnAllocMemory))(sizeof(Setting*) * newCapacity);
			if ( !newBuffer )
				return NULL;
			if ( m_childItems )
			{
				(*(m_mm->pfnCopyMemory))(newBuffer, m_childItems, sizeof(Setting*) * m_childCount);
				(*(m_mm->pfnFreeMemory))(m_childItems);
			}
			m_childCapacity = newCapacity;
			m_childItems = (Setting**)newBuffer;
		}

		void* newBuffer = (*(m_mm->pfnAllocMemory))(sizeof(Setting));
		if ( !newBuffer )
			return NULL;
		Setting *newSetting = new(newBuffer) Setting(m_mm, name, lenOfName);
		m_childItems[m_childCount] = newSetting;
		++m_childCount;
		return newSetting;
	}
	bool deleteChild(Setting *child)
	{
		for ( unsigned int i = 0; i < m_childCount; ++i )
		{
			if ( m_childItems[i] == child )
			{
				child->~Setting();
				(*(m_mm->pfnFreeMemory))(child);
				for ( unsigned int j = i + 1; j < m_childCount; ++j )
					m_childItems[j - 1] = m_childItems[j];
				--m_childCount;
				return true;
			}
		}
		return false;
	}
	void deleteAllChildren()
	{
		if ( m_childItems )
		{
			for ( unsigned int i = 0; i < m_childCount; ++i )
			{
				m_childItems[i]->~Setting();
				(*(m_mm->pfnFreeMemory))( m_childItems[i] );
			}

			m_childCount = 0;
		}
	}

	LPCWSTR getData() const
	{
		return m_data;
	}

	void setData(LPCWSTR data, int lenOfData = -1)
	{
		if (!m_data)
		{
			m_data = (*(m_mm->pfnDupString))( data, lenOfData );
		}
		else
		{
			LPCWSTR oldData = m_data;

			m_data = (*(m_mm->pfnCatString))(oldData, data, lenOfData);

			(*(m_mm->pfnFreeString))(oldData);
		}
	}
};

// -------------------------------------------------------------------------

class Config
{
	PMemMgr m_mm;
	Setting *m_root;

public:
	Config(PMemMgr mm = StdMemMgr::getInstance());
	~Config();

	Setting* getRoot() const;
	Setting* lookupSetting(LPCWSTR path) const;
	Attribute* lookupAttribute(LPCWSTR path) const;
	
	void clear();

	bool readFrom(InputStream &is, ExpansileString* pEStrs = NULL, ULONG nEStrs = 0);
	bool writeTo(OutputStream &os);

private:
	bool __writeSetting(OutputStream &os, Setting *setting);
	bool __writeString(OutputStream &os, LPCWSTR value);
};

inline
xmlcfg::Attribute* cfgCreateAttribute( xmlcfg::Setting* pNode, LPCWSTR szName, LPCWSTR szValue )
{
	xmlcfg::Attribute* pAttribute = 
		pNode->createAttribute( szName );

	if ( pAttribute != NULL )
	{
		if ( !pAttribute->putString( szValue ) )
		{
			pNode->deleteAttribute( pAttribute );
			pAttribute = NULL;
		}
	}

	return pAttribute;
}

inline
void cfgDeleteAttribute( xmlcfg::Setting* pNode, LPCWSTR szName )
{
	xmlcfg::Attribute* pAttribute =
		pNode->lookupAttribute( szName );

	if ( pAttribute != NULL )
		pNode->deleteAttribute( pAttribute );
}

inline
bool cfgGetAttribute( xmlcfg::Setting* pNode, LPCWSTR szName, LPCWSTR &szValue )
{
	bool bRet = false;

	xmlcfg::Attribute* pAttribute =
		pNode->lookupAttribute( szName );

	if ( pAttribute != NULL )
	{
		if ( pAttribute->getString( szValue ) && szValue != NULL )
		{
			bRet = true;
		}
	}

	return bRet;
}

inline
bool cfgSetAttribute( xmlcfg::Setting* pNode, LPCWSTR szName, LPCWSTR szValue )
{
	bool bRet = false;

	xmlcfg::Attribute* pAttribute = 
		pNode->lookupAttribute( szName );

	if ( pAttribute != NULL )
	{
		if ( pAttribute->putString( szValue ) )
		{
			bRet = true;
		}
	}

	return bRet;
}

// -------------------------------------------------------------------------
}	// End of namespace xmlcfg
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
//	$Log: $

#endif /* __XMLCFG_H__ */
