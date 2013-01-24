//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   TiXMLHandle.h
//  Version     :   1.0
//  Creator     :   YangXD(yxdh)
//  Date        :   2006-3-31 14:51:33
//  Comment     :   Interface for the TiXMLHandle class.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _TIXML_HANDLE_H_
#define _TIXML_HANDLE_H_

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include "tinyxml.h"


/**
*	A TiXmlHandleEx is a class that extends TiXmlHandle for wrapping TinyXml;
*/
class TiXmlHandleEx
{
public:
	TiXmlHandleEx() : node(NULL)					
	{ 
	}

	/// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
	TiXmlHandleEx( TiXmlNode* _node )					
	{ 
		this->node = _node; 
	}

	/// Copy constructor
	TiXmlHandleEx( const TiXmlHandleEx& ref )			
	{ 
		this->node = ref.node; 
	}
	TiXmlHandleEx operator=( const TiXmlHandleEx& ref ) 
	{ 
		this->node = ref.node; 
		return *this; 
	}

	/// Return a handle to the first child node.
	TiXmlHandleEx FirstChild() const;
	/// Return a handle to the first child node with the given name.
	TiXmlHandleEx FirstChild(const char* value) const;
	/// Return a handle to the first child node with the given name.
	TiXmlHandleEx FirstChild(const wchar_t* value) const;
	/// Return a handle to the first child element.
	TiXmlHandleEx FirstChildElement() const;
	/// Return a handle to the first child element with the given name.
	TiXmlHandleEx FirstChildElement(const char* value) const;
	/// Return a handle to the first child element with the given name.
	TiXmlHandleEx FirstChildElement(const wchar_t* value) const;

	/** Return a handle to the "index" child with the given name. 
	The first child is 0, the second 1, etc.
	*/
	TiXmlHandleEx Child(const char* value, int index) const;
	/** Return a handle to the "index" child with the given name. 
	The first child is 0, the second 1, etc.
	*/
	TiXmlHandleEx Child(const wchar_t* value, int index) const;
	/** Return a handle to the "index" child. 
	The first child is 0, the second 1, etc.
	*/
	TiXmlHandleEx Child( int index ) const;
	/** Return a handle to the "index" child element with the given name. 
	The first child element is 0, the second 1, etc. Note that only TiXmlElements
	are indexed: other types are not counted.
	*/
	TiXmlHandleEx ChildElement(const char* value, int index) const;
	/** Return a handle to the "index" child element with the given name. 
	The first child element is 0, the second 1, etc. Note that only TiXmlElements
	are indexed: other types are not counted.
	*/
	TiXmlHandleEx ChildElement(const wchar_t* value, int index) const;
	/** Return a handle to the "index" child element. 
	The first child element is 0, the second 1, etc. Note that only TiXmlElements
	are indexed: other types are not counted.
	*/
	TiXmlHandleEx ChildElement( int index ) const;

#ifdef TIXML_USE_STL
	TiXmlHandleEx FirstChild(const std::string& _value) const
	{
		return FirstChild( _value.c_str() ); 
	}
	TiXmlHandleEx FirstChild(const std::wstring& _value) const
	{
		return FirstChild( _value.c_str() ); 
	}

	TiXmlHandleEx FirstChildElement(const std::string& _value) const		
	{ 
		return FirstChildElement( _value.c_str() ); 
	}
	TiXmlHandleEx FirstChildElement(const std::wstring& _value) const		
	{ 
		return FirstChildElement( _value.c_str() ); 
	}


	TiXmlHandleEx Child(const std::string& _value, int index) const		
	{ 
		return Child( _value.c_str(), index ); 
	}
	TiXmlHandleEx Child(const std::wstring& _value, int index) const		
	{ 
		return Child( _value.c_str(), index ); 
	}

	TiXmlHandleEx ChildElement(const std::string& _value, int index) const	
	{ 
		return ChildElement( _value.c_str(), index ); 
	}
	TiXmlHandleEx ChildElement(const std::wstring& _value, int index) const	
	{ 
		return ChildElement( _value.c_str(), index ); 
	}

#endif

	/// Return the handle as a TiXmlNode. This may return null.
	TiXmlNode* Node() const			
	{ 
		return node; 
	} 
	/// Return the handle as a TiXmlElement. This may return null.
	TiXmlElement* Element() const	
	{ 
		return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); 
	}
	/// Return the handle as a TiXmlText. This may return null.
	TiXmlText* Text() const			
	{ 
		return ( ( node && node->ToText() ) ? node->ToText() : 0 ); 
	}
	/// Return the handle as a TiXmlUnknown. This may return null;
	TiXmlUnknown* Unknown() const			
	{ 
		return ( ( node && node->ToUnknown() ) ? node->ToUnknown() : 0 ); 
	}


	TiXmlHandleEx NextElement() const;
	TiXmlHandleEx ParentElement() const;

private:
	TiXmlNode* node;
};

#endif	// #ifndef _TIXML_HANDLE_H_

