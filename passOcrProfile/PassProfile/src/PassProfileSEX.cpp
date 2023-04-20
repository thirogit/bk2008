// PassProfileSEX.cpp
// 
// generated on 2009.12.25 at 14:26:48.844
//
#include "stdafx.h"
#include "PassProfileSEX.h"

PassProfileSEX::PassProfileSEX()
{
    SetDefaults();
}

PassProfileSEX::PassProfileSEX(const PassProfileSEX& sEX)
{
    Copy(sEX);
}

PassProfileSEX::~PassProfileSEX()
{
    Clear();
}

void PassProfileSEX::operator=(const PassProfileSEX& sEX)
{
    Copy(sEX);
}

void PassProfileSEX::Copy(const PassProfileSEX& sEX)
{
    Clear();

    passField_ = sEX.passField_;
}

void PassProfileSEX::SetDefaults()
{
    Clear();
}

void PassProfileSEX::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfileSEX::GetPassField()
{
    return passField_;
}

void PassProfileSEX::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfileSEX::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfileSEX::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    CComPtr<IXMLDOMElement> newElement;
    HRESULT hr;
    cmString tmp;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);

    passField_.AppendToDOMDocument(xmlDoc, newElement);

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

