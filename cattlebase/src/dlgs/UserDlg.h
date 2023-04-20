#ifndef USERDLG_H_INCLUDED
#define USERDLG_H_INCLUDED

#pragma once

#include "../ctrls/BaseListCtrl.h"

class CUsrList : public CBaseListCtrl
{
public:
	CUsrList();
	void UpdateItemCount();
protected:
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual void SortByCol(int Column,bool bAsc);
private:
	static const GRIDHEADERS szUserListCols[];

};

// CUserDlg dialog

class CUserDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CUserDlg)

public:
	CUserDlg();
	virtual ~CUserDlg();
	
    
// Dialog Data
	enum { IDD = IDD_USERS };
private:
	CUsrList m_users;
	//CUserDataArray m_userary;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAdduser();
	afx_msg void OnDelUser();
	afx_msg void OnChange();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
};

#endif