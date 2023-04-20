// PassProfileFIRSTOWNER.h
// 
// generated on 2009.12.25 at 14:26:48.830
//
#if !defined(_PASSPROFILEFIRSTOWNER_H_)
#define _PASSPROFILEFIRSTOWNER_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfileFIRSTOWNER: public CXmlSerializable
{
public:
    PassProfileFIRSTOWNER();
    PassProfileFIRSTOWNER(const PassProfileFIRSTOWNER& copy);
    virtual ~PassProfileFIRSTOWNER();


    // utils
    //
public:
    void operator=(const PassProfileFIRSTOWNER& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("FIRSTOWNER");}

protected:
    void Copy(const PassProfileFIRSTOWNER& copy);

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

#endif //!defined(_PASSPROFILEFIRSTOWNER_H_)

