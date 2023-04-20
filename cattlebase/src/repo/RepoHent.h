#ifndef __REPOHENT_H__
#define __REPOHENT_H__

#include "RepoHentType.h"
#include "../classes/types/DateTime.h"
#include "../classes/types/Latitude.h"
#include "../classes/types/Longitude.h"
#include "../datatypes/Hent.h"

class RepoHent
{
public:
	RepoHent();
	RepoHent(const RepoHent& src);
	RepoHent(const Hent& srcHent);

	void CopyFrom(const RepoHent& src);
	void CopyFrom(const Hent& srcHent);


	const CString& GetName() const;
	void SetName(const CString& hentName);
	
	const CString& GetAlias() const;
	void SetAlias(const CString& hentAlias);
	
	const CString& GetStreet() const;
	void SetStreet(const CString& hentStreet);
		
	const CString& GetPOBox() const;
	void SetPOBox(const CString& hentPOBox);
	
	const CString& GetCity() const;
	void SetCity(const CString& hentCity);
	
	const CString& GetZip() const;
	void SetZip(const CString& hentZip);
	
	const CString& GetPhone() const;
	void SetPhone(const CString& hentPhone);
	
	const CString& GetNIP() const;
	void SetNIP(const CString& hentNIP);
	
	const CString& GetFarmNo() const;
	void SetFarmNo(const CString& hentFarmNo);
	
	const CString& GetWetNo() const;
	void SetWetNo(const CString& hentWetNo);
	
	const CString& GetExtras() const;
	void SetExtras(const CString& hentExtras);
		
	const CString& GetPlate() const;	
	void SetPlate(const CString& hentPlate);
	
	RepoHentType GetHentType() const;
	void SetHentType(RepoHentType hentType);
		
	const CString& GetPESEL() const;
	void SetPESEL(const CString& hentPESEL);
	
	const CString& GetREGON() const;
	void SetREGON(const CString& hentREGON);
	
	const CString& GetIdNo() const;
	void SetIdNo(const CString& hentIdNo);
	
	const CDateTime& GetIssueDate() const;
	void SetIssueDate(const CDateTime& hentIdIssueDate);
	
	const CString& GetIssuePost() const;
	void SetIssuePost(const CString& hentIssuePost);
		
	const CString& GetAccountNo() const;
	void SetAccountNo(const CString& hentAccountNo);
	
	const CString& GetBankName() const;
	void SetBankName(const CString& hentBankName);
	
	const CString& GetWetLicenceNo() const;
	void SetWetLicenceNo(const CString& sWetLicenceNo);
	
	const CString& GetCellPhoneNo() const;
	void SetCellPhoneNo(const CString& sCellPhoneNo);
	
	const CString& GetEmailAddress() const;
	void SetEmailAddress(const CString& sEmailAddress);
	
	const Latitude& GetLatitude() const;
	void SetLatitude(const Latitude& lati);
	
	const Longitude& GetLongitude() const;
	void SetLongitude(const Longitude& longi);

private:
	CString name;
	CString alias;
	CString street;
	CString pobox;
	CString city;
	CString zip;
	CString phone;
	CString nip;
	CString farmno;
	CString wetno;
	CString extras;
	CString plate;	
	RepoHentType henttype;
	
	CString pesel;
	CString regon;
	CString idno;
	CDateTime issuedate;
	CString issuepost;
	CString accountno;
	CString bankname;
	CString wetlicenceno;
	CString cellphoneno;
	CString emailaddress;
	Latitude latitude;
	Longitude longitude;
};
	

#endif