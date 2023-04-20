// PassProfilePassProfiles.h
// 
// generated on 2009.12.25 at 14:05:30.923
//
#if !defined(_PASSPROFILEPASSPROFILES_H_)
#define _PASSPROFILEPASSPROFILES_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassProfileArray.h"

using namespace std;

class PassProfilePassProfile;

class AFX_EXT_CLASS PassProfilePassProfiles: public CXmlSerializable
{
public:
    PassProfilePassProfiles();
    PassProfilePassProfiles(const PassProfilePassProfiles& copy);
    virtual ~PassProfilePassProfiles();


    // utils
    //
public:
    void operator=(const PassProfilePassProfiles& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("PassProfiles");}

protected:
    void Copy(const PassProfilePassProfiles& copy);

    // serialization
    //
    public:
    virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node);
    virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement);

    // Get/Set methods
    //
public:

    PassProfilePassProfileArray& GetPassProfileArray();
    void SetPassProfileArray(PassProfilePassProfileArray& PassProfileArray);

    // inner members
    //
protected:
    PassProfilePassProfileArray passProfileArray_;

};

#endif //!defined(_PASSPROFILEPASSPROFILES_H_)

