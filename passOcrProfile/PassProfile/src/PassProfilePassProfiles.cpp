// PassProfilePassProfiles.cpp
// 
// generated on 2009.12.25 at 14:05:30.933
//
#include "stdafx.h"
#include "PassProfilePassProfiles.h"
#include "PassProfilePassProfile.h"

PassProfilePassProfiles::PassProfilePassProfiles()
{
    SetDefaults();
}

PassProfilePassProfiles::PassProfilePassProfiles(const PassProfilePassProfiles& passProfiles)
{
    Copy(passProfiles);
}

PassProfilePassProfiles::~PassProfilePassProfiles()
{
    Clear();
}

void PassProfilePassProfiles::operator=(const PassProfilePassProfiles& passProfiles)
{
    Copy(passProfiles);
}

void PassProfilePassProfiles::Copy(const PassProfilePassProfiles& passProfiles)
{
    Clear();

    passProfileArray_ = passProfiles.passProfileArray_;
}

void PassProfilePassProfiles::SetDefaults()
{
    Clear();
}

void PassProfilePassProfiles::Clear()
{
    passProfileArray_.Clear();
}

PassProfilePassProfileArray& PassProfilePassProfiles::GetPassProfileArray()
{
    return passProfileArray_;
}

void PassProfilePassProfiles::SetPassProfileArray(PassProfilePassProfileArray& passProfileArray)
{
    passProfileArray_ = passProfileArray;
}

bool PassProfilePassProfiles::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassProfileArray"), passProfileArray_);

    return true;
}

bool PassProfilePassProfiles::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    CComPtr<IXMLDOMElement> newElement;
    HRESULT hr;
    cmString tmp;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);

    passProfileArray_.AppendToDOMDocument(xmlDoc, newElement);

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

