// PassProfileEAN.h
// 
// generated on 2009.12.25 at 14:19:49.307
//
#if !defined(_PASSPROFILEEAN_H_)
#define _PASSPROFILEEAN_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfileEAN: public CXmlSerializable
{
public:
    PassProfileEAN();
    PassProfileEAN(const PassProfileEAN& copy);
    virtual ~PassProfileEAN();


    // utils
    //
public:
    void operator=(const PassProfileEAN& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("EAN");}

protected:
    void Copy(const PassProfileEAN& copy);

    // serialization
    //
    public:
    virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node);
    virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement);

    // Get/Set methods
    //
public:

    PassProfilePassField& GetPassField();
    void SetPassField(PassProfilePassField& PassField);

    // inner members
    //
protected:
    PassProfilePassField passField_;

};

#endif //!defined(_PASSPROFILEEAN_H_)

