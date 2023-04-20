#include "stdafx.h"
#include "RepoCowBuilder.h"
#include "RepoException.h"


RepoCowBuilder::RepoCowBuilder(HentResolver* pHentResolver)
{
	m_pHentResolver = pHentResolver;
}
	
RepoCow* RepoCowBuilder::Build()
{
	if(m_ConstructedCow.GetCowNo().IsEmpty())
		throw new RepoException("invalid cow no");

	if(m_ConstructedCow.GetStockCode().IsEmpty())
		throw new RepoException("missing stock");

	if(m_ConstructedCow.GetWeight() < 0.001)
		throw new RepoException("invalid weight, it has to be > 0.001");

	if(m_ConstructedCow.GetPrice() < 0.01)
		throw new RepoException("invalid price, it has to be > 0.01");

	RepoCow* pNewCow = new RepoCow();
	pNewCow->CopyFrom(m_ConstructedCow);	
	return pNewCow;	
}

RepoHent* RepoCowBuilder::ResolveHent(const CString& sFarmNo)
{
	if(m_pHentResolver)
		return m_pHentResolver->Resolve(sFarmNo);
	return NULL;
}

void RepoCowBuilder::SetFirstOwner(const CString& sFirstOwnerFarmNo)
{
	m_ConstructedCow.SetFirstOwner(ResolveHent(sFirstOwnerFarmNo));
}

void RepoCowBuilder::SetCowNo(const CString& sCowNo)
{
	m_ConstructedCow.SetCowNo(sCowNo);
}

void RepoCowBuilder::SetSex(const CowSex& sex)
{
	m_ConstructedCow.SetSex(sex);
}

void RepoCowBuilder::SetStockCode(const CString& sStockCode)
{
	m_ConstructedCow.SetStockCode(sStockCode);
}

void RepoCowBuilder::SetWeight(const Weight& weight)
{
	m_ConstructedCow.SetWeight(weight);
}

void RepoCowBuilder::SetPrice(const Money& price)
{
	m_ConstructedCow.SetPrice(price);
}

void RepoCowBuilder::SetLatitude(const Latitude& latitude)
{
	m_ConstructedCow.SetLatitude(latitude);
}

void RepoCowBuilder::SetLongitude(const Longitude& longitude)
{
	m_ConstructedCow.SetLongitude(longitude);
}

void RepoCowBuilder::SetPassportNo(const CString& sPassportNo)
{
	m_ConstructedCow.SetPassportNo(sPassportNo);
}

void RepoCowBuilder::SetPassportIssueDt(const CDateTime& passportIssueDt)
{
	m_ConstructedCow.SetPassportIssueDt(passportIssueDt);
}

void RepoCowBuilder::SetHealthCertNo(const CString& sHealthCertNo)
{
	m_ConstructedCow.SetHealthCertNo(sHealthCertNo);
}

void RepoCowBuilder::SetMotherNo(const CString& sMotherNo)
{
	m_ConstructedCow.SetMotherNo(sMotherNo);
}

void RepoCowBuilder::SetBirthPlace(const CString& sBirthPlace)
{
	m_ConstructedCow.SetBirthPlace(sBirthPlace);
}

void RepoCowBuilder::SetBirthDt(const CDateTime& birthDt)
{
	m_ConstructedCow.SetBirthDt(birthDt);
}
