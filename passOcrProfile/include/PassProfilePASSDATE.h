// PassProfilePASSDATE.h
// 
// generated on 2009.12.25 at 14:26:48.835
//
#if !defined(_PASSPROFILEPASSDATE_H_)
#define _PASSPROFILEPASSDATE_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfilePASSDATE: public CXmlSerializable
{
public:
    PassProfilePASSDATE();
    PassProfilePASSDATE(const PassProfilePASSDATE& copy);
    virtual ~PassProfilePASSDATE();


    // utils
    //
public:
    void operator=(const PassProfilePASSDATE& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("PASSDATE");}

protected:
    void Copy(const PassProfilePASSDATE& copy);

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

#endif //!defined(_PASSPROFILEPASSDATE_H_)

