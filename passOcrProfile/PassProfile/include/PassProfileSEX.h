// PassProfileSEX.h
// 
// generated on 2009.12.25 at 14:26:48.842
//
#if !defined(_PASSPROFILESEX_H_)
#define _PASSPROFILESEX_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfileSEX: public CXmlSerializable
{
public:
    PassProfileSEX();
    PassProfileSEX(const PassProfileSEX& copy);
    virtual ~PassProfileSEX();


    // utils
    //
public:
    void operator=(const PassProfileSEX& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("SEX");}

protected:
    void Copy(const PassProfileSEX& copy);

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

#endif //!defined(_PASSPROFILESEX_H_)

