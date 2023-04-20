// PassProfilePassProfileArray.cpp
// 
// generated on 2009.12.25 at 14:37:59.181
//
#include "stdafx.h"
#include "PassProfilePassProfileArray.h"
#include "PassProfileBIRTHINFO.h"
#include "PassProfileEAN.h"
#include "PassProfileEANBARCODE.h"
#include "PassProfileFARMNOBARCODE.h"
#include "PassProfileFIRSTOWNER.h"
#include "PassProfileMOTHEREAN.h"
#include "PassProfilePASSDATE.h"
#include "PassProfilePASSNO.h"
#include "PassProfileSEX.h"
#include "PassProfileSTOCK.h"

PassProfilePassProfileArray::PassProfilePassProfileArray()
{
    SetDefaults();
}

PassProfilePassProfileArray::PassProfilePassProfileArray(const PassProfilePassProfileArray& passProfileArray)
{
    Copy(passProfileArray);
}

PassProfilePassProfileArray::~PassProfilePassProfileArray()
{
    Clear();
}

void PassProfilePassProfileArray::operator=(const PassProfilePassProfileArray& passProfileArray)
{
    Copy(passProfileArray);
}

void PassProfilePassProfileArray::Copy(const PassProfilePassProfileArray& passProfileArray)
{
    Clear();

    for (int i = 0; i < passProfileArray.GetSize(); i++)
        passProfileArray_.push_back(passProfileArray[i]);
}

void PassProfilePassProfileArray::SetDefaults()
{
    Clear();
}

void PassProfilePassProfileArray::Clear()
{
    passProfileArray_.clear();
}

int PassProfilePassProfileArray::GetSize() const
{
    return passProfileArray_.size();
}

const PassProfilePassProfile& PassProfilePassProfileArray::operator[] (int index) const
{
    return passProfileArray_[index];
}

PassProfilePassProfile& PassProfilePassProfileArray::operator[] (int index)
{
    return passProfileArray_[index];
}

void PassProfilePassProfileArray::Insert(int index, const PassProfilePassProfile& passProfile)
{
    passProfileArray_.insert(passProfileArray_.begin() + index, passProfile);
}

void PassProfilePassProfileArray:: Delete(int index)
{
    passProfileArray_.erase(passProfileArray_.begin() + index);
}

bool PassProfilePassProfileArray::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    USES_CONVERSION;

    CComPtr<IXMLDOMNode> childNode;
    CComPtr<IXMLDOMNodeList> children;
    CComBSTR nodeName;
    HRESULT hr;
    long size;
    PassProfilePassProfile newElement;

    SetDefaults();

    hr = node->get_childNodes(&children);
    hr = children->get_length(&size);
    for (long i = 0; i < size; i++) {
        nodeName.Empty();
        hr = children->get_item(i, &childNode);
        hr = childNode->get_nodeName(&nodeName);
        if (newElement.GetXmlNodeName().CompareNoCase(W2T((BSTR)nodeName)) == 0) {
            newElement.BuildFromXml(childNode);
            passProfileArray_.push_back(newElement);
        }
        childNode.Release();

    }

    return true;
}

bool PassProfilePassProfileArray::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    HRESULT hr;
    CComPtr<IXMLDOMElement> newElement;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);
    if (FAILED(hr))
        return false;

    for (unsigned int i = 0; i < passProfileArray_.size(); i++) {
        passProfileArray_[i].AppendToDOMDocument(xmlDoc, newElement);
    }

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

