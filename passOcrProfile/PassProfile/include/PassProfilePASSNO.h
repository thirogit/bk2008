// PassProfilePASSNO.h
// 
// generated on 2009.12.25 at 14:26:48.838
//
#if !defined(_PASSPROFILEPASSNO_H_)
#define _PASSPROFILEPASSNO_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfilePASSNO: public CXmlSerializable
{
public:
    PassProfilePASSNO();
    PassProfilePASSNO(const PassProfilePASSNO& copy);
    virtual ~PassProfilePASSNO();


    // utils
    //
public:
    void operator=(const PassProfilePASSNO& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("PASSNO");}

protected:
    void Copy(const PassProfilePASSNO& copy);

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

#endif //!defined(_PASSPROFILEPASSNO_H_)

