#pragma once

#include "../../ctrls/BaseListCtrl.h"

class CAddItemBaseDlg : public CDialog
{
public:
	CAddItemBaseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddItemBaseDlg();
	int GetSelectedItemIndex();

// Dialog Data
	enum { IDD = IDD_ADDITEM };
protected:
	BOOL OnInitDialog();
	void OnOK();
protected:
	CBaseListCtrl *m_itemList;
	int m_selIndex;	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
