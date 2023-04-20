#include "stdafx.h"
#include "CompanyInfo.h"
#include "../global_arrays.h"
#include "../CattleSQL.h"

Company Company::m_Company;

Company::Company() : m_lock(&m_cs,FALSE),m_lockingThread(NULL),m_ProxyForChanging(this),m_CompanyHent(NULL_HENTID),m_ProxyForInitialisation(this)
{
}

CompanyForChanging* Company::GetCompanyForChanging()
{
	if(m_Company.IsLockedByCurrentThread())
		return &m_Company.m_ProxyForChanging;

	return NULL;
}

bool Company::IsLockedByCurrentThread()
{
	return m_lock.IsLocked() && m_lockingThread != NULL && (m_lockingThread == GetCurrentThread());
}

CompanyForInitialisation* Company::GetCompanyForInitialisation()
{
	if(m_Company.IsLockedByCurrentThread())
		return &m_Company.m_ProxyForInitialisation;
	return NULL;
	
}

const Company* Company::GetCompany()
{	
	return &m_Company;
}

void Company::Lock()
{	
	m_Company.m_lock.Lock();
	m_Company.m_lockingThread = GetCurrentThread();
}
void Company::Unlock()
{
	if(m_Company.m_lockingThread == GetCurrentThread())
	{
		m_Company.m_lock.Unlock();
		m_Company.m_lockingThread = NULL;
		m_Company.CreateCompanyHent();
	}
}

void Company::CreateCompanyHent()
{
	m_CompanyHent.SetName(GetCompanyName());
	m_CompanyHent.SetCity(GetCity());
	m_CompanyHent.SetZip(GetZipCode());
	m_CompanyHent.SetStreet(GetStreet());
	m_CompanyHent.SetPOBox(GetPOBox());
	m_CompanyHent.SetFarmNo(GetFarmNo());
	m_CompanyHent.SetNIP(GetNIP());
	m_CompanyHent.SetREGON(GetREGON());
	m_CompanyHent.SetBankName(GetBank());
	m_CompanyHent.SetAccountNo(GetAccountNo());
	m_CompanyHent.SetWetNo(GetWetIdNo());
	m_CompanyHent.SetWetLicenceNo(GetWetLicNo());
	m_CompanyHent.SetPhone(GetPhoneNo());

	CString sCountryCode = GetFarmNo().Left(2);
	m_CompanyHent.SetCountry(ary_countries.GetCountryByCountryCode(sCountryCode));
	m_CompanyHent.SetHentType(ary_htypes.GetHentTypeByHentTypeId(HTYPE_COMPANY));

}
const CString& Company::GetCompanyName() const
{
	return m_CompanyName;	
}

const CString& Company::GetCity() const
{
	return m_City;	
}

const CString& Company::GetZipCode() const
{
	return m_ZipCode;	
}

const CString& Company::GetStreet() const
{
	return m_Street;	
}

const CString& Company::GetPOBox() const
{
	return m_POBox;	
}

const CString& Company::GetFarmNo() const
{
	return m_FarmNo;		
}
const CString& Company::GetNIP() const
{
	return m_NIP;	
}

const CString& Company::GetREGON() const
{
	return m_REGON;	
}

const CString& Company::GetBank() const
{
	return m_Bank;	
}

const CString& Company::GetAccountNo() const
{
	return m_AccountNo;	
}

const CString& Company::GetWetIdNo() const
{
	return m_WetIdNo;	
}

const CString& Company::GetWetLicNo() const
{
	return m_WetLicNo;	
}

const CString& Company::GetPhoneNo() const
{
	return m_PhoneNo;
}

const Hent*    Company::GetAsHent() const
{
	return &m_CompanyHent;
}

/////////////////////////////////////////////////////////////////////////////////////////

Company::CompanyForInitialisationImpl::CompanyForInitialisationImpl(Company* pCompany)
{
	m_pCompany = pCompany;
}

const CString& Company::CompanyForInitialisationImpl::GetCompanyName() const
{
	return m_pCompany->GetCompanyName();
}

const CString& Company::CompanyForInitialisationImpl::GetCity() const
{
	return m_pCompany->GetCity();
}

