// PassProfileRoot.h
// 
// generated on 2009.12.25 at 14:37:59.152
//
#if !defined(_PASSPROFILEROOT_H_)
#define _PASSPROFILEROOT_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"
#include "PassProfilePassProfiles.h"

using namespace std;

class PassProfilePassField;
class PassProfilePassProfiles;

class AFX_EXT_CLASS PassProfileRoot: public CXmlSerializable
{
public:
    PassProfileRoot();
    PassProfileRoot(const PassProfileRoot& copy);
    virtual ~PassProfileRoot();



    // utils
    //
public:
    void operator=(const PassProfileRoot& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("Root");}

protected:
    void Copy(const PassProfileRoot& copy);

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

    PassProfilePassProfiles& GetPassProfiles();
    void SetPassProfiles(PassProfilePassProfiles& PassProfiles);

    // inner members
    //
protected:
    PassProfilePassField passField_;
    PassProfilePassProfiles passProfiles_;

};

#endif //!defined(_PASSPROFILEROOT_H_)

