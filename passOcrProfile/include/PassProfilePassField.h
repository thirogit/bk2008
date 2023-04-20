// PassProfilePassField.h
// 
// generated on 2009.12.26 at 15:36:20.452
//
#if !defined(_PASSPROFILEPASSFIELD_H_)
#define _PASSPROFILEPASSFIELD_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"

using namespace std;


class AFX_EXT_CLASS PassProfilePassField: public CXmlSerializable
{
public:
    PassProfilePassField();
    PassProfilePassField(const PassProfilePassField& copy);
    virtual ~PassProfilePassField();






    // utils
    //
public:
    void operator=(const PassProfilePassField& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("PassField");}

protected:
    void Copy(const PassProfilePassField& copy);

    // serialization
    //
    public:
    virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node);
    virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement);

    // Get/Set methods
    //
public:

    bool GetBarcode() const;
    void SetBarcode(bool Barcode);

    long GetH() const;
    void SetH(long H);

    long GetW() const;
    void SetW(long W);

    long GetX() const;
    void SetX(long X);

    long GetY() const;
    void SetY(long Y);

    // inner members
    //
protected:
    bool barcode_;
    long h_;
    long w_;
    long x_;
    long y_;

};

#endif //!defined(_PASSPROFILEPASSFIELD_H_)

