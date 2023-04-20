#include "stdafx.h"
#include "RepoCow.h"

RepoCow::RepoCow() : m_pFirstOwner(NULL)
{
}

RepoCow::RepoCow(const RepoCow& src)
{
	CopyFrom(src);
}

void RepoCow::CopyFrom(const RepoCow& src)
{
	m_CowNo = src.m_CowNo;
	m_Sex = src.m_Sex;	
	m_StockCode = src.m_StockCode;
	m_Weight = src.m_Weight;	
	m_Price = src.m_Price;	
	m_Latitude = src.m_Latitude;
	m_Longitude = src.m_Longitude;
	m_PassportNo = src.m_PassportNo;	
	m_pFirstOwner = src.m_pFirstOwner;
	m_PassportIssueDt = src.m_PassportIssueDt;
	m_HealthCertNo = src.m_HealthCertNo;
	m_MotherNo = src.m_MotherNo;
	m_BirthPlace = src.m_BirthPlace;
	m_BirthDt = src.m_BirthDt;
}

const CString& RepoCow::GetCowNo() const
{
	return m_CowNo; 
}

const CowSex& RepoCow::GetSex() const
{
	return m_Sex;
}
	
const CString& RepoCow::GetStockCode() const
{
	return m_StockCode;
}

const Weight& RepoCow::GetWeight() const
{
	return m_Weight;
}
	
const Money& RepoCow::GetPrice() const
{
	return m_Price;
}
	
const Latitude& RepoCow::GetLatitude() const
{
	return m_Latitude;
}

const Longitude& RepoCow::GetLongitude() const
{
	return m_Longitude;
}

const CString& RepoCow::GetPassportNo() const
{
	return m_PassportNo;
}
	
RepoHent* RepoCow::GetFirstOwner() const
{
	return m_pFirstOwner;
}

const CDateTime& RepoCow::GetPassportIssueDt() const
{
	return m_PassportIssueDt;
}

const CString& RepoCow::GetHealthCertNo() const
{
	return m_HealthCertNo;
}

const CString& RepoCow::GetMotherNo() const
{
	return m_MotherNo;
}

const CString& RepoCow::GetBirthPlace() const
{
	return m_BirthPlace;
}

const CDateTime& RepoCow::GetBirthDt() const
{
	return m_BirthDt;
}

void RepoCow::SetCowNo(const CString& sCowNo)
{
	m_CowNo = sCowNo;
}

void RepoCow::SetSex(const CowSex& sex)
{
	m_Sex = sex;
}
	
void RepoCow::SetStockCode(const CString& sStockCode)
{
	m_StockCode = sStockCode;
}

void RepoCow::SetWeight(const Weight& weight)
{
	m_Weight = weight;
}
	
void RepoCow::SetPrice(const Money& price)
{
	m_Price = price;
}
	
void RepoCow::SetLatitude(const Latitude& latitude)
{
	m_Latitude = latitude;
}

void RepoCow::SetLongitude(const Longitude& longitude)
{
	m_Longitude = longitude;
}

void RepoCow::SetPassportNo(const CString& sPassportNo)
{
	m_PassportNo = sPassportNo;
}
	
void RepoCow::SetFirstOwner(RepoHent* pFirstOwner)
{
	m_pFirstOwner = pFirstOwner;
}

void RepoCow::SetPassportIssueDt(const CDateTime& passportIssueDt)
{
	m_PassportIssueDt = passportIssueDt;
}

void RepoCow::SetHealthCertNo(const CString& sHealthCertNo)
{
	m_HealthCertNo = sHealthCertNo;
}

void RepoCow::SetMotherNo(const CString& sMotherNo)
{
	m_MotherNo = sMotherNo;
}

void RepoCow::SetBirthPlace(const CString& sBirthPlace)
{
	m_BirthPlace = sBirthPlace;
}

void RepoCow::SetBirthDt(const CDateTime& birthDt)
{
	m_BirthDt = birthDt;
}