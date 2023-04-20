#include "stdafx.h"
#include "RepoCompany.h"

RepoCompany::RepoCompany()
{
}

RepoCompany::RepoCompany(const RepoCompany& src)
{
	CopyFrom(src);
}

RepoCompany::RepoCompany(const Company& src)
{
	CopyFrom(src);
}


void RepoCompany::CopyFrom(const RepoCompany& src)
{
	m_sCompanyName = src.m_sCompanyName;
	m_sCity = src.m_sCity;
	m_sZipCode = src.m_sZipCode;
	m_sStreet = src.m_sStreet;
	m_sPOBox = src.m_sPOBox;
	m_sFarmNo = src.m_sFarmNo;	
	m_sNIP = src.m_sNIP;
	m_sREGON = src.m_sREGON;
	m_sBank = src.m_sBank;
	m_sAccountNo = src.m_sAccountNo;
	m_sWetIdNo = src.m_sWetIdNo;
	m_sWetLicNo = src.m_sWetLicNo;
	m_sPhoneNo = src.m_sPhoneNo;
}


void RepoCompany::CopyFrom(const Company& src)
{
	m_sCompanyName = src.GetCompanyName();
	m_sCity = src.GetCity();
	m_sZipCode = src.GetZipCode();
	m_sStreet = src.GetStreet();
	m_sPOBox = src.GetPOBox();
	m_sFarmNo = src.GetFarmNo();	
	m_sNIP = src.GetNIP();
	m_sREGON = src.GetREGON();
	m_sBank = src.GetBank();
	m_sAccountNo = src.GetAccountNo();
	m_sWetIdNo = src.GetWetIdNo();
	m_sWetLicNo = src.GetWetLicNo();
	m_sPhoneNo = src.GetPhoneNo();
}

void RepoCompany::SetCompanyName(const CString& sCompanyName)
{
	m_sCompanyName = sCompanyName;
}

void RepoCompany::SetCity(const CString& sCity)
{
	m_sCity = sCity;
}

void RepoCompany::SetZipCode(const CString& sZipCode)
{
	m_sZipCode = sZipCode;
}

void RepoCompany::SetStreet(const CString& sStreet)
{
	m_sStreet = sStreet;
}

void RepoCompany::SetPOBox(const CString& sPOBox)
{
	m_sPOBox = sPOBox;
}

void RepoCompany::SetFarmNo(const CString& sFarmNo)
{
	m_sFarmNo = sFarmNo;
}

void RepoCompany::SetNIP(const CString& sNIP)
{
	m_sNIP = sNIP;
}

void RepoCompany::SetREGON(const CString& sREGON)
{
	m_sREGON = sREGON;
}

void RepoCompany::SetBank(const CString& sBank)
{
	m_sBank = sBank;
}

void RepoCompany::SetAccountNo(const CString& sAccountNo)
{
	m_sAccountNo = sAccountNo;
}

void RepoCompany::SetWetIdNo(const CString& sWetIdNo)
{
	m_sWetIdNo = sWetIdNo;
}

void RepoCompany::SetWetLicNo(const CString& sWetLicNo)
{
	m_sWetLicNo = sWetLicNo;
}

void RepoCompany::SetPhoneNo(const CString& sPhoneNo)
{
	m_sPhoneNo = sPhoneNo;
}

const CString& RepoCompany::GetCompanyName() const
{
	return m_sCompanyName;
}

const CString& RepoCompany::GetCity() const
{
	return m_sCity;
}

const CString& RepoCompany::GetZipCode() const
{
	return m_sZipCode;
}

const CString& RepoCompany::GetStreet() const
{
	return m_sStreet;
}

const CString& RepoCompany::GetPOBox() const
{
	return m_sPOBox;
}

const CString& RepoCompany::GetFarmNo() const
{
	return m_sFarmNo;
}	
const CString& RepoCompany::GetNIP() const
{
	return m_sNIP;
}
const CString& RepoCompany::GetREGON() const
{
	return m_sREGON;
}
const CString& RepoCompany::GetBank() const
{
	return m_sBank;
}
const CString& RepoCompany::GetAccountNo() const
{
	return m_sAccountNo;
}
const CString& RepoCompany::GetWetIdNo() const
{
	return m_sWetIdNo;
}
const CString& RepoCompany::GetWetLicNo() const
{
	return m_sWetLicNo;
}
const CString& RepoCompany::GetPhoneNo() const
{
	return m_sPhoneNo;
}
	
