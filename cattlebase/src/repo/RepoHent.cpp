#include "stdafx.h"
#include "RepoHent.h"


RepoHent::RepoHent() : henttype(HentType_Individual)
{
}

RepoHent::RepoHent(const RepoHent& src)
{
	CopyFrom(src);
}

RepoHent::RepoHent(const Hent& srcHent)
{
	CopyFrom(srcHent);
}

void RepoHent::CopyFrom(const RepoHent& src)
{
	name = src.name;
	alias = src.alias;
	street = src.street;
	pobox = src.pobox;
	city = src.city;
	zip = src.zip;
	phone = src.phone;
	nip = src.nip;
	farmno = src.farmno;
	wetno = src.wetno;
	extras = src.extras;
	plate = src.plate;	
	henttype = src.henttype;	
	pesel = src.pesel;
	regon = src.regon;
	idno = src.idno;
	issuedate = src.issuedate;
	issuepost = src.issuepost;
	accountno = src.accountno;
	bankname = src.bankname;
	wetlicenceno = src.wetlicenceno;
	cellphoneno = src.cellphoneno;
	emailaddress = src.emailaddress;
	latitude = src.latitude;
	longitude = src.longitude;
}

void RepoHent::CopyFrom(const Hent& srcHent)
{
	name = srcHent.GetName();
	alias = srcHent.GetAlias();
	street = srcHent.GetStreet();
	pobox = srcHent.GetPOBox();
	city = srcHent.GetCity();
	zip = srcHent.GetZip();
	phone = srcHent.GetPhone();
	nip = srcHent.GetNIP();
	farmno = srcHent.GetFarmNo();
	wetno = srcHent.GetWetNo();
	extras = srcHent.GetExtras();
	plate = srcHent.GetPlate();

	HentType* pHentType = srcHent.GetHentType();

	henttype = HentType_Individual;
	if(pHentType->IsCompany())
	{
		henttype = HentType_Company;
	}
	
	pesel = srcHent.GetPESEL();
	regon = srcHent.GetREGON();
	idno = srcHent.GetIdNo();
	issuedate = srcHent.GetIssueDate();
	issuepost = srcHent.GetIssuePost();

	accountno = srcHent.GetAccountNo();
	if(!accountno.IsEmpty())
	{
		accountno.Insert(0,"PL");
	}
	
	bankname = srcHent.GetBankName();
	wetlicenceno = srcHent.GetWetLicenceNo();
	cellphoneno = srcHent.GetCellPhoneNo();
	emailaddress = srcHent.GetEmailAddress();
	latitude = srcHent.GetLatitude();
	longitude = srcHent.GetLongitude();
}


const CString& RepoHent::GetName() const
{
	return name;
}

void RepoHent::SetName(const CString& hentName)
{
	name = hentName;
}


const CString& RepoHent::GetAlias() const
{
	return alias;
}

void RepoHent::SetAlias(const CString& hentAlias)
{
	alias = hentAlias;
}

const CString& RepoHent::GetStreet() const
{
	return street;
}

void RepoHent::SetStreet(const CString& hentStreet)
{
	street = hentStreet;
}

const CString& RepoHent::GetPOBox() const
{
	return pobox;
}

void RepoHent::SetPOBox(const CString& hentPOBox)
{
	pobox = hentPOBox;
}

const CString& RepoHent::GetCity() const
{
	return city;
}

void RepoHent::SetCity(const CString& hentCity)
{
	city = hentCity;
}

const CString& RepoHent::GetZip() const
{
	return zip;
}

void RepoHent::SetZip(const CString& hentZip)
{
	zip = hentZip;
}

const CString& RepoHent::GetPhone() const
{
	return phone;
}

void RepoHent::SetPhone(const CString& hentPhone)
{
	phone = hentPhone;
}

const CString& RepoHent::GetNIP() const
{
	return nip;
}

void RepoHent::SetNIP(const CString& hentNIP)
{
		nip = hentNIP;
}

const CString& RepoHent::GetFarmNo() const
{
	return farmno;
}

void RepoHent::SetFarmNo(const CString& hentFarmNo)
{
	farmno = hentFarmNo;
}

const CString& RepoHent::GetWetNo() const
{
	return wetno;
}

void RepoHent::SetWetNo(const CString& hentWetNo)
{
	wetno = hentWetNo;
}

const CString& RepoHent::GetExtras() const
{
	return extras;
}

void RepoHent::SetExtras(const CString& hentExtras)
{
	extras = hentExtras;
}

const CString& RepoHent::GetPlate() const
{
	return plate;
}

void RepoHent::SetPlate(const CString& hentPlate)
{
	plate = hentPlate;
}

RepoHentType RepoHent::GetHentType() const
{
	return henttype;
}

void RepoHent::SetHentType(RepoHentType hentType)
{
	henttype = hentType;	
}

const CString& RepoHent::GetPESEL() const
{
	return pesel;
}

void RepoHent::SetPESEL(const CString& hentPESEL)
{
	pesel = hentPESEL;
}

const CString& RepoHent::GetREGON() const
{
	return regon;
}

void RepoHent::SetREGON(const CString& hentREGON)
{
	regon = hentREGON;
}

const CString& RepoHent::GetIdNo() const
{
	return idno;
}

void RepoHent::SetIdNo(const CString& hentIdNo)
{
	idno = hentIdNo;
}

const CDateTime& RepoHent::GetIssueDate() const
{
	return issuedate;
}

void RepoHent::SetIssueDate(const CDateTime& hentIdIssueDate)
{
	issuedate = hentIdIssueDate;
}

const CString& RepoHent::GetIssuePost() const
{
	return issuepost;
}

void RepoHent::SetIssuePost(const CString& hentIssuePost)
{
	issuepost = hentIssuePost;
}

const CString& RepoHent::GetAccountNo() const
{
	return accountno;
}

void RepoHent::SetAccountNo(const CString& hentAccountNo)
{
	accountno = hentAccountNo;
}

const CString& RepoHent::GetBankName() const
{
	return bankname;
}

void RepoHent::SetBankName(const CString& hentBankName)
{
	bankname = hentBankName;
}

const CString& RepoHent::GetWetLicenceNo() const
{
	return wetlicenceno;
}

void RepoHent::SetWetLicenceNo(const CString& sWetLicenceNo)
{
	wetlicenceno = sWetLicenceNo;
}

const CString& RepoHent::GetCellPhoneNo() const
{
	return cellphoneno;
}

void RepoHent::SetCellPhoneNo(const CString& sCellPhoneNo)
{
	cellphoneno = sCellPhoneNo;
}

const CString& RepoHent::GetEmailAddress() const
{
	return emailaddress;
}

void RepoHent::SetEmailAddress(const CString& sEmailAddress)
{
	emailaddress = sEmailAddress;
}

const Latitude& RepoHent::GetLatitude() const
{
	return latitude;
}

void RepoHent::SetLatitude(const Latitude& lati)
{
	latitude = lati;

}

const Longitude& RepoHent::GetLongitude() const
{
	return longitude;
}

void RepoHent::SetLongitude(const Longitude& longi)
{
	longitude = longi;

}