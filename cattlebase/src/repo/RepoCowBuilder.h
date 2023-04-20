#ifndef __REPOCOWBUILDER_H__
#define __REPOCOWBUILDER_H__


#include "HentResolver.h"
#include "RepoCow.h"


class RepoCowBuilder 
{

public:
	RepoCowBuilder(HentResolver* pHentResolver);
	
	void SetFirstOwner(const CString& sFirstOwnerFarmNo);
	
	void SetCowNo(const CString& sCowNo);
	void SetSex(const CowSex& sex);	
	void SetStockCode(const CString& sStockCode);
	void SetWeight(const Weight& weight);	
	void SetPrice(const Money& price);	
	void SetLatitude(const Latitude& latitude);
	void SetLongitude(const Longitude& longitude);
	void SetPassportNo(const CString& sPassportNo);		
	void SetPassportIssueDt(const CDateTime& passportIssueDt);
	void SetHealthCertNo(const CString& sHealthCertNo);
	void SetMotherNo(const CString& sMotherNo);
	void SetBirthPlace(const CString& sBirthPlace);
	void SetBirthDt(const CDateTime& birthDt);

	RepoCow* Build();
protected:
	RepoHent* ResolveHent(const CString& sFarmNo);
private:
	HentResolver* m_pHentResolver;
	RepoCow m_ConstructedCow;
	


};
	

#endif