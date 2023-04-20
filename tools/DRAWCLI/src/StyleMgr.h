#pragma once

#include "TextStyle.h"

class CStyleMgr : public CDialog
{
	DECLARE_DYNAMIC(CStyleMgr)

public:
	CStyleMgr(CView *pView);   // standard constructor
	virtual ~CStyleMgr();
private:
	CListCtrl m_styles;
	CStyleList *m_styleList;	
	
protected:
	int FindNextStyleID(CStyleList *pStyleList);

// Dialog Data
	enum { IDD = IDD_STYLEMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnStyleDel();
	afx_msg void OnNewStyle();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
public:
	
};
