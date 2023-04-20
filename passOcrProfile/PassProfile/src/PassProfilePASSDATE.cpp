// PassProfilePASSDATE.cpp
// 
// generated on 2009.12.25 at 14:26:48.836
//
#include "stdafx.h"
#include "PassProfilePASSDATE.h"

PassProfilePASSDATE::PassProfilePASSDATE()
{
    SetDefaults();
}

PassProfilePASSDATE::PassProfilePASSDATE(const PassProfilePASSDATE& pASSDATE)
{
    Copy(pASSDATE);
}

PassProfilePASSDATE::~PassProfilePASSDATE()
{
    Clear();
}

void PassProfilePASSDATE::operator=(const PassProfilePASSDATE& pASSDATE)
{
    Copy(pASSDATE);
}

void PassProfilePASSDATE::Copy(const PassProfilePASSDATE& pASSDATE)
{
    Clear();

    passField_ = pASSDATE.passField_;
}

void PassProfilePASSDATE::SetDefaults()
{
    Clear();
}

void PassProfilePASSDATE::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfilePASSDATE::GetPassField()
{
    return passField_;
}

void PassProfilePASSDATE::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfilePASSDATE::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfilePASSDATE::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

