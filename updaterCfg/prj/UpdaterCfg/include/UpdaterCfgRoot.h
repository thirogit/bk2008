// UpdaterCfgRoot.h
// 
// generated on 2009.10.30 at 17:39:29.421
//
#if !defined(_UPDATERCFGROOT_H_)
#define _UPDATERCFGROOT_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
#include "UpdaterCfgUpdaterCfg.h"

using namespace std;

class UpdaterCfgUpdaterCfg;

class AFX_EXT_CLASS UpdaterCfgRoot: public CXmlSerializable
{
public:
    UpdaterCfgRoot();
    UpdaterCfgRoot(const UpdaterCfgRoot& copy);
    virtual ~UpdaterCfgRoot();


    // utils
    //
public:
    void operator=(const UpdaterCfgRoot& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("Root");}

protected:
    void Copy(const UpdaterCfgRoot& copy);

    // serialization
    //
    public:
    virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node);
    virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement);

    // Get/Set methods
    //
public:

    UpdaterCfgUpdaterCfg& GetUpdaterCfg();
    void SetUpdaterCfg(UpdaterCfgUpdaterCfg& UpdaterCfg);

    // inner members
    //
protected:
    UpdaterCfgUpdaterCfg updaterCfg_;

};

#endif //!defined(_UPDATERCFGROOT_H_)

