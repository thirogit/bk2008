#ifndef __CHOOSECOWENADLG_H__INCLUDED__
#define __CHOOSECOWENADLG_H__INCLUDED__

#include "../../DataTypes.h"
#include "../../ctrls/CowListCtrl.h"

#pragma once


// CChooseCowEANDlg dialog

class CChooseCowEANDlg : public CDialog
{	
public:
	static Cow* ChooseCowEAN(CCowArray &inputArray,CWnd* pParent = NULL);
private:
	CChooseCowEANDlg(CCowArray &cowArray,CWnd* pParent = NULL);   // standard constructor
	virtual ~CChooseCowEANDlg();

// Dialog Data
	enum { IDD = IDD_CHOOSECOWBYEANDLG };
private:
	CCowArray m_cowArray;
	CCowListCtrl m_cowList;

	Cow *m_pSelectedCow;

	static const GRIDHEADERS m_ChooseCowEANListHeader[];
protected:

	void OnOK();
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#endif