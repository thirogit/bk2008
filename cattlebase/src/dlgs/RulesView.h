#pragma once


#include "global_arrays.h"
#include "../ctrls/BaseListCtrl.h"
#include "..\imgbutton\ImageButton.h"


enum RULELISTVIEWCOLS
{
	COL_RULENAME,
	COL_LIMWEIGHT,
	COL_PRICEUNDERXY,
	COL_PRICEOVERXY,
	COL_PRICEUNDERXX,
	COL_PRICEOVERXX
};

class CRuleListCtrl : public CBaseListCtrl
{
public:
	CRuleListCtrl();

	void UpdateItemCount();
private:

	static const GRIDHEADERS szRuleListCols[]; 
protected:
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	
	DECLARE_MESSAGE_MAP()
};

// CRulesView dialog


class CRulesView : public CDialog
{
	DECLARE_DYNAMIC(CRulesView)

public:
	CRulesView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRulesView();

// Dialog Data
	enum { IDD = IDD_LISTEDITDLG };
private:
	CRuleListCtrl  m_rules;
	CImageButton  m_OKBtn;
	CImageButton  m_EditBtn;
	CImageButton  m_AddBtn;
	CImageButton  m_DeleteBtn;

	void UpdateRuleListCount();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	void EditRule(int iListIndex);
	virtual BOOL OnInitDialog();

	afx_msg void OnAddRule();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelRule();
	afx_msg void OnRuleEdit();
	DECLARE_MESSAGE_MAP()
};
