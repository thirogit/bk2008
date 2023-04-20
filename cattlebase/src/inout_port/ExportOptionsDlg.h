#ifndef __EXPORTOPTIONSDLG_H__
#define __EXPORTOPTIONSDLG_H__

#include "BoolOptionsDlg.h"
#include "ExportSettings.h"

class CExportOptionsDlg : public CBoolOptionsDlg
{
public:
	CExportOptionsDlg(ExportSettings& exportSettings,CWnd* pParent = NULL);   
	virtual ~CExportOptionsDlg();
protected:
	virtual CString GetDlgCaption();

	
};

#endif