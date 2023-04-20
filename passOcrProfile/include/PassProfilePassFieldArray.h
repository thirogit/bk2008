// PassProfilePassFieldArray.h
// 
// generated on 2009.12.04 at 18:36:36.000
//
#if !defined(_PASSPROFILEPASSFIELD_H_Array)
#define _PASSPROFILEPASSFIELD_H_Array

#include <vector>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"


using namespace std;

class AFX_EXT_CLASS PassProfilePassFieldArray: public CXmlSerializable
{
public:
    PassProfilePassFieldArray();
    PassProfilePassFieldArray(const PassProfilePassFieldArray& copy);
    virtual ~PassProfilePassFieldArray();

    // utils
    //
public:
    void operator=(const PassProfilePassFieldArray& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("PassFieldArray");}

    // access methods
    //
    int GetSize() const;
    const PassProfilePassField& operator[] (int index) const;
    PassProfilePassField& operator[] (int index);
    void Insert(int index, const PassProfilePassField& passField);
    void Delete(int index);

protected:
    void Copy(const PassProfilePassFieldArray& copy);

    // serialization
    //
    public:
    virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node);
    virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement);

    // inner members
    //
protected:
    vector<PassProfilePassField> passFieldArray_;
};

#endif //!defined(_PASSPROFILEPASSFIELDARRAY_H_)

