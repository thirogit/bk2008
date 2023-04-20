// PassProfileFARMNOBARCODE.h
// 
// generated on 2009.12.25 at 14:26:48.827
//
#if !defined(_PASSPROFILEFARMNOBARCODE_H_)
#define _PASSPROFILEFARMNOBARCODE_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "PassProfilePassField.h"

using namespace std;

class PassProfilePassField;

class AFX_EXT_CLASS PassProfileFARMNOBARCODE: public CXmlSerializable
{
public:
    PassProfileFARMNOBARCODE();
    PassProfileFARMNOBARCODE(const PassProfileFARMNOBARCODE& copy);
    virtual ~PassProfileFARMNOBARCODE();


    // utils
    //
public:
    void operator=(const PassProfileFARMNOBARCODE& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("FARMNOBARCODE");}

protected:
    void Copy(const PassProfileFARMNOBARCODE& copy);

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

#endif //!defined(_PASSPROFILEFARMNOBARCODE_H_)

