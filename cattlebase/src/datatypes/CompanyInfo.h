#ifndef __COMPANY_H__
#define __COMPANY_H__

#include <afxmt.h>
#include "Hent.h"


class CompanyForChanging
{
public:	

	virtual void SetCompanyName(const CString& sCompanyName) = 0;
	virtual void SetCity(const CString& sCity) = 0;
	virtual void SetZipCode(const CString& sZipCode) = 0;
	virtual void SetStreet(const CString& sStreet) = 0;
	virtual void SetPOBox(const CString& sPOBox) = 0;
	virtual void SetFarmNo(const CString& sFarmNo) = 0;	
	virtual void SetNIP(const CString& sNIP) = 0;
	virtual void SetREGON(const CString&) = 0;
	virtual void SetBank(const CString&) = 0;
	virtual void SetAccountNo(const CString&) = 0;
	virtual void SetWetIdNo(const CString&) = 0;
	virtual void SetWetLicNo(const CString&) = 0;
	virtual void SetPhoneNo(const CString&) = 0;

	virtual const CString& GetCompanyName() const = 0;
	virtual const CString& GetCity() const = 0;
	virtual const CString& GetZipCode() const = 0;
	virtual const CString& GetStreet() const = 0;
	virtual const CString& GetPOBox() const = 0;
	virtual const CString& GetFarmNo() const = 0;	
	virtual const CString& GetNIP() const = 0;
	virtual const CString& GetREGON() const = 0;
	virtual const CString& GetBank() const = 0;
	virtual const CString& GetAccountNo() const = 0;
	virtual const CString& GetWetIdNo() const = 0;
	virtual const CString& GetWetLicNo() const = 0;
	virtual const CString& GetPhoneNo() const = 0;
	virtual bool Save() = 0;

};

class CompanyForInitialisation
{
public:
	virtual const CString& GetCompanyName() const = 0;
	virtual const CString& GetCity() const = 0;
	virtual const CString& GetZipCode() const = 0;
	virtual const CString& GetStreet() const = 0;
	virtual const CString& GetPOBox() const = 0;
	virtual const CString& GetFarmNo() const = 0;	
	virtual const CString& GetNIP() const = 0;
	virtual const CString& GetREGON() const = 0;
	virtual const CString& GetBank() const = 0;
	virtual const CString& GetAccountNo() const = 0;
	virtual const CString& GetWetIdNo() const = 0;
	virtual const CString& GetWetLicNo() const = 0;
	virtual const CString& GetPhoneNo() const = 0;

	virtual void SetCompanyName(const CString&) = 0;
	virtual void SetCity(const CString&) = 0;
	virtual void SetZipCode(const CString&) = 0;
	virtual void SetStreet(const CString&) = 0;
	virtual void SetPOBox(const CString&) = 0;
	virtual void SetFarmNo(const CString&) = 0;	
	virtual void SetNIP(const CString&) = 0;
	virtual void SetREGON(const CString&) = 0;
	virtual void SetBank(const CString&) = 0;
	virtual void SetAccountNo(const CString&) = 0;
	virtual void SetWetIdNo(const CString&) = 0;
	virtual void SetWetLicNo(const CString&) = 0;
	virtual void SetPhoneNo(const CString&) = 0;

};

class Company
{
private:
	class CompanyForChangingImpl : public CompanyForChanging
	{
	public:
		CompanyForChangingImpl(Company *pCompany);
		
		virtual void SetCompanyName(const CString& sCompanyName);
		virtual void SetCity(const CString& sCity);
		virtual void SetZipCode(const CString& sZipCode);
		virtual void SetStreet(const CString& sStreet);
		virtual void SetPOBox(const CString& sPOBox);
		virtual void SetFarmNo(const CString& sFarmNo);	
		virtual void SetNIP(const CString& sNIP);
		virtual void SetREGON(const CString& sREGON);
		virtual void SetBank(const CString& sBankName);
		virtual void SetAccountNo(const CString& sAccountNo);
		virtual void SetWetIdNo(const CString& sWetIdNo);
		virtual void SetWetLicNo(const CString& sWetLicNo);
		virtual void SetPhoneNo(const CString& sPhoneNo);

