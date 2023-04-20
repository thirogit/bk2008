// PassProfileSTOCK.cpp
// 
// generated on 2009.12.25 at 14:26:48.847
//
#include "stdafx.h"
#include "PassProfileSTOCK.h"

PassProfileSTOCK::PassProfileSTOCK()
{
    SetDefaults();
}

PassProfileSTOCK::PassProfileSTOCK(const PassProfileSTOCK& sTOCK)
{
    Copy(sTOCK);
}

PassProfileSTOCK::~PassProfileSTOCK()
{
    Clear();
}

void PassProfileSTOCK::operator=(const PassProfileSTOCK& sTOCK)
{
    Copy(sTOCK);
}

void PassProfileSTOCK::Copy(const PassProfileSTOCK& sTOCK)
{
    Clear();

    passField_ = sTOCK.passField_;
}

void PassProfileSTOCK::SetDefaults()
{
    Clear();
}

void PassProfileSTOCK::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfileSTOCK::GetPassField()
{
    return passField_;
}

void PassProfileSTOCK::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfileSTOCK::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfileSTOCK::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

