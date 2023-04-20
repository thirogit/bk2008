#ifndef __NEWHENTSESSION_H__
#define __NEWHENTSESSION_H__

#include "../HentSession.h"
#include "../../../datatypes/Hent.h"
#include "../../../dao/db/Transaction.h"
#include "ContextTransactionImpl.h"

class NewHentSession : public HentSession
{
public:	

	NewHentSession(ContextTransactionImpl* pParentTransaction);
		
	virtual void SetName(const CString& hentName);
	virtual void SetAlias(const CString& hentAlias);
	virtual void SetStreet(const CString& hentStreet);
	virtual void SetPOBox(const CString& hentPOBox);
	virtual void SetCity(const CString& hentCity);
	virtual void SetZip(const CString& hentZip);
	virtual void SetPhone(const CString& hentPhone);
	virtual void SetNIP(const CString& hentNIP);
	virtual void SetFarmNo(const CString& hentFarmNo);
	virtual void SetWetNo(const CString& hentWetNo);
	virtual void SetExtras(const CString& hentExtras);
	virtual void SetCountry(Country* hentCountry);
	virtual void SetPlate(const CString& hentPlate);
	virtual void SetHentType(HentType* hentHentType);
	virtual void SetSyncIt(bool hentSyncIt);
	virtual void SetPESEL(const CString& hentPESEL);
	virtual void SetREGON(const CString& hentREGON);
	virtual void SetIdNo(const CString& hentIdNo);
	virtual void SetIssueDate(const CDateTime& hentIdIssueDate);
	virtual void SetIssuePost(const CString& hentIssuePost);
	virtual void SetAccountNo(const CString& hentAccountNo);
	virtual void SetBankName(const CString& hentBankName);
	virtual void SetWetLicenceNo(const CString& sWetLicenceNo);
	virtual void SetCellPhoneNo(const CString& sCellPhoneNo);
	virtual void SetEmailAddress(const CString& sEmailAddress);
	virtual void SetLatitude(const Latitude& lati);
	virtual void SetLongitude(const Longitude& longi);
	
	virtual UINT GetId() const;
	virtual CString GetName() const;
	virtual CString GetAlias() const;
	virtual CString GetStreet() const;
	virtual CString GetPOBox() const;
	virtual CString GetCity() const;
	virtual CString GetZip() const;
	virtual CString GetPhone() const;
	virtual CString GetNIP() const;
	virtual CString GetFarmNo() const;
	virtual CString GetWetNo() const;
	virtual CString GetExtras() const;
	virtual Country* GetCountry() const;
	virtual CString GetPlate() const; 
	virtual HentType* GetHentType() const;
	virtual bool GetSyncIt() const;
	virtual CString GetPESEL() const;
	virtual CString GetREGON() const;
	virtual CString GetIdNo() const;
	virtual CDateTime GetIssueDate() const;
	virtual CString GetIssuePost() const;
	virtual CString GetAccountNo() const;
	virtual CString GetBankName() const;
	virtual CString GetWetLicenceNo() const;
	virtual CString GetCellPhoneNo() const;
	virtual CString GetEmailAddress() const;
	virtual Latitude GetLatitude() const;
	virtual Longitude GetLongitude() const;
		
	virtual void Discard();	

	void Commit(Transaction transaction);

private:
	StringDataMember name;
	StringDataMember alias;
	StringDataMember street;
	StringDataMember pobox;
	StringDataMember city;
	StringDataMember zip;
	StringDataMember phone;
	StringDataMember nip;
	StringDataMember arimrno;
	StringDataMember wetno;
	StringDataMember extras;
	CountryDataMember country;
	StringDataMember plate;	
	HentTypeDataMember henttype;
	BooleanDataMember syncit;
	StringDataMember pesel;
	StringDataMember regon;
	StringDataMember idno;
	DateTimeDataMember issuedate;
	StringDataMember issuepost;
	StringDataMember accountno;
	StringDataMember bankname;
	StringDataMember wetlicenceno;
	StringDataMember cellphoneno;
	StringDataMember emailaddress;
	LatitudeDataMember latitude;
	LongitudeDataMember longitude;

	ContextTransactionImpl* m_pParentTransaction;

	

};
	

#endif