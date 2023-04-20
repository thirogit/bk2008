// UpdaterCfgUpdaterCfg.h
// 
// generated on 2009.10.30 at 17:39:29.421
//
#if !defined(_UPDATERCFGUPDATERCFG_H_)
#define _UPDATERCFGUPDATERCFG_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"

using namespace std;


class AFX_EXT_CLASS UpdaterCfgUpdaterCfg: public CXmlSerializable
{
public:
    UpdaterCfgUpdaterCfg();
    UpdaterCfgUpdaterCfg(const UpdaterCfgUpdaterCfg& copy);
    virtual ~UpdaterCfgUpdaterCfg();




    // utils
    //
public:
    void operator=(const UpdaterCfgUpdaterCfg& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("UpdaterCfg");}

protected:
    void Copy(const UpdaterCfgUpdaterCfg& copy);

    // serialization
    //
    public:
    virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node);
    virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement);

    // Get/Set methods
    //
public:

    cmString GetBaseURL() const;
    void SetBaseURL(const cmString& BaseURL);

    cmString GetVersionInfoFileName() const;
    void SetVersionInfoFileName(const cmString& VersionInfoFileName);

    cmString GetZipsDirName() const;
    void SetZipsDirName(const cmString& ZipsDirName);

    // inner members
    //
protected:
    cmString baseURL_;
    cmString versionInfoFileName_;
    cmString zipsDirName_;

};

#endif //!defined(_UPDATERCFGUPDATERCFG_H_)

