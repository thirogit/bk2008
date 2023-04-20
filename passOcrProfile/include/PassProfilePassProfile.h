// PassProfilePassProfile.h
// 
// generated on 2009.12.25 at 14:37:59.159
//
#if !defined(_PASSPROFILEPASSPROFILE_H_)
#define _PASSPROFILEPASSPROFILE_H_

#include <string>
#include "XmlSerializable.h"
#include "cmString.h"
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

using namespace std;

class PassProfileBIRTHINFO;
class PassProfileEAN;
class PassProfileEANBARCODE;
class PassProfileFARMNOBARCODE;
class PassProfileFIRSTOWNER;
class PassProfileMOTHEREAN;
class PassProfilePASSDATE;
class PassProfilePASSNO;
class PassProfileSEX;
class PassProfileSTOCK;

class AFX_EXT_CLASS PassProfilePassProfile: public CXmlSerializable
{
public:
    PassProfilePassProfile();
    PassProfilePassProfile(const PassProfilePassProfile& copy);
    virtual ~PassProfilePassProfile();














    // utils
    //
public:
    void operator=(const PassProfilePassProfile& copy);

    void SetDefaults();
    void Clear();
    virtual cmString GetXmlNodeName(){return _T("PassProfile");}

protected:
    void Copy(const PassProfilePassProfile& copy);

    // serialization
    //
    public:
    virtual bool BuildFromXml(CComPtr<IXMLDOMNode>& node);
    virtual bool AppendToDOMDocument(CComPtr<IXMLDOMDocument2>& xmlDoc, CComPtr<IXMLDOMElement>& parentElement);

    // Get/Set methods
    //
public:

    PassProfileBIRTHINFO& GetBIRTHINFO();
    void SetBIRTHINFO(PassProfileBIRTHINFO& BIRTHINFO);

    PassProfileEAN& GetEAN();
    void SetEAN(PassProfileEAN& EAN);

    PassProfileEANBARCODE& GetEANBARCODE();
    void SetEANBARCODE(PassProfileEANBARCODE& EANBARCODE);

    PassProfileFARMNOBARCODE& GetFARMNOBARCODE();
    void SetFARMNOBARCODE(PassProfileFARMNOBARCODE& FARMNOBARCODE);

    PassProfileFIRSTOWNER& GetFIRSTOWNER();
    void SetFIRSTOWNER(PassProfileFIRSTOWNER& FIRSTOWNER);

    PassProfileMOTHEREAN& GetMOTHEREAN();
    void SetMOTHEREAN(PassProfileMOTHEREAN& MOTHEREAN);

    PassProfilePASSDATE& GetPASSDATE();
    void SetPASSDATE(PassProfilePASSDATE& PASSDATE);

    PassProfilePASSNO& GetPASSNO();
    void SetPASSNO(PassProfilePASSNO& PASSNO);

    long GetPassDimX() const;
    void SetPassDimX(long PassDimX);

    long GetPassDimY() const;
    void SetPassDimY(long PassDimY);

    cmString GetPassProfileName() const;
    void SetPassProfileName(const cmString& PassProfileName);

    PassProfileSEX& GetSEX();
    void SetSEX(PassProfileSEX& SEX);

    PassProfileSTOCK& GetSTOCK();
    void SetSTOCK(PassProfileSTOCK& STOCK);

    // inner members
    //
protected:
    PassProfileBIRTHINFO bIRTHINFO_;
    PassProfileEAN eAN_;
    PassProfileEANBARCODE eANBARCODE_;
    PassProfileFARMNOBARCODE fARMNOBARCODE_;
    PassProfileFIRSTOWNER fIRSTOWNER_;
    PassProfileMOTHEREAN mOTHEREAN_;
    PassProfilePASSDATE pASSDATE_;
    PassProfilePASSNO pASSNO_;
    long passDimX_;
    long passDimY_;
    cmString passProfileName_;
    PassProfileSEX sEX_;
    PassProfileSTOCK sTOCK_;

};

#endif //!defined(_PASSPROFILEPASSPROFILE_H_)

