// PassProfileSTOCK.h
// 
// generated on 2009.12.25 at 14:26:48.845
//
#if !defined(_PASSPROFILESTOCK_H_)
#define _PASSPROFILESTOCK_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfileSTOCK: public CXmlSerializable
{
public:
    PassProfileSTOCK();
    PassProfileSTOCK(const PassProfileSTOCK& copy);
    virtual ~PassProfileSTOCK();


    // utils
    //
public:
    void operator=(const PassProfileSTOCK& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("STOCK");}

protected:
    void Copy(const PassProfileSTOCK& copy);

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

#endif //!defined(_PASSPROFILESTOCK_H_)

