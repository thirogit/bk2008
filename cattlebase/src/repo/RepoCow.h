#ifndef __REPOCOW_H__
#define __REPOCOW_H__

#include "RepoHent.h"
#include "../classes/types/Weight.h"
#include "../classes/types/CowSex.h"

class RepoCow
{
public:	
	RepoCow();
	RepoCow(const RepoCow& src);
	void CopyFrom(const RepoCow& src);

	const CString& GetCowNo() const;
	const CowSex& GetSex() const;	
	const CString& GetStockCode() const;
	const Weight& GetWeight() const;	
	const Money& GetPrice() const;	
	const Latitude& GetLatitude() const;
	const Longitude& GetLongitude() const;
	const CString& GetPassportNo() const;	
	RepoHent* GetFirstOwner() const;
	const CDateTime& GetPassportIssueDt() const;
	const CString& GetHealthCertNo() const;
	const CString& GetMotherNo() const;
	const CString& GetBirthPlace() const;
	const CDateTime& GetBirthDt() const;
	void SetCowNo(const CString& sCowNo);
	void SetSex(const CowSex& sex);	
	void SetStockCode(const CString& sStockCode);
	void SetWeight(const Weight& weight);	
	void SetPrice(const Money& price);	
	void SetLatitude(const Latitude& latitude);
	void SetLongitude(const Longitude& longitude);
	void SetPassportNo(const CString& sPassportNo);	
	void SetFirstOwner(RepoHent* pFirstOwner);
	void SetPassportIssueDt(const CDateTime& passportIssueDt);
	void SetHealthCertNo(const CString& sHealthCertNo);
	void SetMotherNo(const CString& sMotherNo);
	void SetBirthPlace(const CString& sBirthPlace);
	void SetBirthDt(const CDateTime& birthDt);


private:
	CString m_CowNo;
	CowSex m_Sex;	
	CString m_StockCode;
	Weight m_Weight;	
	Money m_Price;	
	Latitude m_Latitude;
	Longitude m_Longitude;
	CString m_PassportNo;	
	RepoHent* m_pFirstOwner;
	CDateTime m_PassportIssueDt;
	CString m_HealthCertNo;
	CString m_MotherNo;
	CString m_BirthPlace;
	CDateTime m_BirthDt;

};
	

#endif