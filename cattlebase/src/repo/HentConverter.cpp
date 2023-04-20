#include "stdafx.h"
#include "HentConverter.h"
#include "../global_arrays.h"

void HentConverter::ConvertHent(const RepoHent& src,Hent& dest)
{
	dest.SetName(src.GetName());
	dest.SetAlias(src.GetAlias());
	dest.SetStreet(src.GetStreet());
	dest.SetPOBox(src.GetPOBox());
	dest.SetCity(src.GetCity());
	dest.SetZip(src.GetZip());
	dest.SetPhone(src.GetPhone());
	dest.SetNIP(src.GetNIP());
	dest.SetFarmNo(src.GetFarmNo());
	dest.SetWetNo(src.GetWetNo());
	dest.SetExtras(src.GetExtras());
	dest.SetPlate(src.GetPlate());	
	dest.SetHentType(GetHentTypeForRepoHentType(src.GetHentType()));
	dest.SetPESEL(src.GetPESEL());
	dest.SetREGON(src.GetREGON());
	dest.SetIdNo(src.GetIdNo());
	dest.SetIssueDate(src.GetIssueDate());
	dest.SetIssuePost(src.GetIssuePost());
	
	CString sValidAccountNo;
	CString sAccountNo = src.GetAccountNo();
	if(sAccountNo.GetLength() == 28)
	{
		sValidAccountNo = sAccountNo.Mid(2);
	}
	else
	{
		sValidAccountNo = sAccountNo;
	}

	dest.SetAccountNo(sValidAccountNo);
	
	dest.SetBankName(src.GetBankName());
	
	dest.SetWetLicenceNo(src.GetWetLicenceNo());
	dest.SetCellPhoneNo(src.GetCellPhoneNo());
	dest.SetEmailAddress(src.GetEmailAddress());
	dest.SetLatitude(src.GetLatitude());
	dest.SetLongitude(src.GetLongitude());

	CString hentCountryCode = src.GetFarmNo().Left(2);

	dest.SetCountry(ary_countries.GetCountryByCountryCode(hentCountryCode));
}

HentType* HentConverter::GetHentTypeForRepoHentType(RepoHentType hentType)
{
	switch(hentType)
	{
		case HentType_Individual:
			return ary_htypes.GetHentTypeByHentTypeId(HTYPE_INDIVIDUAL);
		case HentType_Company:
			return ary_htypes.GetHentTypeByHentTypeId(HTYPE_COMPANY);
	}
	return NULL;
}