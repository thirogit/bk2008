// XmlSerializable.cpp
//

#include "stdafx.h"
#include "XmlSerializable.h"
#include <comdef.h>

void CXmlSerializable::AppendMemberNode(const cmString& nodeName, 
										const cmString& nodeValue, 
										CComPtr<IXMLDOMElement>& parentElement,
										CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	USES_CONVERSION; 

	CComPtr<IXMLDOMElement> member;
	HRESULT hRes;

	hRes = xmlDoc->createElement(CComBSTR((const TCHAR*)nodeName), &member);
	member->put_text(CComBSTR((const TCHAR*)nodeValue));
	
	hRes = parentElement->appendChild(member, NULL);

	// member->Release();
}

void CXmlSerializable::AppendMemberNode(const cmString& nodeName, 
										 int nodeValue, 
										 CComPtr<IXMLDOMElement>& parentElement,
										 CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;

	buf.Format(_T("%d"), nodeValue);

	AppendMemberNode(nodeName, buf, parentElement, xmlDoc);
}

void CXmlSerializable::AppendMemberNode(const cmString& nodeName, 
										 long nodeValue, 
										 CComPtr<IXMLDOMElement>& parentElement,
										 CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;

	buf.Format(_T("%ld"), nodeValue);

	AppendMemberNode(nodeName, buf, parentElement, xmlDoc);
}

void CXmlSerializable::AppendMemberNode(const cmString& nodeName, 
										 bool nodeValue, 
										 CComPtr<IXMLDOMElement>& parentElement,
										 CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	AppendMemberNode(nodeName, (int)nodeValue, parentElement, xmlDoc);
}

void CXmlSerializable::AppendMemberNode(const cmString& nodeName, 
										 VARIANT_BOOL nodeValue, 
										 CComPtr<IXMLDOMElement>& parentElement,
										 CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	AppendMemberNode(nodeName, (nodeValue == VARIANT_TRUE) ? 1 : 0, parentElement, xmlDoc);
}

void CXmlSerializable::AppendMemberNode( const cmString& nodeName, 
										 double nodeValue, 
										 int precision,
										 CComPtr<IXMLDOMElement>& parentElement,
										 CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;
	cmString format;

	format.Format(_T("%%1.%df"), precision);
	buf.Format(format, nodeValue);

	AppendMemberNode(nodeName, buf, parentElement, xmlDoc);
}

bool CXmlSerializable::SaveToXmlFile(const cmString& fileName)
{
	USES_CONVERSION;

	CoInitialize(NULL);
    {	
        CComPtr<IXMLDOMDocument2> xmlDoc;
	    CComPtr<IXMLDOMProcessingInstruction> pi;
        CComPtr<IXMLDOMElement> parentElement;
	HRESULT hr;

	hr = CoCreateInstance(__uuidof(DOMDocument), //CLSID_DOMDocument, 
						  NULL, 
						  CLSCTX_INPROC_SERVER, 
						  __uuidof(IXMLDOMDocument2), //IID_IXMLDOMDocument2, 
						  (void**)&xmlDoc);
	if (FAILED(hr)) {
		CoUninitialize();		
		return false;
	}
	
	_bstr_t data;
	_bstr_t target;

	target = _T("xml");
	data = _T(" version='1.0' encoding='ISO-8859-1'");
	//data = _T(" version='1.0' encoding='UTF-8'");
	hr = xmlDoc->createProcessingInstruction(target, data, &pi);
	if (SUCCEEDED(hr)) {
		hr = xmlDoc->appendChild(pi, NULL);
		
		    AppendToDOMDocument(xmlDoc, parentElement);

		    //pi->Release();
		hr = xmlDoc->save(_variant_t(CComBSTR((const TCHAR*)fileName)));
	}
	//xmlDoc->Release();
    }
	CoUninitialize();
	
	return true;
}

bool CXmlSerializable::LoadFromXmlFile(const cmString& fileName)
{
	bool ret = false;

	CoInitialize(NULL);
    {
	    CComPtr<IXMLDOMDocument2> xmlDoc;
	    CComPtr<IXMLDOMNodeList> childList;
	    CComPtr<IXMLDOMNode> child;
	VARIANT_BOOL res;
	HRESULT hr;

	Clear();
	SetDefaults();
		
	hr = CoCreateInstance(__uuidof(DOMDocument), //CLSID_DOMDocument, 
						  NULL, 
						  CLSCTX_INPROC_SERVER, 
						  __uuidof(IXMLDOMDocument2), //IID_IXMLDOMDocument2, 
						  (void**)&xmlDoc);
	if (FAILED(hr)) {
		CoUninitialize();		
		return false;
	}
	
	hr = xmlDoc->load(_variant_t(CComVariant((const TCHAR*)fileName)), &res);
			
	if (SUCCEEDED(hr) && res == VARIANT_TRUE) {
		long size;
		CComBSTR nodeName;

		xmlDoc->get_childNodes(&childList);
		childList->get_length(&size);

		ret = false;
		for (long i = 0; i < size; i++) {
			    nodeName.Empty();
			childList->get_item(i, &child);
			child->get_nodeName(&nodeName);

			if (GetXmlNodeName() == nodeName)
				ret = BuildFromXml(child);
				    
                child.Release();
		    }		
		}		
	}

	CoUninitialize();

	return ret;
}

