#include "stdafx.h"
#include "ExportSettings.h"
#include "MainCfg.h"

ExportSettings::ExportSettings() : 
	bExportInvoices(false),
	bExportPassImages(false),
	bExportSellBuyPrice(false),
	bExportBuySellValues(false),
	bExportMyPriceAsMyPrice(false)
{
}

void ExportSettings::Load()
{
	bExportInvoices = AppCfg().GetBool(CFG_EXPORTINVOICES) !=  FALSE;
	bExportPassImages = AppCfg().GetBool(CFG_EXPORTPASSIMGS) !=  FALSE;	
	bExportSellBuyPrice = AppCfg().GetBool(CFG_EXPORTSELLBUYPRICE) !=  FALSE;
	bExportBuySellValues = AppCfg().GetBool(CFG_EXPORTBUYSELLVALUES) !=  FALSE;
	bExportMyPriceAsMyPrice = AppCfg().GetBool(CFG_EXPORTMYPRICEASMYPRICE) !=  FALSE;
}

void ExportSettings::Save()
{

	AppCfg().SetBool(CFG_EXPORTINVOICES,bExportInvoices);
	AppCfg().SetBool(CFG_EXPORTPASSIMGS,bExportPassImages);
	AppCfg().SetBool(CFG_EXPORTSELLBUYPRICE,bExportSellBuyPrice);

	AppCfg().SetBool(CFG_EXPORTBUYSELLVALUES,bExportBuySellValues);
	AppCfg().SetBool(CFG_EXPORTMYPRICEASMYPRICE,bExportMyPriceAsMyPrice);
}
	
	