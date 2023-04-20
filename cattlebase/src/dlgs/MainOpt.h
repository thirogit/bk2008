#pragma once
#include "../combo/UserCombo.h"
#include "ctrls\curdateedit\curdateedit.h"
#include "..\FilterEdit\FilterEdit.h"
#include "ctrls\ColorOptionBtn.h"
#include "ctrls\ColorsCombo.h"
#include "combo\GtLtCombo.h"
// CMainOpt dialog

class CMainOpt : public CPropertyPage
{
	DECLARE_DYNAMIC(CMainOpt)

public:
	CMainOpt();
	virtual ~CMainOpt();

// Dialog Data
	enum { IDD = IDD_MAINOPT };

	

private:
	
	CUserCombo   m_autousers;
	CString      m_autopass;
	BOOL         m_autologin;
	int          m_autouserid;


	BOOL m_bDoubleAskDel;
	BOOL m_bWelcomeScreen;
	BOOL m_bAskAtExit;
	BOOL m_bCheckEANs;
	BOOL m_bMaskMyPrice;
	BOOL m_bMarkCowAge;
	BOOL m_bWarnCowAge;
	BOOL m_MarkBuySellLoss;
	BOOL m_bCheckFarmNo;

	BOOL m_bWarnEmptyPlateNo;

	BOOL m_bWarnToSmallPrice;
	Money m_WarnMinPrice;
	CFilterEdit m_MinPriceEdit;

	BOOL m_bWarnToBigProfit;
	Money m_MaxProfit;
	CFilterEdit m_MaxProfitEdit;
	COLORREF m_WarnToBigProfitClr;
	CColorOptionBtn	m_ctlWarnToBigProfitClr;

	CRegEx       m_REunsigned;

	int m_CowAgeDays;
	int m_WarnCowAgeDays;
	
	COLORREF m_MarkAgeColor;
	COLORREF m_WarnAgeColor;
	COLORREF m_MarkLossColor;

	CColorOptionBtn	m_ctlMarkAgeColour;
	CColorOptionBtn	m_ctlWarnAgeColour;
	CColorOptionBtn	m_ctlWarnLossColour;

	CColorsCombo m_grpClrCombo;

	BOOL m_bMarkMyPriceGtLtBuy;
	BOOL m_bMarkMyPriceGtLtSell;
	UINT m_nMarkMyPriceGtLtBuy;
	UINT m_nMarkMyPriceGtLtSell;

	CGtLtCombo m_MyPriceGtLtBuy;
	CGtLtCombo m_MyPriceGtLtSell;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	BOOL OnApply();

	afx_msg void AuthorizeUser();
	afx_msg void UpdateControls();
	afx_msg void OnVATComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVATChange();
	afx_msg void OnColorSelEnd();
	DECLARE_MESSAGE_MAP()
};
