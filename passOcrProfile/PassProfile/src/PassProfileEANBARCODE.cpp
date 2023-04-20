// PassProfileEANBARCODE.cpp
// 
// generated on 2009.12.25 at 14:26:48.825
//
#include "stdafx.h"
#include "PassProfileEANBARCODE.h"

PassProfileEANBARCODE::PassProfileEANBARCODE()
{
    SetDefaults();
}

PassProfileEANBARCODE::PassProfileEANBARCODE(const PassProfileEANBARCODE& eANBARCODE)
{
    Copy(eANBARCODE);
}

PassProfileEANBARCODE::~PassProfileEANBARCODE()
{
    Clear();
}

void PassProfileEANBARCODE::operator=(const PassProfileEANBARCODE& eANBARCODE)
{
    Copy(eANBARCODE);
}

void PassProfileEANBARCODE::Copy(const PassProfileEANBARCODE& eANBARCODE)
{
    Clear();

    passField_ = eANBARCODE.passField_;
}

void PassProfileEANBARCODE::SetDefaults()
{
    Clear();
}

void PassProfileEANBARCODE::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfileEANBARCODE::GetPassField()
{
    return passField_;
}

void PassProfileEANBARCODE::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfileEANBARCODE::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfileEANBARCODE::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

