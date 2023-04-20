#ifndef _HENTMATCHER_H_
#define _HENTMATCHER_H_

#include "RawDataMatcher.h"
#include "MatchNullInt.h"
#include "MatchIntRange.h"
#include "MatchDate.h"

class CHentMatcher : public CRawDataMatcher<Hent>
{
public:
	
	void FindName(const CString& hentName);
	CString GetName(Hent* pHent);

	void FindAlias(const CString& hentAlias);
	CString GetAlias(Hent* pHent);
	
	void FindARiMRNo(const CString& hentARiMRNo);
	CString GetARiMRNo(Hent *pHent);
	
	void FindWetNo(const CString& hentWetNo);
	CString GetWetNo(Hent *pHent);

	void FindNIP(const CString& hentNIP);
	CString GetNIP(Hent *pHent);

	void FindStreet(const CString& hentStreet);
	CString GetStreet(Hent *pHent);

	void FindPoBox(const CString& hentPoBox);
	CString GetPoBox(Hent *pHent);

	void FindPhoneNo(const CString& hentPhoneNo);
	CString GetPhoneNo(Hent *pHent);

	void FindCity(const CString& hentCity);
	CString GetCity(Hent *pHent);
	
	void FindZip(const CString& hentZip);
	CString GetZip(Hent *pHent);

	void FindHentType(HentType *pHentType);
	HentType* GetHentType(Hent *pHent);

	void FindPesel(const CString& hentPesel);
	CString GetPesel(Hent *pHent);

	void FindRegon(const CString& hentRegon);
	CString GetRegon(Hent *pHent);

	void FindIdNo(const CString& hentIdNo);
	CString GetIdNo(Hent *pHent);

	void FindIssueDate(const CDateTime& issueDt1,const CDateTime& issueDt2,TIMESPAN spanType);
	CDateTime GetIssueDate(Hent *pHent);

	void FindIssuePost(const CString& hentIssuePost);
	CString GetIssuePost(Hent *pHent);

	void FindAccountNo(const CString& hentAccountNo);
	CString GetAccountNo(Hent *pHent);

	void FindBankName(const CString& hentBankName);
	CString GetBankName(Hent *pHent);

	void FindCountry(Country *pCountry);
	Country* GetCountry(Hent *pHent);
	
	void FindExtras(const CString& hentExtras);
	CString GetExtras(Hent *pHent);

	void FindPlate(const CString& hentPlate);
	CString GetPlate(Hent *pHent);
	
};




#endif


	
	





