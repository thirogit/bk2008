// PassProfileFIRSTOWNER.cpp
// 
// generated on 2009.12.25 at 14:26:48.831
//
#include "stdafx.h"
#include "PassProfileFIRSTOWNER.h"

PassProfileFIRSTOWNER::PassProfileFIRSTOWNER()
{
    SetDefaults();
}

PassProfileFIRSTOWNER::PassProfileFIRSTOWNER(const PassProfileFIRSTOWNER& fIRSTOWNER)
{
    Copy(fIRSTOWNER);
}

PassProfileFIRSTOWNER::~PassProfileFIRSTOWNER()
{
    Clear();
}

void PassProfileFIRSTOWNER::operator=(const PassProfileFIRSTOWNER& fIRSTOWNER)
{
    Copy(fIRSTOWNER);
}

void PassProfileFIRSTOWNER::Copy(const PassProfileFIRSTOWNER& fIRSTOWNER)
{
    Clear();

    passField_ = fIRSTOWNER.passField_;
}

void PassProfileFIRSTOWNER::SetDefaults()
{
    Clear();
}

void PassProfileFIRSTOWNER::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfileFIRSTOWNER::GetPassField()
{
    return passField_;
}

void PassProfileFIRSTOWNER::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfileFIRSTOWNER::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfileFIRSTOWNER::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

