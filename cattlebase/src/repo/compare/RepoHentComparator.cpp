#include "stdafx.h"
#include "RepoHentComparator.h"

COMPARE_RESULT RepoHentComparator::CompareBy(UINT dataMember,RepoHent* DataLeft,RepoHent* DataRight)
{
	RepoHent *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);
		case REPOHENT_MEMBER_NAME:
			return PolishStringCompare(left->GetName(),right->GetName());
		case REPOHENT_MEMBER_FARMNO:
			return left->GetFarmNo().CompareNoCase(right->GetFarmNo());
		case REPOHENT_MEMBER_ALIAS:
			return PolishStringCompare(left->GetAlias(),right->GetAlias());
		case REPOHENT_MEMBER_STREET:
			return PolishStringCompare(left->GetStreet(),right->GetStreet());
		case REPOHENT_MEMBER_POBOX:
			return left->GetPOBox().Compare(right->GetPOBox());
		case REPOHENT_MEMBER_CITY:
			return PolishStringCompare(left->GetCity(),right->GetCity());
		case REPOHENT_MEMBER_ZIP:
			return left->GetZip().Compare(right->GetZip());
		case REPOHENT_MEMBER_HENTTYPE:
			return left->GetHentType() > right->GetHentType();
		case REPOHENT_MEMBER_PLATE:
			return PolishStringCompare(left->GetPlate(),right->GetPlate());
		case REPOHENT_MEMBER_PHONE:
			return PolishStringCompare(left->GetPhone(),right->GetPhone());
		case REPOHENT_MEMBER_NIP:
			return left->GetNIP().Compare(right->GetNIP());
		case REPOHENT_MEMBER_WETNO:
			return left->GetWetNo().Compare(right->GetWetNo());
		case REPOHENT_MEMBER_EXTRAS:
			return PolishStringCompare(left->GetExtras(),right->GetExtras());
		case REPOHENT_MEMBER_PESEL:
			return left->GetPESEL().Compare(right->GetPESEL());
		case REPOHENT_MEMBER_REGON:
			return left->GetREGON().Compare(right->GetREGON());
		case REPOHENT_MEMBER_IDNO:
			return left->GetIdNo().Compare(right->GetIdNo());
		case REPOHENT_MEMBER_ISSUEDATE:
			return CompareDateTime(left->GetIssueDate(),right->GetIssueDate());
		case REPOHENT_MEMBER_ISSUEPOST:
			return PolishStringCompare(left->GetIssuePost(),right->GetIssuePost());
		case REPOHENT_MEMBER_ACCOUNTNO:
			return left->GetAccountNo().Compare(right->GetAccountNo());
		case REPOHENT_MEMBER_BANKNAME:
			return PolishStringCompare(left->GetBankName(),right->GetBankName());
		case REPOHENT_MEMBER_WETLICENCENO:
			return PolishStringCompare(left->GetWetLicenceNo(),right->GetWetLicenceNo());
		case REPOHENT_MEMBER_CELLPHONENO:
			return PolishStringCompare(left->GetCellPhoneNo(),right->GetCellPhoneNo());
		case REPOHENT_MEMBER_EMAILADDRESS:
			return PolishStringCompare(left->GetEmailAddress(),right->GetEmailAddress());
		case REPOHENT_MEMBER_LATITUDE:
			return CompareNullable<Latitude>(left->GetLatitude(),right->GetLatitude());
		case REPOHENT_MEMBER_LONGITUDE:
			return CompareNullable<Longitude>(left->GetLongitude(),right->GetLongitude());
	}
	ASSERT(0);
	return COMPARE_LESS;
}
