// UpdaterCfgRoot.cpp
// 
// generated on 2009.10.30 at 17:39:29.421
//
#include "stdafx.h"
#include "UpdaterCfgRoot.h"
#include "UpdaterCfgUpdaterCfg.h"

UpdaterCfgRoot::UpdaterCfgRoot()
{
    SetDefaults();
}

UpdaterCfgRoot::UpdaterCfgRoot(const UpdaterCfgRoot& root)
{
    Copy(root);
}

UpdaterCfgRoot::~UpdaterCfgRoot()
{
    Clear();
}

void UpdaterCfgRoot::operator=(const UpdaterCfgRoot& root)
{
    Copy(root);
}

void UpdaterCfgRoot::Copy(const UpdaterCfgRoot& root)
{
    Clear();

    updaterCfg_ = root.updaterCfg_;
}

void UpdaterCfgRoot::SetDefaults()
{
    Clear();
}

void UpdaterCfgRoot::Clear()
{
    updaterCfg_.Clear();
}

UpdaterCfgUpdaterCfg& UpdaterCfgRoot::GetUpdaterCfg()
{
    return updaterCfg_;
}

void UpdaterCfgRoot::SetUpdaterCfg(UpdaterCfgUpdaterCfg& updaterCfg)
{
    updaterCfg_ = updaterCfg;
}

bool UpdaterCfgRoot::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("UpdaterCfg"), updaterCfg_);

    return true;
}

bool UpdaterCfgRoot::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    CComPtr<IXMLDOMElement> newElement;
    HRESULT hr;
    cmString tmp;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);

    updaterCfg_.AppendToDOMDocument(xmlDoc, newElement);

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

