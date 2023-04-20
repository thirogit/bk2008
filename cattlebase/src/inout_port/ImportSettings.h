#ifndef __IMPORTSETTINGS_H__
#define __IMPORTSETTINGS_H__

#include "BoolSettings.h"

class ImportSettings : public BoolSettings
{
public:
	ImportSettings();
	virtual void Load();
	virtual void Save();
		
	bool bImportPassImages;
	bool bImportInsertMissingStocks;	
	bool bImportInsertMissingHents;
	bool bImportSaveDocDates;
	bool bImportSaveOriginalHent;

};

#endif