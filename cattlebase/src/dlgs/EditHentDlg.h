#ifndef __EDITHENTDLG_H__
#define __EDITHENTDLG_H__

#include "BaseHentDlg.h"

class CEditHentDlg : public CBaseHentDlg
{
public:
	CEditHentDlg(Hent *pHent2Edit,CWnd* pParent = NULL);
			
protected:
	Hent *m_pHent2Edit;
	virtual CString GetWindowCaption();
	virtual bool CommitHent();
	virtual void SetupDialogControls();
	virtual void InitDialogControls();
	void UpdateHent(Hent *pHent);
	

};

#endif