		virtual const CString& GetCompanyName() const;
		virtual const CString& GetCity() const;
		virtual const CString& GetZipCode() const;
		virtual const CString& GetStreet() const;
		virtual const CString& GetPOBox() const;
		virtual const CString& GetFarmNo() const;	
		virtual const CString& GetNIP() const;
		virtual const CString& GetREGON() const;
		virtual const CString& GetBank() const;
		virtual const CString& GetAccountNo() const;
		virtual const CString& GetWetIdNo() const;
		virtual const CString& GetWetLicNo() const;
		virtual const CString& GetPhoneNo() const;
		virtual bool Save();
	private:
		Company *m_pCompany;
		CString m_REGON;
		CString m_Bank;
		CString m_AccountNo;
		CString m_WetIdNo;
		CString m_WetLicNo;
		CString m_PhoneNo;
		CString m_CompanyName;
		CString m_City;
		CString m_ZipCode;
		CString m_Street;
		CString m_POBox;
		CString m_FarmNo;
		CString m_NIP;
	};


	class CompanyForInitialisationImpl : public CompanyForInitialisation
	{
	public:
		CompanyForInitialisationImpl(Company* pCompany);
		virtual const CString& GetCompanyName() const;
		virtual const CString& GetCity() const;
		virtual const CString& GetZipCode() const;
		virtual const CString& GetStreet() const;
		virtual const CString& GetPOBox() const;
		virtual const CString& GetFarmNo() const;	
		virtual const CString& GetNIP() const;
		virtual const CString& GetREGON() const;
		virtual const CString& GetBank() const;
		virtual const CString& GetAccountNo() const;
		virtual const CString& GetWetIdNo() const;
		virtual const CString& GetWetLicNo() const;
		virtual const CString& GetPhoneNo() const;

		virtual void SetCompanyName(const CString& sCompanyName);
		virtual void SetCity(const CString& sCity);
		virtual void SetZipCode(const CString& sZipCode);
		virtual void SetStreet(const CString& sStreet);
		virtual void SetPOBox(const CString& sPOBox);
		virtual void SetFarmNo(const CString& sFarmNo);	
		virtual void SetNIP(const CString& sNIP);
		virtual void SetREGON(const CString& sREGON);
		virtual void SetBank(const CString& sBankName);
		virtual void SetAccountNo(const CString& sAccountNo);
		virtual void SetWetIdNo(const CString& sWetIdNo);
		virtual void SetWetLicNo(const CString& sWetLicNo);
		virtual void SetPhoneNo(const CString& sPhoneNo);
	private:
		Company* m_pCompany;
	};
private:
	static Company m_Company;
private:
	Company();
	void CreateCompanyHent();
	bool IsLockedByCurrentThread();
	
	CString m_CompanyName;
	CString m_City;
	CString m_ZipCode;
	CString m_Street;
	CString m_POBox;
	CString m_FarmNo;	
	CString m_NIP;
	CString m_REGON;
	CString m_Bank;
	CString m_AccountNo;
	CString m_WetIdNo;
	CString m_WetLicNo;
	CString m_PhoneNo;
	
	CCriticalSection m_cs;
	CSingleLock m_lock;
	HANDLE m_lockingThread;	
	CompanyForChangingImpl m_ProxyForChanging;
	CompanyForInitialisationImpl m_ProxyForInitialisation;
	Hent   m_CompanyHent;
	
public:
	static const Company* GetCompany();
	static CompanyForChanging* GetCompanyForChanging();
	static CompanyForInitialisation* GetCompanyForInitialisation();
	static void Lock();
	static void Unlock();

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
	const Hent*    GetAsHent() const;


	
};




#endif