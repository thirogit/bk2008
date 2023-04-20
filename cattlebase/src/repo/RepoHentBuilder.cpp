#include "stdafx.h"
#include "RepoHentBuilder.h"
#include "RepoException.h"

RepoHentBuilder::RepoHentBuilder() : m_HentType(HentType_Individual)
{

}

const CString& RepoHentBuilder::GetName() const
{
	return m_Name;
}

void RepoHentBuilder::SetName(const CString& sHentName)
{
	m_Name = sHentName;
}

const CString& RepoHentBuilder::GetAlias() const
{
	return m_Alias;
}

void RepoHentBuilder::SetAlias(const CString& sHentAlias)
{
	m_Alias = sHentAlias;
}

const CString& RepoHentBuilder::GetStreet() const
{
	return m_Street;
}

void RepoHentBuilder::SetStreet(const CString& sHentStreet)
{
	m_Street = sHentStreet;
}
	
const CString& RepoHentBuilder::GetPOBox() const
{
	return m_POBox;
}

void RepoHentBuilder::SetPOBox(const CString& sHentPOBox)
{
	m_POBox = sHentPOBox;
}

const CString& RepoHentBuilder::GetCity() const
{
	return m_City;
}

void RepoHentBuilder::SetCity(const CString& sHentCity)
{
	m_City = sHentCity;
}

const CString& RepoHentBuilder::GetZip() const
{
	return m_Zip;
}
void RepoHentBuilder::SetZip(const CString& sHentZip)
{
	m_Zip = sHentZip;
}

const CString& RepoHentBuilder::GetPhone() const
{
	return m_Phone;
}

void RepoHentBuilder::SetPhone(const CString& sHentPhone)
{
	m_Phone = sHentPhone;
}

const CString& RepoHentBuilder::GetNIP() const
{
	return m_NIP;
}

void RepoHentBuilder::SetNIP(const CString& sHentNIP)
{
	m_NIP = sHentNIP;
}

const CString& RepoHentBuilder::GetFarmNo() const
{
	return m_FarmNo;
}

void RepoHentBuilder::SetFarmNo(const CString& sHentFarmNo)
{
	m_FarmNo = sHentFarmNo;
}

const CString& RepoHentBuilder::GetWetNo() const
{
	return m_WetNo;
}
void RepoHentBuilder::SetWetNo(const CString& sHentWetNo)
{
	m_WetNo = sHentWetNo;
}

const CString& RepoHentBuilder::GetExtras() const
{
	return m_Extras;
}

void RepoHentBuilder::SetExtras(const CString& sHentExtras)
{
	m_Extras = sHentExtras;
}
	
const CString& RepoHentBuilder::GetPlate() const
{
	return m_PlateNo;
}

void RepoHentBuilder::SetPlate(const CString& sHentPlate)
{
	m_PlateNo = sHentPlate;
}

RepoHentType RepoHentBuilder::GetHentType() const
{
	return m_HentType;
}

void RepoHentBuilder::SetHentType(RepoHentType hentType)
{
	m_HentType = hentType;
}

const CString& RepoHentBuilder::GetPESEL() const
{
	return m_PESEL;
}

void RepoHentBuilder::SetPESEL(const CString& sHentPESEL)
{
	m_PESEL = sHentPESEL;
}

const CString& RepoHentBuilder::GetREGON() const
{
	return m_REGON;
}

void RepoHentBuilder::SetREGON(const CString& sHentREGON)
{
	m_REGON = sHentREGON;
}

const CString& RepoHentBuilder::GetIdNo() const
{
	return m_IdNo;
}

void RepoHentBuilder::SetIdNo(const CString& sHentIdNo)
{
	m_IdNo = sHentIdNo;
}

const CDateTime& RepoHentBuilder::GetIssueDate() const
{
	return m_IssueDate;
}

void RepoHentBuilder::SetIssueDate(const CDateTime& hentIdIssueDt)
{
	m_IssueDate = hentIdIssueDt;
}

const CString& RepoHentBuilder::GetIssuePost() const
{
	return m_IssuePost;
}

