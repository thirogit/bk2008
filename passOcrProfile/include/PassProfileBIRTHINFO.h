// PassProfileBIRTHINFO.h
// 
// generated on 2009.12.25 at 14:19:49.287
//
#if !defined(_PASSPROFILEBIRTHINFO_H_)
#define _PASSPROFILEBIRTHINFO_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfileBIRTHINFO: public CXmlSerializable
{
public:
    PassProfileBIRTHINFO();
    PassProfileBIRTHINFO(const PassProfileBIRTHINFO& copy);
    virtual ~PassProfileBIRTHINFO();


    // utils
    //
public:
    void operator=(const PassProfileBIRTHINFO& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("BIRTHINFO");}

protected:
    void Copy(const PassProfileBIRTHINFO& copy);

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

#endif //!defined(_PASSPROFILEBIRTHINFO_H_)

