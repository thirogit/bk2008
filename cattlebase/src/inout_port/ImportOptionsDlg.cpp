#include "stdafx.h"
#include "ImportOptionsDlg.h"
#include "../classes/TempStringLoader.h"

    
CImportOptionsDlg::CImportOptionsDlg(ImportSettings& importSettings,CWnd* pParent /*=NULL*/) : 
		CBoolOptionsDlg(NULL,pParent)
{

	static BOOLOPTIONS importOptions[] = 
	{		
		{IDS_OPTDESCIMPORTPASSIMGS,			&importSettings.bImportPassImages},
		{IDS_OPTDESCINSERTMISSINGSTOCKS,	&importSettings.bImportInsertMissingStocks},	
		{IDS_OPTDESCINSERTMISSINGHENTS,		&importSettings.bImportInsertMissingHents},
		{IDS_OPTDESCIMPORTSAVEDOCDATES,		&importSettings.bImportSaveDocDates},
		{IDS_OPTDESCIMPORTSAVEORIGINALHENT,	&importSettings.bImportSaveOriginalHent},
		{0,NULL},
	};

	m_pBoolOptions = importOptions;
}


CImportOptionsDlg::~CImportOptionsDlg()
{
}

CString CImportOptionsDlg::GetDlgCaption()
{
	return CTempStringLoader(IDS_IMPORTOPTIONS);
}