const CString& Company::CompanyForInitialisationImpl::GetZipCode() const
{
	return m_pCompany->GetZipCode();
}

const CString& Company::CompanyForInitialisationImpl::GetStreet() const
{
	return m_pCompany->GetStreet();
}

const CString& Company::CompanyForInitialisationImpl::GetPOBox() const
{
	return m_pCompany->GetPOBox();
}

const CString& Company::CompanyForInitialisationImpl::GetFarmNo() const
{
	return m_pCompany->GetFarmNo();
}
	
const CString& Company::CompanyForInitialisationImpl::GetNIP() const
{
	return m_pCompany->GetNIP();
}

const CString& Company::CompanyForInitialisationImpl::GetREGON() const
{
	return m_pCompany->GetREGON();
}

const CString& Company::CompanyForInitialisationImpl::GetBank() const
{
	return m_pCompany->GetBank();
}

const CString& Company::CompanyForInitialisationImpl::GetAccountNo() const
{
	return m_pCompany->GetAccountNo();
}

const CString& Company::CompanyForInitialisationImpl::GetWetIdNo() const
{
	return m_pCompany->GetWetIdNo();
}

const CString& Company::CompanyForInitialisationImpl::GetWetLicNo() const
{
	return m_pCompany->GetWetLicNo();
}

const CString& Company::CompanyForInitialisationImpl::GetPhoneNo() const
{
	return m_pCompany->GetPhoneNo();
}

void Company::CompanyForInitialisationImpl::SetCompanyName(const CString& sCompanyName)
{
	m_pCompany->m_CompanyName = sCompanyName;
}

void Company::CompanyForInitialisationImpl::SetCity(const CString& sCity)
{
	m_pCompany->m_City = sCity;
}

void Company::CompanyForInitialisationImpl::SetZipCode(const CString& sZipCode)
{
	m_pCompany->m_ZipCode = sZipCode;
}

void Company::CompanyForInitialisationImpl::SetStreet(const CString& sStreet)
{
	m_pCompany->m_Street = sStreet;
}

void Company::CompanyForInitialisationImpl::SetPOBox(const CString& sPOBox)
{
	m_pCompany->m_POBox = sPOBox;
}

void Company::CompanyForInitialisationImpl::SetFarmNo(const CString& sFarmNo)
{
	m_pCompany->m_FarmNo = sFarmNo;
}
	
void Company::CompanyForInitialisationImpl::SetNIP(const CString& sNIP)
{
	m_pCompany->m_NIP = sNIP;
}

void Company::CompanyForInitialisationImpl::SetREGON(const CString& sREGON)
{
	m_pCompany->m_REGON = sREGON;
}

void Company::CompanyForInitialisationImpl::SetBank(const CString& sBankName)
{
	m_pCompany->m_Bank = sBankName;
}

void Company::CompanyForInitialisationImpl::SetAccountNo(const CString& sAccountNo)
{
	m_pCompany->m_AccountNo = sAccountNo;
}

void Company::CompanyForInitialisationImpl::SetWetIdNo(const CString& sWetIdNo)
{
	m_pCompany->m_WetIdNo = sWetIdNo;
}

void Company::CompanyForInitialisationImpl::SetWetLicNo(const CString& sWetLicNo)
{
	m_pCompany->m_WetLicNo = sWetLicNo;
}

void Company::CompanyForInitialisationImpl::SetPhoneNo(const CString& sPhoneNo)
{
	m_pCompany->m_PhoneNo = sPhoneNo;
}



/////////////////////////////////////////////////////////////////////////////////////////

Company::CompanyForChangingImpl::CompanyForChangingImpl(Company *pCompany) : m_pCompany(pCompany)
{
	m_REGON = pCompany->GetREGON();
	m_Bank = pCompany->GetBank();
	m_AccountNo = pCompany->GetAccountNo();
	m_WetIdNo = pCompany->GetWetIdNo();
	m_WetLicNo = pCompany->GetWetLicNo();
	m_PhoneNo = pCompany->GetPhoneNo();
}

