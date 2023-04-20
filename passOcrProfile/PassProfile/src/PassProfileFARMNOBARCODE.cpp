// PassProfileFARMNOBARCODE.cpp
// 
// generated on 2009.12.25 at 14:26:48.828
//
#include "stdafx.h"
#include "PassProfileFARMNOBARCODE.h"

PassProfileFARMNOBARCODE::PassProfileFARMNOBARCODE()
{
    SetDefaults();
}

PassProfileFARMNOBARCODE::PassProfileFARMNOBARCODE(const PassProfileFARMNOBARCODE& fARMNOBARCODE)
{
    Copy(fARMNOBARCODE);
}

PassProfileFARMNOBARCODE::~PassProfileFARMNOBARCODE()
{
    Clear();
}

void PassProfileFARMNOBARCODE::operator=(const PassProfileFARMNOBARCODE& fARMNOBARCODE)
{
    Copy(fARMNOBARCODE);
}

void PassProfileFARMNOBARCODE::Copy(const PassProfileFARMNOBARCODE& fARMNOBARCODE)
{
    Clear();

    passField_ = fARMNOBARCODE.passField_;
}

void PassProfileFARMNOBARCODE::SetDefaults()
{
    Clear();
}

void PassProfileFARMNOBARCODE::Clear()
{
    passField_.Clear();
}

PassProfilePassField& PassProfileFARMNOBARCODE::GetPassField()
{
    return passField_;
}

void PassProfileFARMNOBARCODE::SetPassField(PassProfilePassField& passField)
{
    passField_ = passField;
}

bool PassProfileFARMNOBARCODE::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("PassField"), passField_);

    return true;
}

bool PassProfileFARMNOBARCODE::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
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

