// PassProfilePassProfile.cpp
// 
// generated on 2009.12.25 at 14:37:59.161
//
#include "stdafx.h"
#include "PassProfilePassProfile.h"
#include "PassProfileBIRTHINFO.h"
#include "PassProfileEAN.h"
#include "PassProfileEANBARCODE.h"
#include "PassProfileFARMNOBARCODE.h"
#include "PassProfileFIRSTOWNER.h"
#include "PassProfileMOTHEREAN.h"
#include "PassProfilePASSDATE.h"
#include "PassProfilePASSNO.h"
#include "PassProfileSEX.h"
#include "PassProfileSTOCK.h"

PassProfilePassProfile::PassProfilePassProfile()
{
    SetDefaults();
}

PassProfilePassProfile::PassProfilePassProfile(const PassProfilePassProfile& passProfile)
{
    Copy(passProfile);
}

PassProfilePassProfile::~PassProfilePassProfile()
{
    Clear();
}

void PassProfilePassProfile::operator=(const PassProfilePassProfile& passProfile)
{
    Copy(passProfile);
}

void PassProfilePassProfile::Copy(const PassProfilePassProfile& passProfile)
{
    Clear();

    bIRTHINFO_ = passProfile.bIRTHINFO_;
    eAN_ = passProfile.eAN_;
    eANBARCODE_ = passProfile.eANBARCODE_;
    fARMNOBARCODE_ = passProfile.fARMNOBARCODE_;
    fIRSTOWNER_ = passProfile.fIRSTOWNER_;
    mOTHEREAN_ = passProfile.mOTHEREAN_;
    pASSDATE_ = passProfile.pASSDATE_;
    pASSNO_ = passProfile.pASSNO_;
    passDimX_ = passProfile.passDimX_;
    passDimY_ = passProfile.passDimY_;
    passProfileName_ = passProfile.passProfileName_;
    sEX_ = passProfile.sEX_;
    sTOCK_ = passProfile.sTOCK_;
}

void PassProfilePassProfile::SetDefaults()
{
    Clear();
    passDimX_ = 0;
    passDimY_ = 0;
    passProfileName_ = _T("");
}

void PassProfilePassProfile::Clear()
{
    bIRTHINFO_.Clear();
    eAN_.Clear();
    eANBARCODE_.Clear();
    fARMNOBARCODE_.Clear();
    fIRSTOWNER_.Clear();
    mOTHEREAN_.Clear();
    pASSDATE_.Clear();
    pASSNO_.Clear();
    sEX_.Clear();
    sTOCK_.Clear();
}

PassProfileBIRTHINFO& PassProfilePassProfile::GetBIRTHINFO()
{
    return bIRTHINFO_;
}

void PassProfilePassProfile::SetBIRTHINFO(PassProfileBIRTHINFO& bIRTHINFO)
{
    bIRTHINFO_ = bIRTHINFO;
}

PassProfileEAN& PassProfilePassProfile::GetEAN()
{
    return eAN_;
}

void PassProfilePassProfile::SetEAN(PassProfileEAN& eAN)
{
    eAN_ = eAN;
}

PassProfileEANBARCODE& PassProfilePassProfile::GetEANBARCODE()
{
    return eANBARCODE_;
}

void PassProfilePassProfile::SetEANBARCODE(PassProfileEANBARCODE& eANBARCODE)
{
    eANBARCODE_ = eANBARCODE;
}

PassProfileFARMNOBARCODE& PassProfilePassProfile::GetFARMNOBARCODE()
{
    return fARMNOBARCODE_;
}

void PassProfilePassProfile::SetFARMNOBARCODE(PassProfileFARMNOBARCODE& fARMNOBARCODE)
{
    fARMNOBARCODE_ = fARMNOBARCODE;
}

PassProfileFIRSTOWNER& PassProfilePassProfile::GetFIRSTOWNER()
{
    return fIRSTOWNER_;
}

void PassProfilePassProfile::SetFIRSTOWNER(PassProfileFIRSTOWNER& fIRSTOWNER)
{
    fIRSTOWNER_ = fIRSTOWNER;
}

PassProfileMOTHEREAN& PassProfilePassProfile::GetMOTHEREAN()
{
    return mOTHEREAN_;
}

void PassProfilePassProfile::SetMOTHEREAN(PassProfileMOTHEREAN& mOTHEREAN)
{
    mOTHEREAN_ = mOTHEREAN;
}

