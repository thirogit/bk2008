// PassProfilePassFieldArray.cpp
// 
// generated on 2009.12.04 at 18:36:36.000
//
#include "stdafx.h"
#include "PassProfilePassFieldArray.h"

PassProfilePassFieldArray::PassProfilePassFieldArray()
{
    SetDefaults();
}

PassProfilePassFieldArray::PassProfilePassFieldArray(const PassProfilePassFieldArray& passFieldArray)
{
    Copy(passFieldArray);
}

PassProfilePassFieldArray::~PassProfilePassFieldArray()
{
    Clear();
}

void PassProfilePassFieldArray::operator=(const PassProfilePassFieldArray& passFieldArray)
{
    Copy(passFieldArray);
}

void PassProfilePassFieldArray::Copy(const PassProfilePassFieldArray& passFieldArray)
{
    Clear();

    for (int i = 0; i < passFieldArray.GetSize(); i++)
        passFieldArray_.push_back(passFieldArray[i]);
}

void PassProfilePassFieldArray::SetDefaults()
{
    Clear();
}

void PassProfilePassFieldArray::Clear()
{
    passFieldArray_.clear();
}

int PassProfilePassFieldArray::GetSize() const
{
    return passFieldArray_.size();
}

const PassProfilePassField& PassProfilePassFieldArray::operator[] (int index) const
{
    return passFieldArray_[index];
}

PassProfilePassField& PassProfilePassFieldArray::operator[] (int index)
{
    return passFieldArray_[index];
}

void PassProfilePassFieldArray::Insert(int index, const PassProfilePassField& passField)
{
    passFieldArray_.insert(passFieldArray_.begin() + index, passField);
}

void PassProfilePassFieldArray:: Delete(int index)
{
    passFieldArray_.erase(passFieldArray_.begin() + index);
}

bool PassProfilePassFieldArray::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    USES_CONVERSION;

    CComPtr<IXMLDOMNode> childNode;
    CComPtr<IXMLDOMNodeList> children;
    CComBSTR nodeName;
    HRESULT hr;
    long size;
    PassProfilePassField newElement;

    SetDefaults();

    hr = node->get_childNodes(&children);
    hr = children->get_length(&size);
    for (long i = 0; i < size; i++) {
        nodeName.Empty();
        hr = children->get_item(i, &childNode);
        hr = childNode->get_nodeName(&nodeName);
        if (newElement.GetXmlNodeName().CompareNoCase(W2T((BSTR)nodeName)) == 0) {
            newElement.BuildFromXml(childNode);
            passFieldArray_.push_back(newElement);
        }
        childNode.Release();

    }

    return true;
}

bool PassProfilePassFieldArray::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    HRESULT hr;
    CComPtr<IXMLDOMElement> newElement;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);
    if (FAILED(hr))
        return false;

    for (unsigned int i = 0; i < passFieldArray_.size(); i++) {
        passFieldArray_[i].AppendToDOMDocument(xmlDoc, newElement);
    }

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

