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
	virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node) = 0;
	virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement) = 0;
	
	virtual bool SaveToXmlFile(const cmString& fileName);
	virtual bool LoadFromXmlFile(const cmString& fileName);

	virtual void Clear() = 0;
	virtual void SetDefaults() = 0;

	virtual cmString GetXmlNodeName();
	
protected:

	void AppendMemberNode(const cmString& nodeName, const cmString& nodeValue, CComPtr<IXMLDOMElement>& parentElement,CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendMemberNode(const cmString& nodeName, int nodeValue, CComPtr<IXMLDOMElement>& parentElement,CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendMemberNode(const cmString& nodeName, long nodeValue, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendMemberNode(const cmString& nodeName, double nodeValue, int precision, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendMemberNode(const cmString& nodeName, bool nodeValue, CComPtr<IXMLDOMElement>& parentElement,CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendMemberNode(const cmString& nodeName, VARIANT_BOOL nodeValue, CComPtr<IXMLDOMElement>& parentElement,CComPtr<IXMLDOMDocument2>& xmlDoc);

void AppendAttributeNode(const cmString& nodeName, const cmString& nodeValue, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, int nodeValue, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, long nodeValue, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, double nodeValue, int precision, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, bool nodeValue, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendAttributeNode(const cmString& nodeName, VARIANT_BOOL nodeValue, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);

	void AppendCDATAMemberNode(const cmString& nodeValue, CComPtr<IXMLDOMElement>& parentElement,CComPtr<IXMLDOMDocument2>& xmlDoc);
	bool ExtractCDATAMemberNode(CComPtr<IXMLDOMNode>& parentElement, cmString& member);

	bool ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, cmString& member);
	bool ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, int& member);
	bool ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, long& member);
	bool ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, double& member);
	bool ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, bool& member);
	bool ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, CXmlSerializable& member);
	bool ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, VARIANT_BOOL& member);

	bool ExtractAttributeNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, cmString& member);
	bool ExtractAttributeNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, int& member);
	bool ExtractAttributeNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, long& member);
	bool ExtractAttributeNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, double& member);
	bool ExtractAttributeNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, bool& member);
	bool ExtractAttributeNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, VARIANT_BOOL& member);

	bool ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, cmString& member);
	bool ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, int& member);
	bool ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, long& member);
	bool ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, double& member);
	bool ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, bool& member);
	bool ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, VARIANT_BOOL& member);

	void AppendTextNode(cmString& value, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendTextNode(int member, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendTextNode(long member, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendTextNode(double member, int precision, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendTextNode(bool member, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);
	void AppendTextNode(VARIANT_BOOL member, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc);

	CComPtr<IXMLDOMNode> GetChildNode(	CComPtr<IXMLDOMNode>& parentElement, 
								const cmString& nodeName, 
								cmString& nodeValueStr);
};

#endif //!defined(_XMLSERIALIZABLE_H_)
