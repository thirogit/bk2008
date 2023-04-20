#ifndef __REPOCOMPANY_H__
#define __REPOCOMPANY_H__

#include "../datatypes/CompanyInfo.h"

class RepoCompany
{
public:	
	RepoCompany();
	RepoCompany(const RepoCompany& src);
	RepoCompany(const Company& src);

	void CopyFrom(const RepoCompany& src);
	void CopyFrom(const Company& src);

	void SetCompanyName(const CString& sCompanyName);
	void SetCity(const CString& sCity);
	void SetZipCode(const CString& sZipCode);
	void SetStreet(const CString& sStreet);
	void SetPOBox(const CString& sPOBox);
	void SetFarmNo(const CString& sFarmNo);	
	void SetNIP(const CString& sNIP);
	void SetREGON(const CString& sREGON);
	void SetBank(const CString& sBank);
	void SetAccountNo(const CString& sAccountNo);
	void SetWetIdNo(const CString& sWetIdNo);
	void SetWetLicNo(const CString& sWetLicNo);
	void SetPhoneNo(const CString& sPhoneNo);

	const CString& GetCompanyName() const;
	const CString& GetCity() const;
	const CString& GetZipCode() const;
	const CString& GetStreet() const;
	const CString& GetPOBox() const;
	const CString& GetFarmNo() const;	
	const CString& GetNIP() const;
	const CString& GetREGON() const;
	const CString& GetBank() const;
	const CString& GetAccountNo() const;
	const CString& GetWetIdNo() const;
	const CString& GetWetLicNo() const;
	const CString& GetPhoneNo() const;
	
private:
	CString m_sCompanyName;
	CString m_sCity;
	CString m_sZipCode;
	CString m_sStreet;
	CString m_sPOBox;
	CString m_sFarmNo;	
	CString m_sNIP;
	CString m_sREGON;
	CString m_sBank;
	CString m_sAccountNo;
	CString m_sWetIdNo;
	CString m_sWetLicNo;
	CString m_sPhoneNo;
};

#endif