cmString CXmlSerializable::GetXmlNodeName()
{
	return _T("");
}

bool CXmlSerializable::ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement,
										 const cmString& nodeName,
										 cmString& member)
{
	CComPtr<IXMLDOMNode> childNode;
	cmString nodeValueStr;

	childNode = GetChildNode(parentElement, nodeName, nodeValueStr);
	if (childNode) {
		member = nodeValueStr;	
		
		return true;
	}

	return false;
}

bool CXmlSerializable::ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement,
										 const cmString& nodeName,
										 int& member)
{
	CComPtr<IXMLDOMNode> childNode;
	cmString nodeValueStr;

	childNode = GetChildNode(parentElement, nodeName, nodeValueStr);
	if (childNode) {
		member = nodeValueStr;	
		
		return true;
	}

	return false;
}

bool CXmlSerializable::ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement,
										 const cmString& nodeName,
										 long& member)
{
	CComPtr<IXMLDOMNode> childNode;
	cmString nodeValueStr;

	childNode = GetChildNode(parentElement, nodeName, nodeValueStr);
	if (childNode) {
		member = nodeValueStr;	
		
		return true;
	}

	return false;
}

bool CXmlSerializable::ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement,
										 const cmString& nodeName,
										 double& member)
{
	CComPtr<IXMLDOMNode> childNode;
	cmString nodeValueStr;

	childNode = GetChildNode(parentElement, nodeName, nodeValueStr);
	if (childNode) {
		member = nodeValueStr;	
		
		return true;
	}

	return false;
}

bool CXmlSerializable::ExtractMemberNode(CComPtr<IXMLDOMNode>& parentElement,
										 const cmString& nodeName,
										 CXmlSerializable& member)
{
	CComPtr<IXMLDOMNode> childNode;
	cmString nodeValueStr;
	
	childNode = GetChildNode(parentElement, nodeName, nodeValueStr);
	if (childNode) {
		member.BuildFromXml(childNode);	
		
		return true;
	}

	return false;
}

bool CXmlSerializable::ExtractMemberNode(	CComPtr<IXMLDOMNode>& parentElement,
											const cmString& nodeName,
											bool& member)
{
	CComPtr<IXMLDOMNode> childNode;
	cmString nodeValueStr;

	childNode = GetChildNode(parentElement, nodeName, nodeValueStr);
	if (childNode) {
		member = ((int)nodeValueStr == 1);	
		
		return true;
	}

	return false;
}


CComPtr<IXMLDOMNode> CXmlSerializable::GetChildNode(CComPtr<IXMLDOMNode>& parentElement, 
											   const cmString& nodeName,
											   cmString& nodeValueStr)
{
	USES_CONVERSION;

	CComPtr<IXMLDOMNode> childNode;
	CComPtr<IXMLDOMNodeList> children;
	HRESULT hRes;
	long size;
	CComBSTR tmpNodeName;
	CComVariant tmpNodeValue;

	hRes = parentElement->get_childNodes(&children);
	
	children->get_length(&size);
	for (long i = 0; i < size; i++) {
		tmpNodeName.Empty();
		hRes = children->get_item(i, &childNode);
		hRes = childNode->get_nodeName(&tmpNodeName);
		
		if (nodeName.CompareNoCase(W2T(tmpNodeName)) == 0) {
			childNode->get_nodeTypedValue(&tmpNodeValue);

			nodeValueStr = CComBSTR(tmpNodeValue.bstrVal);
			break;
		}
		childNode.Release();
	}

return childNode;
}

bool CXmlSerializable::ExtractMemberNode(	CComPtr<IXMLDOMNode>& parentElement,
											const cmString& nodeName,
											VARIANT_BOOL& member)
{
	CComPtr<IXMLDOMNode> childNode;
	cmString nodeValueStr;

	childNode = GetChildNode(parentElement, nodeName, nodeValueStr);
	if (childNode) {
		member = ((int)nodeValueStr == 1) ? VARIANT_TRUE : VARIANT_FALSE;
		
		return true;
	}

	return false;
}



void CXmlSerializable::AppendAttributeNode(	const cmString& nodeName, 
											const cmString& nodeValue, 
											CComPtr<IXMLDOMElement>& parentElement,
											CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	CComBSTR name;
	CComVariant value;
	HRESULT hr;

	name = (const TCHAR*) nodeName;
	value = nodeValue.AllocSysString();

	hr = parentElement->setAttribute(name, value);
}

