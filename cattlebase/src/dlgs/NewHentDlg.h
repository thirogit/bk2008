#ifndef __NEWHENTDLG_H__
#define __NEWHENTDLG_H__

#include "BaseHentDlg.h"

class CNewHentDlg : public CBaseHentDlg
{
public:
	CNewHentDlg(CWnd* pParent = NULL);
	CNewHentDlg(Hent* pSetHent,CWnd* pParent = NULL);
	Hent *GetNewHent();
		
protected:
	Hent *m_NewHent;
	Hent m_setHent;

	virtual CString GetWindowCaption();
	virtual bool CommitHent();
	virtual void InitDialogControls();
	

};

#endif