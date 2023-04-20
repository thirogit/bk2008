#pragma once

#include "../combo/StockCombo.h"
#include "../combo/VATCombo.h"
#include "../combo/PaywayCombo.h"
#include "../ctrls/CountryCombo.h"
#include "../combo/ReasonCombo.h"
#include "../combo/SexCombo.h"
#include "../combo/HentTypeCombo.h"

// CPreSelOpt dialog

class CPreSelOpt : public CPropertyPage
{
	DECLARE_DYNAMIC(CPreSelOpt)

public:
	CPreSelOpt();   // standard constructor
	virtual ~CPreSelOpt();

// Dialog Data
	enum { IDD = IDD_PRESEL };
protected:
	CStockCombo  m_stocks;
	CVATCombo    m_vatVAT;
	CPaywayCombo m_paywayVATcombo;

	CVATCombo    m_vatRR;
	CPaywayCombo m_paywayRRcombo;

	BOOL         m_setinreason;
	BOOL         m_setoutreason;
	BOOL         m_sethenttypes;
	BOOL         m_setstocks;

	BOOL		 m_setpaywayVAT;
	double       m_VATRateVAT;
	Invoice::PAYWAYS m_paywayVAT;

	BOOL		 m_setpaywayRR;
	double       m_VATRateRR;
	Invoice::PAYWAYS m_paywayRR;

	int          m_inreasonid;
	int          m_outreasonid;
	int          m_henttypeid;
	COWSEX       m_cowsex;
	Stock*   m_stock;
	
	CReasonCombo m_inreasons;
	CReasonCombo m_outreasons;
	CSexCombo    m_sexes;
	CHentTypeCombo m_henttypes;

	
	CSpinButtonCtrl m_spnPayDueDaysVAT;
	int m_payduedaysVAT;
	CSpinButtonCtrl m_spnPayDueDaysRR;
	int m_payduedaysRR;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	BOOL OnApply();
	void SetPayWayCfgItem(CPaywayCombo& payWayCombo,UINT cfgItemId);
	

	afx_msg void UpdateControls();
	afx_msg void OnVATComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVATChange();
	afx_msg void OnStockSelChange();
	DECLARE_MESSAGE_MAP()
};
