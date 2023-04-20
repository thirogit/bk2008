// PassProfilePassProfileArray.h
// 
// generated on 2009.12.25 at 14:05:30.998
//
#if !defined(_PASSPROFILEPASSPROFILE_H_Array)
#define _PASSPROFILEPASSPROFILE_H_Array

#include <vector>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassProfile.h"


using namespace std;

class AFX_EXT_CLASS PassProfilePassProfileArray: public CXmlSerializable
{
public:
    PassProfilePassProfileArray();
    PassProfilePassProfileArray(const PassProfilePassProfileArray& copy);
    virtual ~PassProfilePassProfileArray();

    // utils
    //
public:
    void operator=(const PassProfilePassProfileArray& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("PassProfileArray");}

    // access methods
    //
    int GetSize() const;
    const PassProfilePassProfile& operator[] (int index) const;
    PassProfilePassProfile& operator[] (int index);
    void Insert(int index, const PassProfilePassProfile& passProfile);
    void Delete(int index);

protected:
    void Copy(const PassProfilePassProfileArray& copy);

    // serialization
    //
    public:
    virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node);
    virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement);

    // inner members
    //
protected:
    vector<PassProfilePassProfile> passProfileArray_;
};

#endif //!defined(_PASSPROFILEPASSPROFILEARRAY_H_)

