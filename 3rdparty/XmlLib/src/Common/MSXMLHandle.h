//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   MSXMLHandle.h
//  Version     :   1.0
//  Creator     :   YangXD(yxdh)
//  Date        :   2005-8-5 16:25:57
//  Comment     :   Interface for the MSXMLHandle class.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _MSXML_HANDLE_H_
#define _MSXML_HANDLE_H_


#ifdef WIN32

#include "KType.h"

#import "msxml.dll" named_guids raw_interfaces_only


/**
	A MSXmlHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that MSXmlHandle is not part of the MsXml
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<Element attributeA = "valueA">
			<Child attributeB = "value1" />
			<Child attributeB = "value2" />
		</Element>
	<Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very 
	easy to write a lot of code that looks like:

	@verbatim
	MSXML::IXMLDOMElement* root = document.FirstChildElement( "Document" );
	if ( root )
	{
		MSXML::IXMLDOMElement* element = root->FirstChildElement( "Element" );
		if ( element )
		{
			MSXML::IXMLDOMElement* child = element->FirstChildElement( "Child" );
			if ( child )
			{
				MSXML::IXMLDOMElement* child2 = child->NextSiblingElement( "Child" );
				if ( child2 )
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. MSXmlHandle addresses the verbosity
	of such code. A MSXmlHandle checks for null	pointers so it is perfectly safe 
	and correct to use:

	@verbatim
	MSXmlHandle docHandle( &document );
	MSXML::IXMLDOMElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).Element();
	if ( child2 )
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	MSXmlHandle handleCopy = handle;
	@endverbatim

	What they should not be used for is iteration:

	@verbatim
	int i=0; 
	while ( true )
	{
		MSXML::IXMLDOMElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).Element();
		if ( !child )
			break;
		// do something
		++i;
	}
	@endverbatim

	It seems reasonable, but it is in fact two embedded while loops. The Child method is 
	a linear walk to find the element, so this code would iterate much more than it needs 
	to. Instead, prefer:

	@verbatim
	MSXML::IXMLDOMElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).Element();

	for( child; child; child=child->NextSiblingElement() )
	{
		// do something
	}
	@endverbatim
*/
class MSXmlHandle
{
public:
	MSXmlHandle() : node(NULL)			
	{ 
	}

	/// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
	MSXmlHandle( MSXML::IXMLDOMNode* node )			
	{ 
		this->node = node; 
	}
	
	/// Copy constructor
	MSXmlHandle( const MSXmlHandle& ref )			
	{ 
		this->node = ref.node; 
	}
	MSXmlHandle operator=( const MSXmlHandle& ref ) 
	{ 
		this->node = ref.node; 
		return *this; 
	}

	/// Return a handle to the first child node.
	MSXmlHandle FirstChild() const;
	/// Return a handle to the first child node with the given name.
	MSXmlHandle FirstChild( const char* value ) const;
	/// Return a handle to the first child node with the given name.
	MSXmlHandle FirstChild( const wchar_t* value ) const;
	/// Return a handle to the first child element.
	MSXmlHandle FirstChildElement() const;
	/// Return a handle to the first child element with the given name.
	MSXmlHandle FirstChildElement( const char* value ) const;
	/// Return a handle to the first child element with the given name.
	MSXmlHandle FirstChildElement( const wchar_t* value ) const;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	MSXmlHandle Child( const char* value, int index ) const;
	/** Return a handle to the "index" child with the given name. 
	The first child is 0, the second 1, etc.
	*/
	MSXmlHandle Child( const wchar_t* value, int index ) const;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	MSXmlHandle Child( int index ) const;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only IXMLDOMElement
		are indexed: other types are not counted.
	*/
	MSXmlHandle ChildElement( const char* value, int index ) const;
	/** Return a handle to the "index" child element with the given name. 
	The first child element is 0, the second 1, etc. Note that only IXMLDOMElement
	are indexed: other types are not counted.
	*/
	MSXmlHandle ChildElement( const wchar_t* value, int index ) const;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only IXMLDOMElement
		are indexed: other types are not counted.
	*/
	MSXmlHandle ChildElement( int index ) const;

	MSXmlHandle FirstChild( const std::string& _value ) const				
	{
		return FirstChild( _value.c_str() ); 
	}
	MSXmlHandle FirstChild( const std::wstring& _value ) const				
	{
		return FirstChild( _value.c_str() ); 
	}

	MSXmlHandle FirstChildElement( const std::string& _value ) const		
	{ 
		return FirstChildElement( _value.c_str() ); 
	}
	MSXmlHandle FirstChildElement( const std::wstring& _value ) const		
	{ 
		return FirstChildElement( _value.c_str() ); 
	}

	MSXmlHandle Child( const std::string& _value, int index ) const			
	{ 
		return Child( _value.c_str(), index ); 
	}
	MSXmlHandle Child( const std::wstring& _value, int index ) const			
	{ 
		return Child( _value.c_str(), index ); 
	}

	MSXmlHandle ChildElement( const std::string& _value, int index ) const	
	{ 
		return ChildElement( _value.c_str(), index ); 
	}
	MSXmlHandle ChildElement( const std::wstring& _value, int index ) const	
	{ 
		return ChildElement( _value.c_str(), index ); 
	}

	MSXmlHandle NextElement() const;
	MSXmlHandle ParentElement() const;

	/// Return the handle as a IXMLDOMNode. This may return null.
	MSXML::IXMLDOMNode* Node() const			
	{ 
		return node; 
	} 
	/// Return the handle as a IXMLDOMElement. This may return null.
	const MSXML::IXMLDOMElement* Element() const;
	/// Return the handle as a IXMLDOMText. This may return null.
	const MSXML::IXMLDOMText* Text() const;

protected:
	void SetNode(MSXML::IXMLDOMNode* node)		
	{ 
		this->node = node; 
	}

private:

	MSXML::IXMLDOMNode* node;
};

#endif // #ifdef WIN32


#endif	// #ifndef _MSXML_HANDLE_H_