void CXmlSerializable::AppendAttributeNode(	const cmString& nodeName, 
											int nodeValue, 
											CComPtr<IXMLDOMElement>& parentElement,
											CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;

	buf.Format(_T("%d"), nodeValue);

	AppendAttributeNode(nodeName, buf, parentElement, xmlDoc);

}
	
void CXmlSerializable::AppendAttributeNode(const cmString& nodeName, 
							long nodeValue, 
							CComPtr<IXMLDOMElement>& parentElement,
							CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;

	buf.Format(_T("%ld"), nodeValue);

	AppendAttributeNode(nodeName, buf, parentElement, xmlDoc);
}

void CXmlSerializable::AppendAttributeNode(	const cmString& nodeName, 
											double nodeValue, 
											int precision,
											CComPtr<IXMLDOMElement>& parentElement,
											CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;
	cmString format;

	format.Format(_T("%%1.%df"), precision);
	buf.Format(format, nodeValue);

	AppendAttributeNode(nodeName, buf, parentElement, xmlDoc);
}

void CXmlSerializable::AppendAttributeNode(	const cmString& nodeName, 
											bool nodeValue, 
											CComPtr<IXMLDOMElement>& parentElement,
											CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	AppendAttributeNode(nodeName, (long)nodeValue, parentElement, xmlDoc);
}

bool CXmlSerializable::ExtractAttributeNode(	CComPtr<IXMLDOMNode>& parentElement,
						const cmString& nodeName,
						cmString& member)
{
	CComBSTR name;
	CComVariant value;
	HRESULT hr;
	CComPtr<IXMLDOMElement> e;

	hr = parentElement->QueryInterface(__uuidof(IXMLDOMElement), (void**)&e);
	if (FAILED(hr))
		return false;

	name = (const TCHAR*) nodeName;
	
	hr = e->getAttribute(name, &value);
	if (SUCCEEDED(hr))
		member = CComBSTR(value.bstrVal);

	return SUCCEEDED(hr);
}

bool CXmlSerializable::ExtractAttributeNode(	CComPtr<IXMLDOMNode>& parentElement,
						const cmString& nodeName,
						int& member)
{
	cmString tmp;	
	bool ret;

	ret = ExtractAttributeNode(parentElement, nodeName, tmp);
	if (ret)
		member = tmp;

	return ret;
}
void CXmlSerializable::AppendAttributeNode(const cmString& nodeName, VARIANT_BOOL nodeValue, CComPtr<IXMLDOMElement>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	AppendAttributeNode(nodeName, nodeValue == VARIANT_TRUE ? 1L : 0L, parentElement, xmlDoc);
}

bool CXmlSerializable::ExtractAttributeNode(	CComPtr<IXMLDOMNode>& parentElement,
						const cmString& nodeName,
						long& member)
{
	cmString tmp;	
	bool ret;

	ret = ExtractAttributeNode(parentElement, nodeName, tmp);
	if (ret)
		member = tmp;

	return ret;
}

bool CXmlSerializable::ExtractAttributeNode(	CComPtr<IXMLDOMNode>& parentElement,
						const cmString& nodeName,
						double& member)
{
	cmString tmp;	
	bool ret;

	ret = ExtractAttributeNode(parentElement, nodeName, tmp);
	if (ret)
		member = tmp;

	return ret;
}

bool CXmlSerializable::ExtractAttributeNode(	CComPtr<IXMLDOMNode>& parentElement,
						const cmString& nodeName,
						bool& member)
{
	cmString tmp;	
	bool ret;

	ret = ExtractAttributeNode(parentElement, nodeName, tmp);
	if (ret)
		member = (tmp == _T("1"));

	return ret;
}

bool CXmlSerializable::ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, cmString& member)
{
	USES_CONVERSION;

	CComVariant tmpNodeValue;
	CComPtr<IXMLDOMNode> child;
	IXMLDOMNodeList* childList;
	HRESULT hr;
	long size;
	long i;
	DOMNodeType nodeType;

	hr = parentElement->get_childNodes(&childList);
	hr = childList->get_length(&size);

	member = _T("");
	for (i = 0; i < size; i++) {
		hr = childList->get_item(i, &child);
		hr = child->get_nodeType(&nodeType);

		if(nodeType == NODE_TEXT) {
			child->get_nodeTypedValue(&tmpNodeValue);
			member = CComBSTR(tmpNodeValue.bstrVal);
			break;
		}
	}

	return true;
}

bool CXmlSerializable::ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, int& member)
{
	cmString tmp;

	ExtractTextNode(parentElement, tmp);
	member = tmp;

	return true;
}

