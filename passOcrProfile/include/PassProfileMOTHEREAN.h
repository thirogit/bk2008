// PassProfileMOTHEREAN.h
// 
// generated on 2009.12.25 at 14:26:48.833
//
#if !defined(_PASSPROFILEMOTHEREAN_H_)
#define _PASSPROFILEMOTHEREAN_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfileMOTHEREAN: public CXmlSerializable
{
public:
    PassProfileMOTHEREAN();
    PassProfileMOTHEREAN(const PassProfileMOTHEREAN& copy);
    virtual ~PassProfileMOTHEREAN();


    // utils
    //
public:
    void operator=(const PassProfileMOTHEREAN& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("MOTHEREAN");}

protected:
    void Copy(const PassProfileMOTHEREAN& copy);

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

#endif //!defined(_PASSPROFILEMOTHEREAN_H_)

