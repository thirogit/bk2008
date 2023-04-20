// PassProfileRoot.cpp
// 
// generated on 2009.12.25 at 14:37:59.154
//
#include "stdafx.h"
#include "PassProfileRoot.h"
#include "PassProfilePassField.h"
#include "PassProfilePassProfiles.h"

PassProfileRoot::PassProfileRoot()
{
    SetDefaults();
}

PassProfileRoot::PassProfileRoot(const PassProfileRoot& root)
{
    Copy(root);
}

PassProfileRoot::~PassProfileRoot()
{
    Clear();
}

void PassProfileRoot::operator=(const PassProfileRoot& root)
{
    Copy(root);
}

void PassProfileRoot::Copy(const PassProfileRoot& root)
{
    Clear();

    passField_ = root.passField_;
    passProfiles_ = root.passProfiles_;
}

void PassProfileRoot::SetDefaults()
{
    Clear();
}

void PassProfileRoot::Clear()
{
    passField_.Clear();
    passProfiles_.Clear();
}

PassProfilePassField& PassProfileRoot::GetPassField()
{
    return passField_;
}

void PassProfileRoot::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

PassProfilePassProfiles& PassProfileRoot::GetPassProfiles()
{
    return passProfiles_;
}

void PassProfileRoot::SetPassProfiles(PassProfilePassProfiles& passProfiles)
{
    passProfiles_ = passProfiles;
}

bool PassProfileRoot::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);
    ExtractMemberNode(node, _T("PassProfiles"), passProfiles_);

    return true;
}

bool PassProfileRoot::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    CComPtr<IXMLDOMElement> newElement;
    HRESULT hr;
    cmString tmp;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);

    passField_.AppendToDOMDocument(xmlDoc, newElement);
    passProfiles_.AppendToDOMDocument(xmlDoc, newElement);

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

