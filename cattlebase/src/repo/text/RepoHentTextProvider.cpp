#include "stdafx.h"
#include "../text/RepoHentTextProvider.h"
#include "../compare/RepoHentComparator.h"
#include "../utils/RepoHentTypeString.h"

CString RepoHentTextProvider::GetDataText(RepoHent *pData,UINT memberId)
{
	CString sText;
   	switch(memberId)
	{
		case REPOHENT_MEMBER_NAME:
			sText =  pData->GetName();
			break;
		case REPOHENT_MEMBER_FARMNO:
			sText = pData->GetFarmNo();
			break;
		case REPOHENT_MEMBER_ALIAS:
			sText = pData->GetAlias();
			break;
		case REPOHENT_MEMBER_STREET:
			sText = pData->GetStreet();
			break;
		case REPOHENT_MEMBER_POBOX:
			sText = pData->GetPOBox();
			break;
		case REPOHENT_MEMBER_CITY:
			sText = pData->GetCity();
			break;
		case REPOHENT_MEMBER_ZIP:
			sText = pData->GetZip();
			break;
		case REPOHENT_MEMBER_PLATE:
			sText = pData->GetPlate();
			break;
		case REPOHENT_MEMBER_HENTTYPE:			
			sText = RepoHentTypeString(pData->GetHentType()).ToString();
			break;
		case REPOHENT_MEMBER_PHONE:
			sText = pData->GetPhone();
			break;
		case REPOHENT_MEMBER_NIP:
			sText = pData->GetNIP();
			break;
		case REPOHENT_MEMBER_WETNO:
			sText = pData->GetWetNo();
			break;
		case REPOHENT_MEMBER_EXTRAS:
			sText = pData->GetExtras();
			break;
		case REPOHENT_MEMBER_PESEL:
			sText = pData->GetPESEL();
			break;
		case REPOHENT_MEMBER_REGON:
			sText = pData->GetREGON();
			break;
		case REPOHENT_MEMBER_IDNO:
			sText = pData->GetIdNo();
			break;
		case REPOHENT_MEMBER_ISSUEDATE:
			sText = (LPCSTR)pData->GetIssueDate().GetDayDate();
			break;
		case REPOHENT_MEMBER_ISSUEPOST:
			sText = pData->GetIssuePost();
			break;
		case REPOHENT_MEMBER_ACCOUNTNO:
			sText = pData->GetAccountNo();
			break;
		case REPOHENT_MEMBER_BANKNAME:
			sText = pData->GetBankName();
			break;
		case REPOHENT_MEMBER_WETLICENCENO:
			sText = pData->GetWetLicenceNo();
			break;
		case REPOHENT_MEMBER_CELLPHONENO:
			sText = pData->GetCellPhoneNo();
			break;
		case REPOHENT_MEMBER_EMAILADDRESS:
			sText = pData->GetEmailAddress();
			break;
		case REPOHENT_MEMBER_LATITUDE:
			sText = pData->GetLatitude().ToString();
			break;
		case REPOHENT_MEMBER_LONGITUDE:
			sText = pData->GetLongitude().ToString();
			break;
		default:
			ASSERT(0);
			break;
	}	
	return sText;
}
