// PassProfileEAN.cpp
// 
// generated on 2009.12.25 at 14:19:49.317
//
#include "stdafx.h"
#include "PassProfileEAN.h"

PassProfileEAN::PassProfileEAN()
{
    SetDefaults();
}

PassProfileEAN::PassProfileEAN(const PassProfileEAN& eAN)
{
    Copy(eAN);
}

PassProfileEAN::~PassProfileEAN()
{
    Clear();
}

void PassProfileEAN::operator=(const PassProfileEAN& eAN)
{
    Copy(eAN);
}

void PassProfileEAN::Copy(const PassProfileEAN& eAN)
{
    Clear();

    passField_ = eAN.passField_;
}

void PassProfileEAN::SetDefaults()
{
    Clear();
}

void PassProfileEAN::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfileEAN::GetPassField()
{
    return passField_;
}

void PassProfileEAN::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfileEAN::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfileEAN::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

