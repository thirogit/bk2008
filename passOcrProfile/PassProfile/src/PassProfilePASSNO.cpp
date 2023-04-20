// PassProfilePASSNO.cpp
// 
// generated on 2009.12.25 at 14:26:48.839
//
#include "stdafx.h"
#include "PassProfilePASSNO.h"

PassProfilePASSNO::PassProfilePASSNO()
{
    SetDefaults();
}

PassProfilePASSNO::PassProfilePASSNO(const PassProfilePASSNO& pASSNO)
{
    Copy(pASSNO);
}

PassProfilePASSNO::~PassProfilePASSNO()
{
    Clear();
}

void PassProfilePASSNO::operator=(const PassProfilePASSNO& pASSNO)
{
    Copy(pASSNO);
}

void PassProfilePASSNO::Copy(const PassProfilePASSNO& pASSNO)
{
    Clear();

    passField_ = pASSNO.passField_;
}

void PassProfilePASSNO::SetDefaults()
{
    Clear();
}

void PassProfilePASSNO::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfilePASSNO::GetPassField()
{
    return passField_;
}

void PassProfilePASSNO::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfilePASSNO::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfilePASSNO::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

