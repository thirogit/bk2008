#pragma once

#include "ctrls\FarmNoEdit.h"

// CFastFindHent dialog

class CFastFindHent : public CDialog
{
	DECLARE_DYNAMIC(CFastFindHent)

public:
	CFastFindHent(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFastFindHent();

// Dialog Data
	enum { IDD = IDD_EDITBOXDLG };

	BOOL GetHentNo(CString &hentNo);

protected:
	CFarmNoEdit	  m_hentNoEdit;
	CDialog::DoModal;
	CString m_hentNo;

	virtual BOOL OnInitDialog();
	void OnOK();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
