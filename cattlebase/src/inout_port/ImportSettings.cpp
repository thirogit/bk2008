#include "stdafx.h"
#include "ImportSettings.h"
#include "MainCfg.h"

ImportSettings::ImportSettings() : 
	bImportPassImages(false),
	bImportInsertMissingStocks(false),
	bImportInsertMissingHents(false),
	bImportSaveDocDates(false),
	bImportSaveOriginalHent(false)
{
}

void ImportSettings::Load()
{
	bImportPassImages = AppCfg().GetBool(CFG_IMPORTPASSIMGS) !=  FALSE;
	bImportInsertMissingStocks = AppCfg().GetBool(CFG_INSERTMISSINGSTOCKS) !=  FALSE;	
	bImportInsertMissingHents = AppCfg().GetBool(CFG_INSERTMISSINGHENTS) !=  FALSE;
	bImportSaveDocDates = AppCfg().GetBool(CFG_SAVEDOCDATES) !=  FALSE;
	bImportSaveOriginalHent = AppCfg().GetBool(CFG_SAVEORIGINALHENT) !=  FALSE;
}

void ImportSettings::Save()
{
	AppCfg().SetBool(CFG_IMPORTPASSIMGS,bImportPassImages);
	AppCfg().SetBool(CFG_INSERTMISSINGSTOCKS,bImportInsertMissingStocks);
	AppCfg().SetBool(CFG_INSERTMISSINGHENTS,bImportInsertMissingHents);
	AppCfg().SetBool(CFG_SAVEDOCDATES,bImportSaveDocDates);
	AppCfg().SetBool(CFG_SAVEORIGINALHENT,bImportSaveOriginalHent);
}
	
	