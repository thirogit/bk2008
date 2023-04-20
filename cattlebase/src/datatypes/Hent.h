#ifndef __HENT_H__
#define __HENT_H__

#include "RawData.h"
#include "Country.h"
#include "HentType.h"
#include "../classes/types/Latitude.h"
#include "../classes/types/Longitude.h"
#include "DataMember.h"
#include "DataMembersDefs.h"

#define NULL_HENTID (0)

typedef DataMember<HentType*,HentType*> HentTypeDataMember;
typedef DataMember<Country*,Country*> CountryDataMember;


class Hent : public RawData
{
public:

	Hent(UINT hentId);
	~Hent();
	void CopyFrom(const Hent& copy);
	
	UINT GetId() const;
	void ResetId();
	
	CString GetName() const;
	void SetName(const CString& hentName);
	void ChangeName(const CString& hentName);

	CString GetAlias() const;
	void SetAlias(const CString& hentAlias);
	void ChangeAlias(const CString& hentAlias);

	CString GetStreet() const;
	void SetStreet(const CString& hentStreet);
	void ChangeStreet(const CString& hentStreet);
	
	CString GetPOBox() const;
	void SetPOBox(const CString& hentPOBox);
	void ChangePOBox(const CString& hentPOBox);

	CString GetCity() const;
	void SetCity(const CString& hentCity);
	void ChangeCity(const CString& hentCity);

	CString GetZip() const;
	void SetZip(const CString& hentZip);
	void ChangeZip(const CString& hentZip);

	CString GetPhone() const;
	void SetPhone(const CString& hentPhone);
	void ChangePhone(const CString& hentPhone);

	CString GetNIP() const;
	void SetNIP(const CString& hentNIP);
	void ChangeNIP(const CString& hentNIP);
	bool HasNIP() const;

	CString GetFarmNo() const;
	void SetFarmNo(const CString& hentFarmNo);
	void ChangeFarmNo(const CString& hentFarmNo);

	CString GetWetNo() const;
	void SetWetNo(const CString& hentWetNo);
	void ChangeWetNo(const CString& hentWetNo);
	bool HasWetNo() const;

	CString GetExtras() const;
	void SetExtras(const CString& hentExtras);
	void ChangeExtras(const CString& hentExtras);

	Country* GetCountry() const;
	void SetCountry(Country* hentCountry);
	void ChangeCountry(Country* hentCountry);
	bool IsFromPoland() const;

	CString GetPlate() const;	
	void SetPlate(const CString& hentPlate);
	void ChangePlate(const CString& hentPlate);

	HentType* GetHentType() const;
	void SetHentType(HentType* hentHentType);
	void ChangeHentType(HentType* hentHentType);

	bool GetSyncIt() const;
	void SetSyncIt(bool hentSyncIt);
	void ChangeSyncIt(bool hentSyncIt);
	
	CString GetPESEL() const;
	void SetPESEL(const CString& hentPESEL);
	void ChangePESEL(const CString& hentPESEL);
	bool HasPESEL() const;
	
	CString GetREGON() const;
	void SetREGON(const CString& hentREGON);
	void ChangeREGON(const CString& hentREGON);
	bool HasREGON() const;
	
	CString GetIdNo() const;
	void SetIdNo(const CString& hentIdNo);
	void ChangeIdNo(const CString& hentIdNo);
	
	CDateTime GetIssueDate() const;
	void SetIssueDate(const CDateTime& hentIdIssueDate);
	void ChangeIssueDate(const CDateTime& hentIdIssueDate);
	
	CString GetIssuePost() const;
	void SetIssuePost(const CString& hentIssuePost);
	void ChangeIssuePost(const CString& hentIssuePost);

	//bool HasPersonalIdInformation() const;
	
	CString GetAccountNo() const;
	void SetAccountNo(const CString& hentAccountNo);
	void ChangeAccountNo(const CString& hentAccountNo);
	
	CString GetBankName() const;
	void SetBankName(const CString& hentBankName);
	void ChangeBankName(const CString& hentBankName);

	CString GetWetLicenceNo() const;
	void SetWetLicenceNo(const CString& sWetLicenceNo);
	void ChangeWetLicenceNo(const CString& sWetLicenceNo);
	bool HasWetLicenceNo() const;

	CString GetCellPhoneNo() const;
	void SetCellPhoneNo(const CString& sCellPhoneNo);
	void ChangeCellPhoneNo(const CString& sCellPhoneNo);

	CString GetEmailAddress() const;
	void SetEmailAddress(const CString& sEmailAddress);
	void ChangeEmailAddress(const CString& sEmailAddress);

	Latitude GetLatitude() const;
	void SetLatitude(const Latitude& lati);
	void ChangeLatitude(const Latitude& lati);

	Longitude GetLongitude() const;
	void SetLongitude(const Longitude& longi);
	void ChangeLongitude(const Longitude& longi);

	CString GetComboItemText() const;

	bool InsertThisHent(bool bCommit = true);
	bool UpdateThisHent(bool bCommit = true);
	bool DeleteThisHent(bool bCommit = true);	
	bool CanEditThisHent();
	bool CanDeleteThisHent();

	void RollbackChanges();
	void CommitChanges();
	bool IsUncommited();

protected:
	UINT hentid;

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

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;
};


#endif