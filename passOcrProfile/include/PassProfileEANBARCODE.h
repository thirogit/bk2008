// PassProfileEANBARCODE.h
// 
// generated on 2009.12.25 at 14:26:48.824
//
#if !defined(_PASSPROFILEEANBARCODE_H_)
#define _PASSPROFILEEANBARCODE_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfileEANBARCODE: public CXmlSerializable
{
public:
    PassProfileEANBARCODE();
    PassProfileEANBARCODE(const PassProfileEANBARCODE& copy);
    virtual ~PassProfileEANBARCODE();


    // utils
    //
public:
    void operator=(const PassProfileEANBARCODE& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("EANBARCODE");}

protected:
    void Copy(const PassProfileEANBARCODE& copy);

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

#endif //!defined(_PASSPROFILEEANBARCODE_H_)

