#include "stdafx.h"
#include "BoolOptionsDlg.h"
#include "../classes/TempStringLoader.h"

//#include "ExportSettings.h"
//
//const CBoolOptionsDlg::BOOLOPTIONS CBoolOptionsDlg::_exportOptions[] =
//{
//	{IDS_OPTDESCEXPORTINVOICES, &ExportSettings::bExportInvoices},
//	{IDS_OPTDESCEXPORTPASSIMGS, &ExportSettings::bExportPassImages},
//	{IDS_OPTDESCEXPORTSELLBUYPRICE, &ExportSettings::bExportSellBuyPrice},	
//	{IDS_OPTDESCEXPORTBUYSELLVALUES, &ExportSettings::bExportBuySellValues},
//	{IDS_OPTDESCEXPORTMYPRICEASMYPRICE, &ExportSettings::bExportMyPriceAsMyPrice},
//	{0,NULL},
//};
    
CBoolOptionsDlg::CBoolOptionsDlg(BOOLOPTIONS* pBoolOptions,CWnd* pParent /*=NULL*/) : 
		CCheckListDlg(pParent),m_pBoolOptions(pBoolOptions)
{
}


CBoolOptionsDlg::~CBoolOptionsDlg()
{
}

void CBoolOptionsDlg::SaveAndUse()
{
	SetOptions();
	EndDialog(IDSAVEANDUSE);	
}


void CBoolOptionsDlg::UseOnlyOnce()
{
	SetOptions();
	EndDialog(IDUSEONLYONCE);
}

void CBoolOptionsDlg::Init()
{
	int i = 0 ;
	while(m_pBoolOptions[i].nResIdOptionName != 0)
	{
		AddOption(CTempStringLoader(m_pBoolOptions[i].nResIdOptionName),
		   		  m_pBoolOptions[i].bValuePtr);
		i++;
	}
}