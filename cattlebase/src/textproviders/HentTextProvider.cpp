#include "stdafx.h"
#include "HentTextProvider.h"
#include "../consts.h"
#include "../datatypes/HentComparator.h"


CString HentTextProvider::GetDataText(Hent *pData,UINT memberID)
{
	CString retStr;
   	switch(memberID)
	{
		case HENTS_SORT_BY_NAME:
			retStr =  pData->GetName();
			break;
		case HENTS_SORT_BY_ARIMRNO:
			retStr = pData->GetFarmNo();
			break;
		case HENTS_SORT_BY_ALIAS:
			retStr = pData->GetAlias();
			break;
		case HENTS_SORT_BY_STREET:
			retStr = pData->GetStreet();
			break;
		case HENTS_SORT_BY_POBOX:
			retStr = pData->GetPOBox();
			break;
		case HENTS_SORT_BY_CITY:
			retStr = pData->GetCity();
			break;
		case HENTS_SORT_BY_ZIP:
			retStr = pData->GetZip();
			break;
		case HENTS_SORT_BY_PLATE:
			retStr = pData->GetPlate();
			break;
		case HENTS_SORT_BY_HENTTYPE:
			retStr = pData->GetHentType()->henttypename;
			break;
		case HENTS_SORT_BY_SYNCIT:
			retStr = g_szYesNo[pData->GetSyncIt() ? 1 : 0];
			break;
		case HENTS_SORT_BY_COMBOITEM:
			retStr = pData->GetComboItemText();
			break;
		case HENTDATA_MEMBER_PHONE:
			retStr = pData->GetPhone();
			break;
		case HENTDATA_MEMBER_NIP:
			retStr = pData->GetNIP();
			break;
		case HENTDATA_MEMBER_WETNO:
			retStr = pData->GetWetNo();
			break;
		case HENTDATA_MEMBER_EXTRAS:
			retStr = pData->GetExtras();
			break;
		case HENTDATA_MEMBER_COUNTRY:
			retStr = pData->GetCountry()->GetCountryName();
			break;
		case HENTDATA_MEMBER_PESEL:
			retStr = pData->GetPESEL();
			break;
		case HENTDATA_MEMBER_REGON:
			retStr = pData->GetREGON();
			break;
		case HENTDATA_MEMBER_IDNO:
			retStr = pData->GetIdNo();
			break;
		case HENTDATA_MEMBER_ISSUEDATE:
			retStr = (LPCSTR)pData->GetIssueDate().GetDayDate();
			break;
		case HENTDATA_MEMBER_ISSUEPOST:
			retStr = pData->GetIssuePost();
			break;
		case HENTDATA_MEMBER_ACCOUNTNO:
			retStr = pData->GetAccountNo();
			break;
		case HENTDATA_MEMBER_BANKNAME:
			retStr = pData->GetBankName();
			break;
		case HENTDATA_MEMBER_WETLICENCENO:
			retStr = pData->GetWetLicenceNo();
			break;
		case HENTDATA_MEMBER_CELLPHONENO:
			retStr = pData->GetCellPhoneNo();
			break;
		case HENTDATA_MEMBER_EMAILADDRESS:
			retStr = pData->GetEmailAddress();
			break;
		case HENTDATA_MEMBER_LATITUDE:
			retStr = pData->GetLatitude().ToString();
			break;
		case HENTDATA_MEMBER_LONGITUDE:
			retStr = pData->GetLongitude().ToString();
			break;
		default:
			ASSERT(0);
			break;
	}	
	return retStr;
}
