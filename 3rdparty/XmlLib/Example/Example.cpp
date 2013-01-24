// Example.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"

#include "KXMLManager.h"


#ifndef	_INC_TCHAR
	#if !defined(_UNICODE) && !defined(UNICODE)
	#define _tprintf(x)		printf(_T("%s\n"), x)
	#else
	#define _tprintf(x)		wprintf(_T("%ls\n"), x)
	#endif
#endif


using namespace KXML;


const KXML::KTChar* XmlHeader = _T("<?xml version=\"1.0\" encoding=\"utf-8\" ?><queryOutput />");

KXML::KTString Simres(_T("simpleResult"));
KXML::KTString SimresText(_T("Result"));

KXML::KTString comres(_T("complexResult"));

KXML::KTString patch(_T("patch"));
KXML::KTString item(_T("item"));
KXML::KTString itemAttr(_T("type"));
KXML::KTString itemAttrValue1(_T("add"));
KXML::KTString itemAttrValue2(_T("need"));
KXML::KTString value(_T("value"));
KXML::KTString valueText1(_T("patch1"));
KXML::KTString valueText2(_T("patch2"));
KXML::KTString valueText3(_T("patch3"));
KXML::KTString valueText4(_T("patch4"));

#if !defined(_UNICODE) && !defined(UNICODE)
KXML::KTString policy(_T("policy´ó»°"));
#else
KXML::KTString policy(_T("policy\x5927\x8BDD"));
#endif
KXML::KTString itemAttrValue3(_T("enabled"));
KXML::KTString valueText5(_T("file"));
KXML::KTString valueText6(_T("email"));
KXML::KTString itemAttrValue4(_T("disabled"));
KXML::KTString valueText7(_T("register"));

KXML::KTString virus(_T("virus"));
KXML::KTString itemAttr1(_T("name"));
KXML::KTString itemAttrValue5(_T("virus1"));
KXML::KTString itemAttr2(_T("time"));
KXML::KTString itemAttrValue6(_T("date time1"));
KXML::KTString itemAttrValue9(_T("date time1"));
KXML::KTString itemAttr3(_T("type"));
KXML::KTString itemAttrValue7(_T("memory"));
KXML::KTString itemAttrValue10(_T("file"));
KXML::KTString itemAttr4(_T("result"));
KXML::KTString itemAttrValue8(_T("c"));
KXML::KTString itemAttrValue11(_T("i"));


void UsingXML(KXMLManager& Tmp);

int main(int argc, char* argv[])
{
#ifdef WIN32
	setlocale(LC_ALL, "");
#else
	setlocale(LC_ALL, "zh_CN");
#endif

	try
	{
		KXMLManager Tmp;

		UsingXML(Tmp);
	}
	catch (...) 
	{
		printf("\ncatch some error!");
		return -1;
	}
}