bool CXmlSerializable::ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, long& member)
{
	cmString tmp;

	ExtractTextNode(parentElement, tmp);
	member = tmp;

	return true;
}

bool CXmlSerializable::ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, double& member)
{
	cmString tmp;

	ExtractTextNode(parentElement, tmp);
	member = tmp;

	return true;
}
bool CXmlSerializable::ExtractAttributeNode(CComPtr<IXMLDOMNode>& parentElement, const cmString& nodeName, VARIANT_BOOL& member)
{
	cmString tmp;	
	bool ret;

	ret = ExtractAttributeNode(parentElement, nodeName, tmp);
	if (ret)
		member = (tmp == _T("1") ? VARIANT_TRUE : VARIANT_FALSE);

	return ret;
}

bool CXmlSerializable::ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, bool& member)
{
	cmString tmp;

	ExtractTextNode(parentElement, tmp);
	member = ((int)tmp == 1);	;

	return true;
}

bool CXmlSerializable::ExtractTextNode(CComPtr<IXMLDOMNode>& parentElement, VARIANT_BOOL& member)
{
	cmString tmp;

	ExtractTextNode(parentElement, tmp);
	member = (tmp == _T("1")) ? VARIANT_TRUE : VARIANT_FALSE;

	return true;
}


void CXmlSerializable::AppendTextNode(cmString& value, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	USES_CONVERSION; 

	CComPtr<IXMLDOMNode> newNode;

	HRESULT hr;
	
	hr = xmlDoc->createNode(CComVariant((int)NODE_TEXT), CComBSTR((const TCHAR*)value), NULL, &newNode);
	hr = newNode->put_text(CComBSTR((const TCHAR*)value));
	hr = parentElement->appendChild(newNode, NULL);
}

void CXmlSerializable::AppendTextNode(int value, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;

	buf.Format(_T("%d"), value);

	AppendTextNode(buf, parentElement, xmlDoc);
}

void CXmlSerializable::AppendTextNode(long value, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;

	buf.Format(_T("%ld"), value);

	AppendTextNode(buf, parentElement, xmlDoc);
}

void CXmlSerializable::AppendTextNode(double value, int precision, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	cmString buf;
	cmString format;

	format.Format(_T("%%1.%df"), precision);
	buf.Format(format, value);

	AppendTextNode(buf, parentElement, xmlDoc);
}

void CXmlSerializable::AppendTextNode(bool value, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	AppendTextNode((int)value, parentElement, xmlDoc);
}

void CXmlSerializable::AppendTextNode(VARIANT_BOOL value, CComPtr<IXMLDOMNode>& parentElement, CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	AppendTextNode((value == VARIANT_TRUE) ? 1 : 0, parentElement, xmlDoc);
}

void CXmlSerializable::AppendCDATAMemberNode(const cmString& nodeValue, CComPtr<IXMLDOMElement>& parentElement,CComPtr<IXMLDOMDocument2>& xmlDoc)
{
	USES_CONVERSION; 

	IXMLDOMCDATASection* cdata;
	CComPtr<IXMLDOMNode> newNode;

	HRESULT hr;

	hr = xmlDoc->createNode(CComVariant(NODE_CDATA_SECTION), T2BSTR(_T("")), T2BSTR(_T("")), &newNode);
	hr = newNode->QueryInterface(__uuidof(IXMLDOMCDATASection), (void**)&cdata);

	// replace sequence ]]> with ]]&gt;
	//
	cmString tmp(nodeValue);

	tmp.Replace(_T("]]>"), _T("]]&gt;"));
	cdata->appendData(T2BSTR((const TCHAR*)tmp));

	hr = parentElement->appendChild(cdata, NULL);
}

bool CXmlSerializable::ExtractCDATAMemberNode(CComPtr<IXMLDOMNode>& parentElement, cmString& member)
{
	CComPtr<IXMLDOMNode> child;
	CComPtr<IXMLDOMCDATASection> cdata;
	CComPtr<IXMLDOMNodeList> children;
	cmString tmp;
	CComBSTR data;
	HRESULT hr;
	DOMNodeType nodeType;
	long size;

	hr = parentElement->get_childNodes(&children);
	if (FAILED(hr) || children == NULL)
		return false;

	children->get_length(&size);
	for (long i = 0; i < size; i++) {
		hr = children->get_item(i, &child);
		if (FAILED(hr) || child == NULL)
			return false;

		hr = child->get_nodeType(&nodeType);
		if (nodeType == NODE_CDATA_SECTION) {
			data.Empty();
			hr = child->QueryInterface(__uuidof(IXMLDOMCDATASection), (void**)&cdata);
			hr = cdata->get_data(&data);

			tmp = data;
			tmp.Replace(_T("]]&gt;"), _T("]]>"));
			member = tmp;

			return true;
		}
	}
	
	return false;
}