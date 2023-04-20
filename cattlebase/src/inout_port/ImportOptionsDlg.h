#ifndef __IMPORTOPTIONSDLG_H__
#define __IMPORTOPTIONSDLG_H__

#include "BoolOptionsDlg.h"
#include "ImportSettings.h"

class CImportOptionsDlg : public CBoolOptionsDlg
{
public:
	CImportOptionsDlg(ImportSettings& importSettings,CWnd* pParent = NULL);   
	virtual ~CImportOptionsDlg();
protected:
	virtual CString GetDlgCaption();

	
};

#endif