#ifndef __EXPORTSETTINGS_H__
#define __EXPORTSETTINGS_H__

#include "BoolSettings.h"

class ExportSettings : public BoolSettings
{
public:

	ExportSettings();
	virtual void Load();
	virtual void Save();
	
	bool bExportInvoices;
	bool bExportPassImages;
	bool bExportSellBuyPrice;	
	bool bExportBuySellValues;
	bool bExportMyPriceAsMyPrice;

};



#endif