#ifndef __REPOHENTBUILDER_H__
#define __REPOHENTBUILDER_H__

#include "../classes/types/DateTime.h"
#include "../classes/types/Latitude.h"
#include "../classes/types/Longitude.h"
#include "RepoHent.h"

class RepoHentBuilder
{
public:
	RepoHentBuilder();

	const CString& GetName() const;
	void SetName(const CString& sHentName);
	
	const CString& GetAlias() const;
	void SetAlias(const CString& sHentAlias);
	
	const CString& GetStreet() const;
	void SetStreet(const CString& sHentStreet);
		
	const CString& GetPOBox() const;
	void SetPOBox(const CString& sHentPOBox);
	
	const CString& GetCity() const;
	void SetCity(const CString& sHentCity);
	
	const CString& GetZip() const;
	void SetZip(const CString& sHentZip);
	
	const CString& GetPhone() const;
	void SetPhone(const CString& sHentPhone);
	
	const CString& GetNIP() const;
	void SetNIP(const CString& sHentNIP);
	
	const CString& GetFarmNo() const;
	void SetFarmNo(const CString& sHentFarmNo);
	
	const CString& GetWetNo() const;
	void SetWetNo(const CString& sHentWetNo);
	
	const CString& GetExtras() const;
	void SetExtras(const CString& sHentExtras);
		
	const CString& GetPlate() const;	
	void SetPlate(const CString& sHentPlate);
	
	RepoHentType GetHentType() const;
	void SetHentType(RepoHentType hentType);
		
	const CString& GetPESEL() const;
	void SetPESEL(const CString& sHentPESEL);
	
	const CString& GetREGON() const;
	void SetREGON(const CString& sHentREGON);
	
	const CString& GetIdNo() const;
	void SetIdNo(const CString& sHentIdNo);
	
	const CDateTime& GetIssueDate() const;
	void SetIssueDate(const CDateTime& hentIdIssueDt);
	
	const CString& GetIssuePost() const;
	void SetIssuePost(const CString& sHentIssuePost);
		
	const CString& GetAccountNo() const;
	void SetAccountNo(const CString& sHentAccountNo);
	
	const CString& GetBankName() const;
	void SetBankName(const CString& sHentBankName);
	
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

	RepoHent* Build();

private:
	
	CString m_Name;
	CString m_Alias;
	CString m_Street;
	CString m_POBox;
	CString m_City;
	CString m_Zip;
	CString m_Phone;
	CString m_NIP;
	CString m_FarmNo;
	CString m_WetNo;
	CString m_Extras;
	CString m_PlateNo;	
	RepoHentType m_HentType;
	
	CString m_PESEL;
	CString m_REGON;
	CString m_IdNo;
	CDateTime m_IssueDate;
	CString m_IssuePost;
	CString m_AccountNo;
	CString m_BankName;
	CString m_WetLicenceNo;
	CString m_CellPhoneNo;
	CString m_EmailAddress;
	Latitude m_Latitude;
	Longitude m_Longitude;


	

};
	

#endif