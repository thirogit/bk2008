// UpdaterCfgUpdaterCfg.cpp
// 
// generated on 2009.10.30 at 17:39:29.421
//
#include "stdafx.h"
#include "UpdaterCfgUpdaterCfg.h"

UpdaterCfgUpdaterCfg::UpdaterCfgUpdaterCfg()
{
    SetDefaults();
}

UpdaterCfgUpdaterCfg::UpdaterCfgUpdaterCfg(const UpdaterCfgUpdaterCfg& updaterCfg)
{
    Copy(updaterCfg);
}

UpdaterCfgUpdaterCfg::~UpdaterCfgUpdaterCfg()
{
    Clear();
}

void UpdaterCfgUpdaterCfg::operator=(const UpdaterCfgUpdaterCfg& updaterCfg)
{
    Copy(updaterCfg);
}

void UpdaterCfgUpdaterCfg::Copy(const UpdaterCfgUpdaterCfg& updaterCfg)
{
    Clear();

    baseURL_ = updaterCfg.baseURL_;
    versionInfoFileName_ = updaterCfg.versionInfoFileName_;
    zipsDirName_ = updaterCfg.zipsDirName_;
}

void UpdaterCfgUpdaterCfg::SetDefaults()
{
    Clear();
    baseURL_ = _T("");
    versionInfoFileName_ = _T("");
    zipsDirName_ = _T("");
}

void UpdaterCfgUpdaterCfg::Clear()
{
}

cmString UpdaterCfgUpdaterCfg::GetBaseURL() const
{
    return baseURL_;
}

void UpdaterCfgUpdaterCfg::SetBaseURL(const cmString& baseURL)
{
    baseURL_ = baseURL;
}

cmString UpdaterCfgUpdaterCfg::GetVersionInfoFileName() const
{
    return versionInfoFileName_;
}

void UpdaterCfgUpdaterCfg::SetVersionInfoFileName(const cmString& versionInfoFileName)
{
    versionInfoFileName_ = versionInfoFileName;
}

cmString UpdaterCfgUpdaterCfg::GetZipsDirName() const
{
    return zipsDirName_;
}

void UpdaterCfgUpdaterCfg::SetZipsDirName(const cmString& zipsDirName)
{
    zipsDirName_ = zipsDirName;
}

bool UpdaterCfgUpdaterCfg::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("BaseURL"), baseURL_);
    ExtractMemberNode(node, _T("VersionInfoFileName"), versionInfoFileName_);
    ExtractMemberNode(node, _T("ZipsDirName"), zipsDirName_);

    return true;
}

bool UpdaterCfgUpdaterCfg::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    CComPtr<IXMLDOMElement> newElement;
    HRESULT hr;
    cmString tmp;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);

    AppendMemberNode(_T("BaseURL"), baseURL_, newElement, xmlDoc);
    AppendMemberNode(_T("VersionInfoFileName"), versionInfoFileName_, newElement, xmlDoc);
    AppendMemberNode(_T("ZipsDirName"), zipsDirName_, newElement, xmlDoc);

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

