// PassProfilePassField.cpp
// 
// generated on 2009.12.26 at 15:36:20.467
//
#include "stdafx.h"
#include "PassProfilePassField.h"

PassProfilePassField::PassProfilePassField()
{
    SetDefaults();
}

PassProfilePassField::PassProfilePassField(const PassProfilePassField& passField)
{
    Copy(passField);
}

PassProfilePassField::~PassProfilePassField()
{
    Clear();
}

void PassProfilePassField::operator=(const PassProfilePassField& passField)
{
    Copy(passField);
}

void PassProfilePassField::Copy(const PassProfilePassField& passField)
{
    Clear();

    barcode_ = passField.barcode_;
    h_ = passField.h_;
    w_ = passField.w_;
    x_ = passField.x_;
    y_ = passField.y_;
}

void PassProfilePassField::SetDefaults()
{
    Clear();
    barcode_ = false;
    h_ = 0;
    w_ = 0;
    x_ = 0;
    y_ = 0;
}

void PassProfilePassField::Clear()
{
}

bool PassProfilePassField::GetBarcode() const
{
    return barcode_;
}

void PassProfilePassField::SetBarcode(bool barcode)
{
    barcode_ = barcode;
}

long PassProfilePassField::GetH() const
{
    return h_;
}

void PassProfilePassField::SetH(long h)
{
    h_ = h;
}

long PassProfilePassField::GetW() const
{
    return w_;
}

void PassProfilePassField::SetW(long w)
{
    w_ = w;
}

long PassProfilePassField::GetX() const
{
    return x_;
}

void PassProfilePassField::SetX(long x)
{
    x_ = x;
}

long PassProfilePassField::GetY() const
{
    return y_;
}

void PassProfilePassField::SetY(long y)
{
    y_ = y;
}

bool PassProfilePassField::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractAttributeNode(node, _T("Barcode"), barcode_);
    ExtractAttributeNode(node, _T("H"), h_);
    ExtractAttributeNode(node, _T("W"), w_);
    ExtractAttributeNode(node, _T("X"), x_);
    ExtractAttributeNode(node, _T("Y"), y_);

    return true;
}

bool PassProfilePassField::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    CComPtr<IXMLDOMElement> newElement;
    HRESULT hr;
    cmString tmp;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);

    AppendAttributeNode(_T("Barcode"), barcode_, newElement, xmlDoc);
    AppendAttributeNode(_T("H"), h_, newElement, xmlDoc);
    AppendAttributeNode(_T("W"), w_, newElement, xmlDoc);
    AppendAttributeNode(_T("X"), x_, newElement, xmlDoc);
    AppendAttributeNode(_T("Y"), y_, newElement, xmlDoc);

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

