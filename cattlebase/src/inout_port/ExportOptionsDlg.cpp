#include "stdafx.h"
#include "ExportOptionsDlg.h"
#include "../classes/TempStringLoader.h"

    
CExportOptionsDlg::CExportOptionsDlg(ExportSettings& exportSettings,CWnd* pParent /*=NULL*/) : 
		CBoolOptionsDlg(NULL,pParent)
{

	static BOOLOPTIONS exportOptions[] = 
	{
		{IDS_OPTDESCEXPORTINVOICES, &exportSettings.bExportInvoices},
		{IDS_OPTDESCEXPORTPASSIMGS, &exportSettings.bExportPassImages},
		{IDS_OPTDESCEXPORTSELLBUYPRICE, &exportSettings.bExportSellBuyPrice},	
		{IDS_OPTDESCEXPORTBUYSELLVALUES, &exportSettings.bExportBuySellValues},
		{IDS_OPTDESCEXPORTMYPRICEASMYPRICE, &exportSettings.bExportMyPriceAsMyPrice},
		{0,NULL},
	};


	m_pBoolOptions = exportOptions;
}


CExportOptionsDlg::~CExportOptionsDlg()
{
}

CString CExportOptionsDlg::GetDlgCaption()
{
	return CTempStringLoader(IDS_EXPORTOPTIONS);
}

