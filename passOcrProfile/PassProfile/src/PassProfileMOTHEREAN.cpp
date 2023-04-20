// PassProfileMOTHEREAN.cpp
// 
// generated on 2009.12.25 at 14:26:48.834
//
#include "stdafx.h"
#include "PassProfileMOTHEREAN.h"

PassProfileMOTHEREAN::PassProfileMOTHEREAN()
{
    SetDefaults();
}

PassProfileMOTHEREAN::PassProfileMOTHEREAN(const PassProfileMOTHEREAN& mOTHEREAN)
{
    Copy(mOTHEREAN);
}

PassProfileMOTHEREAN::~PassProfileMOTHEREAN()
{
    Clear();
}

void PassProfileMOTHEREAN::operator=(const PassProfileMOTHEREAN& mOTHEREAN)
{
    Copy(mOTHEREAN);
}

void PassProfileMOTHEREAN::Copy(const PassProfileMOTHEREAN& mOTHEREAN)
{
    Clear();

    passField_ = mOTHEREAN.passField_;
}

void PassProfileMOTHEREAN::SetDefaults()
{
    Clear();
}

void PassProfileMOTHEREAN::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfileMOTHEREAN::GetPassField()
{
    return passField_;
}

void PassProfileMOTHEREAN::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfileMOTHEREAN::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfileMOTHEREAN::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

