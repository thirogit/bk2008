#include "stdafx.h"

#include "HentMatcher.h"
#include "MatchString.h"
#include "MatchPtr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CHentMatcher::FindName(const CString& hentName)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentName,&CHentMatcher::GetName));
}
CString CHentMatcher::GetName(Hent* pHent)
{
	return pHent->GetName();
}

void CHentMatcher::FindAlias(const CString& hentAlias)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentAlias,&CHentMatcher::GetAlias));
}
CString CHentMatcher::GetAlias(Hent* pHent)
{
	return pHent->GetAlias();
}

void CHentMatcher::FindARiMRNo(const CString& hentARiMRNo)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentARiMRNo,&CHentMatcher::GetARiMRNo));
}
CString CHentMatcher::GetARiMRNo(Hent *pHent)
{
	return pHent->GetFarmNo();
}

void CHentMatcher::FindWetNo(const CString& hentWetNo)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentWetNo,&CHentMatcher::GetWetNo));
}
CString CHentMatcher::GetWetNo(Hent *pHent)
{
	return pHent->GetWetNo();
}

void CHentMatcher::FindNIP(const CString& hentNIP)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentNIP,&CHentMatcher::GetNIP));
}
CString CHentMatcher::GetNIP(Hent *pHent)
{
	return pHent->GetNIP();
}

void CHentMatcher::FindStreet(const CString& hentStreet)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentStreet,&CHentMatcher::GetStreet));
}
CString CHentMatcher::GetStreet(Hent *pHent)
{
	return pHent->GetStreet();
}

void CHentMatcher::FindPoBox(const CString& hentPoBox)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentPoBox,&CHentMatcher::GetPoBox));
}
CString CHentMatcher::GetPoBox(Hent *pHent)
{
	return pHent->GetPOBox();
}

void CHentMatcher::FindPhoneNo(const CString& hentPhoneNo)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentPhoneNo,&CHentMatcher::GetPhoneNo));
}
CString CHentMatcher::GetPhoneNo(Hent *pHent)
{
	return pHent->GetPhone();
}

void CHentMatcher::FindCity(const CString& hentCity)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentCity,&CHentMatcher::GetCity));
}
CString CHentMatcher::GetCity(Hent *pHent)
{
	return pHent->GetCity();
}

void CHentMatcher::FindZip(const CString& hentZip)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentZip,&CHentMatcher::GetZip));
}
CString CHentMatcher::GetZip(Hent *pHent)
{
	return pHent->GetZip();
}

void CHentMatcher::FindHentType(HentType *pHentType)
{
	AddMatch(new MatchPtr<Hent,CHentMatcher,HentType>(this,pHentType,false,&CHentMatcher::GetHentType));
}
HentType* CHentMatcher::GetHentType(Hent *pHent)
{
	return pHent->GetHentType();
}

void CHentMatcher::FindPesel(const CString& hentPesel)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentPesel,&CHentMatcher::GetPesel));
}
CString CHentMatcher::GetPesel(Hent *pHent)
{
	return pHent->GetPESEL();
}

void CHentMatcher::FindRegon(const CString& hentRegon)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentRegon,&CHentMatcher::GetRegon));
}
CString CHentMatcher::GetRegon(Hent *pHent)
{
	return pHent->GetREGON();
}

void CHentMatcher::FindIdNo(const CString& hentIdNo)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentIdNo,&CHentMatcher::GetIdNo));
}
CString CHentMatcher::GetIdNo(Hent *pHent)
{
	return pHent->GetIdNo();
}

void CHentMatcher::FindIssueDate(const CDateTime& issueDt1,const CDateTime& issueDt2,TIMESPAN spanType)
{
	AddMatch(new MatchDate<Hent,CHentMatcher>(this,issueDt1,issueDt2,spanType,&CHentMatcher::GetIssueDate));
}
CDateTime CHentMatcher::GetIssueDate(Hent *pHent)
{
	return pHent->GetIssueDate();	
}

void CHentMatcher::FindIssuePost(const CString& hentIssuePost)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentIssuePost,&CHentMatcher::GetIssuePost));
}
CString CHentMatcher::GetIssuePost(Hent *pHent)
{
	return pHent->GetIssuePost();
}

void CHentMatcher::FindAccountNo(const CString& hentAccountNo)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentAccountNo,&CHentMatcher::GetAccountNo));
}
CString CHentMatcher::GetAccountNo(Hent *pHent)
{
	return pHent->GetAccountNo();
}

void CHentMatcher::FindBankName(const CString& hentBankName)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentBankName,&CHentMatcher::GetBankName));
}
CString CHentMatcher::GetBankName(Hent *pHent)
{
	return pHent->GetBankName();
}

void CHentMatcher::FindCountry(Country *pCountry)
{
	AddMatch(new MatchPtr<Hent,CHentMatcher,Country>(this,pCountry,false,&CHentMatcher::GetCountry));
}
Country* CHentMatcher::GetCountry(Hent *pHent)
{
	return pHent->GetCountry();
}

void CHentMatcher::FindExtras(const CString& hentExtras)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentExtras,&CHentMatcher::GetExtras));
}
CString CHentMatcher::GetExtras(Hent *pHent)
{
	return pHent->GetExtras();
}

void CHentMatcher::FindPlate(const CString& hentPlate)
{
	AddMatch(new MatchString<Hent,CHentMatcher>(this,hentPlate,&CHentMatcher::GetPlate));
}
CString CHentMatcher::GetPlate(Hent *pHent)
{
	return pHent->GetPlate();
}

