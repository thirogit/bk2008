#include "stdafx.h"
#include "NewHentSession.h"
	
NewHentSession::NewHentSession(ContextTransactionImpl* pParentTransaction) : m_pParentTransaction(pParentTransaction)
{

}

void NewHentSession::SetName(const CString& hentName)
{
	name.SetTo(hentName);
}

void NewHentSession::SetAlias(const CString& hentAlias)
{
	alias.SetTo(hentAlias);
}

void NewHentSession::SetStreet(const CString& hentStreet)
{
	street.SetTo(hentStreet);
}

void NewHentSession::SetPOBox(const CString& hentPOBox)
{
	pobox.SetTo(hentPOBox);
}

void NewHentSession::SetCity(const CString& hentCity)
{
	city.SetTo(hentCity);
}

void NewHentSession::SetZip(const CString& hentZip)
{
	zip.SetTo(hentZip);
}

void NewHentSession::SetPhone(const CString& hentPhone)
{
	phone.SetTo(hentPhone);
}

void NewHentSession::SetNIP(const CString& hentNIP)
{
	nip.SetTo(hentNIP);
}

void NewHentSession::SetFarmNo(const CString& hentFarmNo)
{
	arimrno.SetTo(hentFarmNo);
}

void NewHentSession::SetWetNo(const CString& hentWetNo)
{
	wetno.SetTo(hentWetNo);
}

void NewHentSession::SetExtras(const CString& hentExtras)
{
	extras.SetTo(hentExtras);
}

void NewHentSession::SetCountry(Country* hentCountry)
{
	country.SetTo(hentCountry);
}

void NewHentSession::SetPlate(const CString& hentPlate)
{
	plate.SetTo(hentPlate);
}

void NewHentSession::SetHentType(HentType* hentHentType)
{
	henttype.SetTo(hentHentType);
}

void NewHentSession::SetSyncIt(bool hentSyncIt)
{
	syncit.SetTo(hentSyncIt);
}

void NewHentSession::SetPESEL(const CString& hentPESEL)
{
	pesel.SetTo(hentPESEL);
}

void NewHentSession::SetREGON(const CString& hentREGON)
{
	regon.SetTo(hentREGON);
}

void NewHentSession::SetIdNo(const CString& hentIdNo)
{
	idno.SetTo(hentIdNo);
}

void NewHentSession::SetIssueDate(const CDateTime& hentIdIssueDate)
{
	issuedate.SetTo(hentIdIssueDate);
}
	
void NewHentSession::SetIssuePost(const CString& hentIssuePost)
{
	issuepost.SetTo(hentIssuePost);
}

void NewHentSession::SetAccountNo(const CString& hentAccountNo)
{
	accountno.SetTo(hentAccountNo);
}

void NewHentSession::SetBankName(const CString& hentBankName)
{
	bankname.SetTo(hentBankName);
}

void NewHentSession::SetWetLicenceNo(const CString& sWetLicenceNo)
{
	wetlicenceno.SetTo(sWetLicenceNo);
}

void NewHentSession::SetCellPhoneNo(const CString& sCellPhoneNo)
{
	cellphoneno.SetTo(sCellPhoneNo);
}

void NewHentSession::SetEmailAddress(const CString& sEmailAddress)
{
	emailaddress.SetTo(sEmailAddress);
}

void NewHentSession::SetLatitude(const Latitude& lati)
{
	latitude.SetTo(lati);
}

void NewHentSession::SetLongitude(const Longitude& longi)
{
	longitude.SetTo(longi);
}

	
UINT NewHentSession::GetId() const
{
	return NULL_HENTID;
}


CString NewHentSession::GetName() const
{
	return name.GetValue();
}

CString NewHentSession::GetAlias() const
{
	return alias.GetValue();
}

CString NewHentSession::GetStreet() const
{
	return street.GetValue();
}

CString NewHentSession::GetPOBox() const
{
	return pobox.GetValue();
}

CString NewHentSession::GetCity() const
{
	return city.GetValue();
}

CString NewHentSession::GetZip() const
{
	return zip.GetValue();
}

CString NewHentSession::GetPhone() const
{
	return phone.GetValue();
}

CString NewHentSession::GetNIP() const
{
	return nip.GetValue();
}

CString NewHentSession::GetFarmNo() const
{
	return arimrno.GetValue();
}

CString NewHentSession::GetWetNo() const
{
	return wetno.GetValue();
}

CString NewHentSession::GetExtras() const
{
	return extras.GetValue();
}

Country* NewHentSession::GetCountry() const
{
	return country.GetValue();
}

CString NewHentSession::GetPlate() const
{
	return plate.GetValue();
}
 
HentType* NewHentSession::GetHentType() const
{
	return henttype.GetValue();
}

bool NewHentSession::GetSyncIt() const
{
	return syncit.GetValue();
}

CString NewHentSession::GetPESEL() const
{
	return pesel.GetValue();
}

CString NewHentSession::GetREGON() const
{
	return regon.GetValue();
}

CString NewHentSession::GetIdNo() const
{
	return idno.GetValue();
}

CDateTime NewHentSession::GetIssueDate() const
{
	return issuedate.GetValue();
}

CString NewHentSession::GetIssuePost() const
{
	return issuepost.GetValue();
}

CString NewHentSession::GetAccountNo() const
{
	return accountno.GetValue();
}

CString NewHentSession::GetBankName() const
{
	return bankname.GetValue();
}

CString NewHentSession::GetWetLicenceNo() const
{
	return wetlicenceno.GetValue();
}

CString NewHentSession::GetCellPhoneNo() const
{
	return cellphoneno.GetValue();
}

CString NewHentSession::GetEmailAddress() const
{
	return emailaddress.GetValue();
}

Latitude NewHentSession::GetLatitude() const
{
	return latitude.GetValue();
}

Longitude NewHentSession::GetLongitude() const
{
	return longitude.GetValue();
}
		
void NewHentSession::Discard()
{
}