void Company::CompanyForChangingImpl::SetCompanyName(const CString& sCompanyName)
{
	m_CompanyName = sCompanyName;
}

void Company::CompanyForChangingImpl::SetCity(const CString& sCity)
{
	m_City = sCity;
}

void Company::CompanyForChangingImpl::SetZipCode(const CString& sZipCode)
{
	m_ZipCode = sZipCode;
}

void Company::CompanyForChangingImpl::SetStreet(const CString& sStreet)
{
	m_Street = sStreet;
}

void Company::CompanyForChangingImpl::SetPOBox(const CString& sPOBox)
{
	m_POBox = sPOBox;
}

void Company::CompanyForChangingImpl::SetFarmNo(const CString& sFarmNo)
{
	m_FarmNo = sFarmNo;
}
	
void Company::CompanyForChangingImpl::SetNIP(const CString& sNIP)
{
	m_NIP = sNIP;
}


void Company::CompanyForChangingImpl::SetREGON(const CString& sREGON)
{
	m_REGON = sREGON;
}

void Company::CompanyForChangingImpl::SetBank(const CString& sBankName)
{
	m_Bank = sBankName;
}

void Company::CompanyForChangingImpl::SetAccountNo(const CString& sAccountNo)
{
	m_AccountNo = sAccountNo;
}

void Company::CompanyForChangingImpl::SetWetIdNo(const CString& sWetIdNo)
{
	m_WetIdNo = sWetIdNo;
}

void Company::CompanyForChangingImpl::SetWetLicNo(const CString& sWetLicNo)
{
	m_WetLicNo = sWetLicNo;
}

void Company::CompanyForChangingImpl::SetPhoneNo(const CString& sPhoneNo)
{
	m_PhoneNo = sPhoneNo;
}

const CString& Company::CompanyForChangingImpl::GetCompanyName() const
{
	return m_CompanyName;
	
}

const CString& Company::CompanyForChangingImpl::GetCity() const
{
	return m_City;
	
}

const CString& Company::CompanyForChangingImpl::GetZipCode() const
{
	return m_ZipCode;
	
}

const CString& Company::CompanyForChangingImpl::GetStreet() const
{
	return m_Street;
	
}

const CString& Company::CompanyForChangingImpl::GetPOBox() const
{
	return m_POBox;
	
}

const CString& Company::CompanyForChangingImpl::GetFarmNo() const
{
	return m_FarmNo;
	
}

const CString& Company::CompanyForChangingImpl::GetNIP() const
{
	return m_NIP;
}

const CString& Company::CompanyForChangingImpl::GetREGON() const
{
	return m_REGON;
	
}

const CString& Company::CompanyForChangingImpl::GetBank() const
{
	return m_Bank;
	
}

const CString& Company::CompanyForChangingImpl::GetAccountNo() const
{
	return m_AccountNo;
	
}

const CString& Company::CompanyForChangingImpl::GetWetIdNo() const
{
	return m_WetIdNo;
	
}

const CString& Company::CompanyForChangingImpl::GetWetLicNo() const
{
	return m_WetLicNo;
	
}

const CString& Company::CompanyForChangingImpl::GetPhoneNo() const
{
	return m_PhoneNo;
}

bool Company::CompanyForChangingImpl::Save()
{
	if(SQLDB.UpdateCompany(this))
	{
		m_pCompany->m_REGON = m_REGON;
		m_pCompany->m_Bank = m_Bank;
		m_pCompany->m_AccountNo = m_AccountNo;
		m_pCompany->m_WetIdNo = m_WetIdNo;
		m_pCompany->m_WetLicNo = m_WetLicNo;
		m_pCompany->m_PhoneNo = m_PhoneNo;	

		m_pCompany->m_CompanyName = m_CompanyName;
		m_pCompany->m_Street = m_Street;
		m_pCompany->m_POBox = m_POBox;
		m_pCompany->m_City = m_City;
		m_pCompany->m_ZipCode = m_ZipCode;
		m_pCompany->m_FarmNo = m_FarmNo;
		m_pCompany->m_NIP = m_NIP;



		return true;
	}
	return false;
}
