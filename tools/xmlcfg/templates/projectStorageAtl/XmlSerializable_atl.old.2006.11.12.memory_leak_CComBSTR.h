// XmlSerializable.h
//

#if !defined(_XMLSERIALIZABLE_H_)
#define _XMLSERIALIZABLE_H_

#include <atlbase.h>
#include "cmString.h"
#include "msxml2.h"

class CXmlSerializable
{
public:
	virtual bool BuildFromXml(IXMLDOMNode* node) = 0;
	virtual bool AppendToDOMDocument(IXMLDOMDocument2* xmlDoc, IXMLDOMElement* parentElement) = 0;
	
	virtual bool SaveToXmlFile(const cmString& fileName);
	virtual bool LoadFromXmlFile(const cmString& fileName);

	virtual void Clear() = 0;
	virtual void SetDefaults() = 0;

	virtual cmString GetXmlNodeName();
	
protected:

	void AppendMemberNode(const cmString& nodeName, const cmString& nodeValue, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendMemberNode(const cmString& nodeName, int nodeValue, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendMemberNode(const cmString& nodeName, long nodeValue, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendMemberNode(const cmString& nodeName, double nodeValue, int precision, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendMemberNode(const cmString& nodeName, bool nodeValue, IXMLDOMElement* parentElement,IXMLDOMDocument2* xmlDoc);
	void AppendMemberNode(const cmString& nodeName, VARIANT_BOOL nodeValue, IXMLDOMElement* parentElement,IXMLDOMDocument2* xmlDoc);

	bool ExtractMemberNode(	IXMLDOMNode* parentElement, const cmString& nodeName, cmString& member);
	bool ExtractMemberNode(	IXMLDOMNode* parentElement, const cmString& nodeName, int& member);
	bool ExtractMemberNode(	IXMLDOMNode* parentElement, const cmString& nodeName, long& member);
	bool ExtractMemberNode(	IXMLDOMNode* parentElement, const cmString& nodeName, double& member);
	bool ExtractMemberNode(	IXMLDOMNode* parentElement, const cmString& nodeName, bool& member);
	bool ExtractMemberNode(	IXMLDOMNode* parentElement, const cmString& nodeName, CXmlSerializable& member);
	bool ExtractMemberNode(	IXMLDOMNode* parentElement, const cmString& nodeName, VARIANT_BOOL& member);

	void AppendCDATAMemberNode(const cmString& nodeValue, IXMLDOMElement* parentElement,IXMLDOMDocument2* xmlDoc);
	bool ExtractCDATAMemberNode(IXMLDOMNode* parentElement, cmString& member);

	void AppendAttributeNode(const cmString& nodeName, const cmString& nodeValue, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, int nodeValue, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, long nodeValue, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, double nodeValue, int precision, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, bool nodeValue, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, VARIANT_BOOL nodeValue, IXMLDOMElement* parentElement, IXMLDOMDocument2* xmlDoc);

	bool ExtractAttributeNode(IXMLDOMNode* parentElement, const cmString& nodeName, cmString& member);
	bool ExtractAttributeNode(IXMLDOMNode* parentElement, const cmString& nodeName, int& member);
	bool ExtractAttributeNode(IXMLDOMNode* parentElement, const cmString& nodeName, long& member);
	bool ExtractAttributeNode(IXMLDOMNode* parentElement, const cmString& nodeName, double& member);
	bool ExtractAttributeNode(IXMLDOMNode* parentElement, const cmString& nodeName, bool& member);
	bool ExtractAttributeNode(IXMLDOMNode* parentElement, const cmString& nodeName, VARIANT_BOOL& member);

	void AppendTextNode(cmString& value, IXMLDOMNode* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendTextNode(int member, IXMLDOMNode* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendTextNode(long member, IXMLDOMNode* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendTextNode(double member, int precision, IXMLDOMNode* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendTextNode(bool member, IXMLDOMNode* parentElement, IXMLDOMDocument2* xmlDoc);
	void AppendTextNode(VARIANT_BOOL member, IXMLDOMNode* parentElement, IXMLDOMDocument2* xmlDoc);

	bool ExtractTextNode(IXMLDOMNode* parentElement, cmString& member);
	bool ExtractTextNode(IXMLDOMNode* parentElement, int& member);
	bool ExtractTextNode(IXMLDOMNode* parentElement, long& member);
	bool ExtractTextNode(IXMLDOMNode* parentElement, double& member);
	bool ExtractTextNode(IXMLDOMNode* parentElement, bool& member);
	bool ExtractTextNode(IXMLDOMNode* parentElement, VARIANT_BOOL& member);

	IXMLDOMNode* GetChildNode(	IXMLDOMNode* parentElement, 
								const cmString& nodeName, 
								cmString& nodeValueStr);
};

#endif //!defined(_XMLSERIALIZABLE_H_)