void UsingXML(KXMLManager& Tmp)
{
	// root	
	Tmp.LoadXml(XmlHeader);

	Tmp.GetRootNode();

	Tmp.SetAttribute(_T("operation"), _T("AVQuery:Product-Name\x5927\x8BDD"));

	// simpleResult
	KXMLNode* simpleResultNode = NULL;
	Tmp.AppendNode(Simres, &simpleResultNode);
	Tmp.SetText(SimresText, simpleResultNode);

	// complexResult
	KXMLNode* complexResultNode = NULL;
	Tmp.AppendNode(comres, &complexResultNode);

	// patch
	KXMLNode* patchNode = NULL;
	Tmp.AppendNode(patch, &patchNode, complexResultNode);

	// item
	KXMLNode* pItemNode = NULL;
	Tmp.AppendNode(item, &pItemNode, patchNode);
	Tmp.SetAttribute(itemAttr, itemAttrValue1, pItemNode);

	// value
	KXMLNode* pValueNode = NULL;
	Tmp.AppendNode(value, &pValueNode, pItemNode);
	Tmp.SetText(valueText1, pValueNode);
	Tmp.AppendNode(value, &pValueNode, pItemNode);
	Tmp.SetText(valueText2, pValueNode);

	// item
	Tmp.AppendNode(item, &pItemNode, patchNode);
	Tmp.SetAttribute(itemAttr, itemAttrValue2, pItemNode);

	// value
	Tmp.AppendNode(value, &pValueNode, pItemNode);
	Tmp.SetText(valueText3, pValueNode);
	Tmp.AppendNode(value, &pValueNode, pItemNode);
	Tmp.SetText(valueText4, pValueNode);


	// policy
	KXMLNode* policyNode = NULL;
	Tmp.AppendNode(policy, &policyNode, complexResultNode);

	// item
	Tmp.AppendNode(item, &pItemNode, policyNode);
	Tmp.SetAttribute(itemAttr, itemAttrValue3, pItemNode);

	// value
	Tmp.AppendNode(value, &pValueNode, pItemNode);
	Tmp.SetText(valueText5, pValueNode);
	Tmp.AppendNode(value, &pValueNode, pItemNode);
	Tmp.SetText(valueText6, pValueNode);

	// item
	Tmp.AppendNode(item, &pItemNode, policyNode);
	Tmp.SetAttribute(itemAttr, itemAttrValue4, pItemNode);

	// value
	Tmp.AppendNode(value, &pValueNode, pItemNode);
	Tmp.SetText(valueText7, pValueNode);
	Tmp.SetText(valueText7, pValueNode);


	// virus
	KXMLNode* VirusNode = NULL;
	Tmp.AppendNode(virus, &VirusNode, complexResultNode);

	// item
	Tmp.AppendNode(item, &pItemNode, VirusNode);
	Tmp.SetAttribute(itemAttr1, itemAttrValue5, pItemNode);
	Tmp.SetAttribute(itemAttr2, itemAttrValue6, pItemNode);
	Tmp.SetAttribute(itemAttr3, itemAttrValue7, pItemNode);
	Tmp.SetAttribute(itemAttr4, itemAttrValue8, pItemNode);

	Tmp.AppendNode(item, &pItemNode, VirusNode);
	Tmp.SetAttribute(itemAttr1, itemAttrValue5, pItemNode);
	Tmp.SetAttribute(itemAttr2, itemAttrValue9, pItemNode);
	Tmp.SetAttribute(itemAttr3, itemAttrValue10, pItemNode);
	Tmp.SetAttribute(itemAttr4, itemAttrValue11, pItemNode);


	KXML::KTString root;
	Tmp.ToXMLString(&root);

	Tmp.SaveFile(_T("Config.xml"));

	_tprintf(root.c_str());

	//getchar();


	// remove root node
	_tprintf(_T("\n\nremove root node:\n"));
	Tmp.RemoveNode(Tmp.FirstChildElement().Node());
	Tmp.ToXMLString(&root);
	_tprintf(root.c_str());

	// Read
	_tprintf(_T("\n\nRead:\n"));
	
	if (!Tmp.LoadFile(_T("Config.xml")))
		_tprintf(_T("\n\nLoad file failed.\n"));

	Tmp.GetAttributeValue(Tmp.FirstChildElement().Node(), _T("operation"), &root);
	_tprintf(root.c_str());

	Tmp.GetNodeText(Tmp.FirstChildElement()
					.FirstChildElement(_T("complexResult"))
					.FirstChildElement(_T("patch"))
					.ChildElement(1)
					.ChildElement(1)
					.Node(),
					&root				
	);

	_tprintf(root.c_str());

	Tmp.GetAttributeValue(Tmp.FirstChildElement()
						.FirstChildElement(_T("complexResult"))
						.FirstChildElement(_T("virus"))
						.ChildElement(0).Node(),
						_T("type"),
						&root);
	_tprintf(root.c_str());

	Tmp.GetAttributeValue(Tmp.FirstChildElement()
		.FirstChildElement()
		.NextElement()
		.FirstChildElement(_T("virus"))
		.ChildElement(0).Node(),
		_T("type"),
		&root);
	_tprintf(root.c_str());


	_tprintf(_T("\n\nRead a attribute not exist:\n"));
	Tmp.GetAttributeValue(Tmp.FirstChildElement()
		.FirstChildElement()
		.NextElement()
		.FirstChildElement(_T("virus"))
		.ChildElement(0).Node(),
		_T("NotExist"),
		&root);
	_tprintf(root.c_str());

	getchar();
	//// Set
	root = _T("Driver");
	Tmp.SetAttribute(
		_T("type"), 
		root, 
		Tmp.FirstChildElement()
		.FirstChildElement(_T("complexResult"))
		.FirstChildElement(_T("virus"))
		.ChildElement(1).Node()
	);

	//// remove
	Tmp.RemoveAttribute(
		Tmp.FirstChildElement()
		.FirstChildElement(_T("complexResult"))
		.FirstChildElement(_T("virus"))
		.ChildElement(0).Node(),
		_T("time")
	);	

	_tprintf(_T("\nremove Attribute"));
	Tmp.SaveFile(_T("Config.xml"));

	getchar();

	Tmp.RemoveAttribute(
		Tmp.FirstChildElement()
		.FirstChildElement(_T("complexResult"))
		.FirstChildElement(_T("virus"))
		.ChildElement(0).Node(),
		_T("time")
		);	

	_tprintf(_T("\nremove Attribute again"));
	Tmp.SaveFile(_T("Config.xml"));

	getchar();

	Tmp.RemoveChildNodes(
		Tmp.FirstChildElement()
		.FirstChildElement(_T("complexResult"))
		.FirstChildElement(_T("policy")).Node()
	);

	_tprintf(_T("\nremove ChildNodes"));
	Tmp.SaveFile(_T("Config.xml"));

	getchar();

	Tmp.RemoveChildNodes(
		Tmp.FirstChildElement()
		.FirstChildElement(_T("complexResult"))
		.FirstChildElement(_T("policy")).Node()
		);

	_tprintf(_T("\nremove ChildNodes again"));
	Tmp.SaveFile(_T("Config.xml"));

	getchar();

	Tmp.RemoveNode(
		Tmp.FirstChildElement()
		.FirstChildElement(_T("complexResult"))
		.FirstChildElement(_T("policy")).Node()
		);

	_tprintf(_T("\nremove Node"));
	Tmp.SaveFile(_T("Config.xml"));

	getchar();

	Tmp.RemoveNode(
		Tmp.FirstChildElement()
		.FirstChildElement(_T("complexResult"))
		.FirstChildElement(_T("patch"))
		.ChildElement(0).Node()
	);

	_tprintf(_T("\nremove Node"));
	Tmp.SaveFile(_T("Config.xml"));

	getchar();



	getchar();
}
