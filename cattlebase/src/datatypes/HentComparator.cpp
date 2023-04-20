#include "stdafx.h"
#include "HentComparator.h"

COMPARE_RESULT HentComparator::CompareDataBy(UINT dataMember,Hent* DataLeft,Hent* DataRight)
{
	Hent *left = DataLeft,*right = DataRight;
	switch(dataMember)
	{
		default:
			ASSERT(0);
		case HENTS_SORT_BY_NAME:
			return PolishStringCompare(left->GetName(),right->GetName());
		case HENTS_SORT_BY_ARIMRNO:
			return left->GetFarmNo().CompareNoCase(right->GetFarmNo());
		case HENTS_SORT_BY_ALIAS:
			return PolishStringCompare(left->GetAlias(),right->GetAlias());
		case HENTS_SORT_BY_STREET:
			return PolishStringCompare(left->GetStreet(),right->GetStreet());
		case HENTS_SORT_BY_POBOX:
			return left->GetPOBox().Compare(right->GetPOBox());
		case HENTS_SORT_BY_CITY:
			return PolishStringCompare(left->GetCity(),right->GetCity());
		case HENTS_SORT_BY_ZIP:
			return left->GetZip().Compare(right->GetZip());
		case HENTS_SORT_BY_HENTTYPE:
			return PolishStringCompare(left->GetHentType()->henttypename,right->GetHentType()->henttypename);
		case HENTS_SORT_BY_PLATE:
			return PolishStringCompare(left->GetPlate(),right->GetPlate());
		case HENTS_SORT_BY_SYNCIT:
			return CompareBool(left->GetSyncIt() , right->GetSyncIt());
		case HENTS_SORT_BY_COMBOITEM:
			return PolishStringCompare(left->GetComboItemText(),right->GetComboItemText());
		case HENTDATA_MEMBER_PHONE:
			return PolishStringCompare(left->GetPhone(),right->GetPhone());
		case HENTDATA_MEMBER_NIP:
			return left->GetNIP().Compare(right->GetNIP());
		case HENTDATA_MEMBER_WETNO:
			return left->GetWetNo().Compare(right->GetWetNo());
		case HENTDATA_MEMBER_EXTRAS:
			return PolishStringCompare(left->GetExtras(),right->GetExtras());
		case HENTDATA_MEMBER_COUNTRY:
			return PolishStringCompare(left->GetCountry()->GetCountryName(),right->GetCountry()->GetCountryName());
		case HENTDATA_MEMBER_PESEL:
			return left->GetPESEL().Compare(right->GetPESEL());
		case HENTDATA_MEMBER_REGON:
			return left->GetREGON().Compare(right->GetREGON());
		case HENTDATA_MEMBER_IDNO:
			return left->GetIdNo().Compare(right->GetIdNo());
		case HENTDATA_MEMBER_ISSUEDATE:
			return CompareDateTime(left->GetIssueDate(),right->GetIssueDate());
		case HENTDATA_MEMBER_ISSUEPOST:
			return PolishStringCompare(left->GetIssuePost(),right->GetIssuePost());
		case HENTDATA_MEMBER_ACCOUNTNO:
			return left->GetAccountNo().Compare(right->GetAccountNo());
		case HENTDATA_MEMBER_BANKNAME:
			return PolishStringCompare(left->GetBankName(),right->GetBankName());
		case HENTDATA_MEMBER_WETLICENCENO:
			return PolishStringCompare(left->GetWetLicenceNo(),right->GetWetLicenceNo());
		case HENTDATA_MEMBER_CELLPHONENO:
			return PolishStringCompare(left->GetCellPhoneNo(),right->GetCellPhoneNo());
		case HENTDATA_MEMBER_EMAILADDRESS:
			return PolishStringCompare(left->GetEmailAddress(),right->GetEmailAddress());
		case HENTDATA_MEMBER_LATITUDE:
			return CompareNullable<Latitude>(left->GetLatitude(),right->GetLatitude());
		case HENTDATA_MEMBER_LONGITUDE:
			return CompareNullable<Longitude>(left->GetLongitude(),right->GetLongitude());
	}
	ASSERT(0);
	return COMPARE_LESS;
}
