//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   TiXMLHandle.cpp
//  Version     :   1.0
//  Creator     :   YangXD(yxdh)
//  Date        :   2006-3-31 14:51:30
//  Comment     :   Implementation of the TiXMLHandle class.
//
//////////////////////////////////////////////////////////////////////////////////////
#include "TiXMLHandle.h"
#include "KAVPublic.h"
#include "KConversion.h"

using namespace KXML;


///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::FirstChild
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the first child node.
// Parameters:
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::FirstChild() const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild();
		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::FirstChild
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the first child node with the given name.
// Parameters:
//		const char* value: The given name.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::FirstChild(const char* value) const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild( value );

		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::FirstChild
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the first child node with the given name.
// Parameters:
//		const wchar_t* value: The given name.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::FirstChild(const wchar_t* value) const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild( CW2A(value) );

		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::FirstChildElement
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the first child element.
// Parameters:
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::FirstChildElement() const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement();
		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::FirstChildElement
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the first child element with the given name.
// Parameters:
//		const char* value: The given name.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::FirstChildElement(const char* value) const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement( value );

		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::FirstChildElement
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the first child element with the given name.
// Parameters:
//		const wchar_t* value: The given name.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::FirstChildElement(const wchar_t* value) const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement( CW2A(value) );

		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::Child
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the "index" child with the given name. 
//		The first child is 0, the second 1, etc.
// Parameters:
//		const char* value: The given name.
//		int index: the index.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::Child(const char* value, int index) const
{
	if ( node )
	{
		int i;

		TiXmlNode* child = node->FirstChild( value );
		for (i = 0;
			 child && i < index;
			 child = child->NextSibling( value ), ++i)
		{
			// nothing
		}
		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::Child
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the "index" child with the given name. 
//		The first child is 0, the second 1, etc.
// Parameters:
//		const wchar_t* value: The given name.
//		int index: the index.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::Child(const wchar_t* value, int index) const
{
	if ( node )
	{
		int i;

		TiXmlNode* child = node->FirstChild( CW2A(value) );
		for (i = 0;
			 child && i < index;
			 child = child->NextSibling( CW2A(value) ), ++i)
		{
			// nothing
		}
		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::Child
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the "index" child. 
//		The first child is 0, the second 1, etc.
// Parameters:
//		int index: the index.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::Child( int index ) const
{
	if ( node )
	{
		int i;
		TiXmlNode* child = node->FirstChild();
		for (	i=0;
			child && i<index;
			child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::ChildElement
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the "index" child element with the given name. 
//		The first child element is 0, the second 1, etc. Note that only 
//		TiXmlElements are indexed: other types are not counted.
// Parameters:
//		const char* value: The given name.
//		int index: the index.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::ChildElement(const char* value, int index) const
{
	if ( node )
	{
		int i;

		TiXmlElement* child = node->FirstChildElement( value );
	
		for (i = 0;
			 child && i < index;
			 child = child->NextSiblingElement( value ), ++i)
		{
			// nothing
		}
		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::ChildElement
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the "index" child element with the given name. 
//		The first child element is 0, the second 1, etc. Note that only 
//		TiXmlElements are indexed: other types are not counted.
// Parameters:
//		const wchar_t* value: The given name.
//		int index: the index.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::ChildElement(const wchar_t* value, int index) const
{
	if ( node )
	{
		int i;

		TiXmlElement* child = node->FirstChildElement( CW2A(value) );
		for (i = 0;
			 child && i < index;
			 child = child->NextSiblingElement( CW2A(value) ), ++i)
		{
			// nothing
		}
		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::ChildElement
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the "index" child element. 
//		The first child element is 0, the second 1, etc. Note that only 
//		TiXmlElements are indexed: other types are not counted.
// Parameters:
//		int index: the index.
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::ChildElement( int index ) const
{
	if ( node )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement();
		for (	i=0;
			child && i<index;
			child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandleEx( child );
	}
	return TiXmlHandleEx( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::NextElement
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the next element. 
// Parameters:
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::NextElement() const
{
	if (node)
	{
		return TiXmlHandleEx(node->NextSiblingElement());	
	}

	return TiXmlHandleEx(0);
}

///////////////////////////////////////////////////////////////////////////////
// TiXmlHandleEx::ParentElement
///////////////////////////////////////////////////////////////////////////////
// Description:
//		Return a handle to the parent element. 
// Parameters:
// Return:
//		TiXmlHandleEx: The handle.
///////////////////////////////////////////////////////////////////////////////
TiXmlHandleEx TiXmlHandleEx::ParentElement() const
{
	if (node)
	{
		return TiXmlHandleEx(node->Parent()->ToElement());	
	}

	return TiXmlHandleEx(0);
}