void RepoHentBuilder::SetIssuePost(const CString& sHentIssuePost)
{
	m_IssuePost = sHentIssuePost;
}

const CString& RepoHentBuilder::GetAccountNo() const
{
	return m_AccountNo;
}

void RepoHentBuilder::SetAccountNo(const CString& sHentAccountNo)
{
	m_AccountNo = sHentAccountNo;
}

const CString& RepoHentBuilder::GetBankName() const
{
	return m_BankName;
}

void RepoHentBuilder::SetBankName(const CString& sHentBankName)
{
	m_BankName = sHentBankName;
}

const CString& RepoHentBuilder::GetWetLicenceNo() const
{
	return m_WetLicenceNo;
}

void RepoHentBuilder::SetWetLicenceNo(const CString& sWetLicenceNo)
{
	m_WetLicenceNo = sWetLicenceNo;
}

const CString& RepoHentBuilder::GetCellPhoneNo() const
{
	return m_CellPhoneNo;
}

void RepoHentBuilder::SetCellPhoneNo(const CString& sCellPhoneNo)
{
	m_CellPhoneNo = sCellPhoneNo;
}

const CString& RepoHentBuilder::GetEmailAddress() const
{
	return m_EmailAddress;
}

void RepoHentBuilder::SetEmailAddress(const CString& sEmailAddress)
{
	m_EmailAddress = sEmailAddress;
}

const Latitude& RepoHentBuilder::GetLatitude() const
{
	return m_Latitude;
}

void RepoHentBuilder::SetLatitude(const Latitude& lati)
{
	m_Latitude = lati;
}

const Longitude& RepoHentBuilder::GetLongitude() const
{
	return m_Longitude;
}

void RepoHentBuilder::SetLongitude(const Longitude& longi)
{
	m_Longitude = longi;
}

RepoHent* RepoHentBuilder::Build()
{
	RepoHent* pNewRepoHent = new RepoHent();

	if(m_FarmNo.IsEmpty())
		throw new RepoException("hent's farm no is empty");

	if(m_Name.IsEmpty())
		throw new RepoException("hent's name is empty");

	if(m_Alias.IsEmpty())
		throw new RepoException("hent's alias is empty");

	if(m_Street.IsEmpty())
		throw new RepoException("hent's street name is empty");

	if(m_POBox.IsEmpty())
		throw new RepoException("hent's pobox is empty");

	if(m_City.IsEmpty())
		throw new RepoException("hent's city name is empty");

	if(m_Zip.IsEmpty())
		throw new RepoException("hent's zip is empty");
	
	pNewRepoHent->SetName(m_Name);
	pNewRepoHent->SetAlias(m_Alias);
	pNewRepoHent->SetStreet(m_Street);
	pNewRepoHent->SetPOBox(m_POBox);
	pNewRepoHent->SetCity(m_City);
	pNewRepoHent->SetZip(m_Zip);
	pNewRepoHent->SetPhone(m_Phone);
	pNewRepoHent->SetNIP(m_NIP);
	pNewRepoHent->SetFarmNo(m_FarmNo);
	pNewRepoHent->SetWetNo(m_WetNo);
	pNewRepoHent->SetExtras(m_Extras);
	pNewRepoHent->SetPlate(m_PlateNo);
	pNewRepoHent->SetHentType(m_HentType);
	pNewRepoHent->SetPESEL(m_PESEL);
	pNewRepoHent->SetREGON(m_REGON);
	pNewRepoHent->SetIdNo(m_IdNo);
	pNewRepoHent->SetIssueDate(m_IssueDate);
	pNewRepoHent->SetIssuePost(m_IssuePost);
	pNewRepoHent->SetAccountNo(m_AccountNo);
	pNewRepoHent->SetBankName(m_BankName);
	pNewRepoHent->SetWetLicenceNo(m_WetLicenceNo);
	pNewRepoHent->SetCellPhoneNo(m_CellPhoneNo);
	pNewRepoHent->SetEmailAddress(m_EmailAddress);
	pNewRepoHent->SetLatitude(m_Latitude);
	pNewRepoHent->SetLongitude(m_Longitude);

	return pNewRepoHent;
}