PassProfilePASSDATE& PassProfilePassProfile::GetPASSDATE()
{
    return pASSDATE_;
}

void PassProfilePassProfile::SetPASSDATE(PassProfilePASSDATE& pASSDATE)
{
    pASSDATE_ = pASSDATE;
}

PassProfilePASSNO& PassProfilePassProfile::GetPASSNO()
{
    return pASSNO_;
}

void PassProfilePassProfile::SetPASSNO(PassProfilePASSNO& pASSNO)
{
    pASSNO_ = pASSNO;
}

long PassProfilePassProfile::GetPassDimX() const
{
    return passDimX_;
}

void PassProfilePassProfile::SetPassDimX(long passDimX)
{
    passDimX_ = passDimX;
}

long PassProfilePassProfile::GetPassDimY() const
{
    return passDimY_;
}

void PassProfilePassProfile::SetPassDimY(long passDimY)
{
    passDimY_ = passDimY;
}

cmString PassProfilePassProfile::GetPassProfileName() const
{
    return passProfileName_;
}

void PassProfilePassProfile::SetPassProfileName(const cmString& passProfileName)
{
    passProfileName_ = passProfileName;
}

PassProfileSEX& PassProfilePassProfile::GetSEX()
{
    return sEX_;
}

void PassProfilePassProfile::SetSEX(PassProfileSEX& sEX)
{
    sEX_ = sEX;
}

PassProfileSTOCK& PassProfilePassProfile::GetSTOCK()
{
    return sTOCK_;
}

void PassProfilePassProfile::SetSTOCK(PassProfileSTOCK& sTOCK)
{
    sTOCK_ = sTOCK;
}

bool PassProfilePassProfile::BuildFromXml(CComPtr<IXMLDOMNode>& node)
{
    cmString tmp;

    SetDefaults();

    ExtractMemberNode(node, _T("BIRTHINFO"), bIRTHINFO_);
    ExtractMemberNode(node, _T("EAN"), eAN_);
    ExtractMemberNode(node, _T("EANBARCODE"), eANBARCODE_);
    ExtractMemberNode(node, _T("FARMNOBARCODE"), fARMNOBARCODE_);
    ExtractMemberNode(node, _T("FIRSTOWNER"), fIRSTOWNER_);
    ExtractMemberNode(node, _T("MOTHEREAN"), mOTHEREAN_);
    ExtractMemberNode(node, _T("PASSDATE"), pASSDATE_);
    ExtractMemberNode(node, _T("PASSNO"), pASSNO_);
    ExtractAttributeNode(node, _T("PassDimX"), passDimX_);
    ExtractAttributeNode(node, _T("PassDimY"), passDimY_);
    ExtractAttributeNode(node, _T("PassProfileName"), passProfileName_);
    ExtractMemberNode(node, _T("SEX"), sEX_);
    ExtractMemberNode(node, _T("STOCK"), sTOCK_);

    return true;
}

bool PassProfilePassProfile::AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement)
{
    CComPtr<IXMLDOMElement> newElement;
    HRESULT hr;
    cmString tmp;

    hr = xmlDoc->createElement(CComBSTR((const TCHAR*)GetXmlNodeName()), &newElement);

    bIRTHINFO_.AppendToDOMDocument(xmlDoc, newElement);
    eAN_.AppendToDOMDocument(xmlDoc, newElement);
    eANBARCODE_.AppendToDOMDocument(xmlDoc, newElement);
    fARMNOBARCODE_.AppendToDOMDocument(xmlDoc, newElement);
    fIRSTOWNER_.AppendToDOMDocument(xmlDoc, newElement);
    mOTHEREAN_.AppendToDOMDocument(xmlDoc, newElement);
    pASSDATE_.AppendToDOMDocument(xmlDoc, newElement);
    pASSNO_.AppendToDOMDocument(xmlDoc, newElement);
    AppendAttributeNode(_T("PassDimX"), passDimX_, newElement, xmlDoc);
    AppendAttributeNode(_T("PassDimY"), passDimY_, newElement, xmlDoc);
    AppendAttributeNode(_T("PassProfileName"), passProfileName_, newElement, xmlDoc);
    sEX_.AppendToDOMDocument(xmlDoc, newElement);
    sTOCK_.AppendToDOMDocument(xmlDoc, newElement);

    if (parentElement)
        hr = parentElement->appendChild(newElement, NULL);
    else
        hr = xmlDoc->appendChild(newElement, NULL);

    return true;
}

