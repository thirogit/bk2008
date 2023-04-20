// PassProfileBIRTHINFO.cpp
// 
// generated on 2009.12.25 at 14:19:49.297
//
#include "stdafx.h"
#include "PassProfileBIRTHINFO.h"

PassProfileBIRTHINFO::PassProfileBIRTHINFO()
{
    SetDefaults();
}

PassProfileBIRTHINFO::PassProfileBIRTHINFO(const PassProfileBIRTHINFO& bIRTHINFO)
{
    Copy(bIRTHINFO);
}

PassProfileBIRTHINFO::~PassProfileBIRTHINFO()
{
    Clear();
}

void PassProfileBIRTHINFO::operator=(const PassProfileBIRTHINFO& bIRTHINFO)
{
    Copy(bIRTHINFO);
}

void PassProfileBIRTHINFO::Copy(const PassProfileBIRTHINFO& bIRTHINFO)
{
    Clear();

    passField_ = bIRTHINFO.passField_;
}

void PassProfileBIRTHINFO::SetDefaults()
{
    Clear();
}

void PassProfileBIRTHINFO::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfileBIRTHINFO::GetPassField()
{
    return passField_;
}

void PassProfileBIRTHINFO::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfileBIRTHINFO::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfileBIRTHINFO::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

