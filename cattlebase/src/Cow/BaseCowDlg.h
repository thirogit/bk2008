#ifndef __BASECOWDLG_H__
#define __BASECOWDLG_H__

#include "../datatypes/Cow.h"
#include "../maskededit/OXMaskedEdit.h"
#include "../imgbutton/ImageButton.h"
#include "../ctrls/ExtrasMemo.h"
#include "../ctrls/EanEdit.h"
#include "../combo/SexCombo.h"
#include "../combo/StockCombo.h"
#include "../ctrls/curdateedit/curdateedit.h"
#include "../combo/HentCombo.h"
#include "../ctrls/TimerTipWnd.h"
#include "../ctrls/WeightEdit.h"

class CBaseCowDlg : public CDialog
{
public:
	CBaseCowDlg(Cow *pCow,CWnd* pParent = NULL);
	enum { IDD = IDD_COW };	
	
protected:
	CEANEdit m_ean;
	CCurDateEdit  m_birthdate;
	COXMaskedEdit m_passno;
	CCurDateEdit  m_passdate;
	CSexCombo	  m_combosex;
	CHentCombo    m_hentowner;
	CStockCombo	  m_stock;
	CExtrasMemo   m_extras;
	CEANEdit      m_motherean;
	CFilterEdit   m_myprice;
	CWeightEdit	  m_weight;
	CString		  m_birthplace;
	CImageButton  m_CowAdd;
	CImageButton  m_CowCancel;
	CTimerTipWnd  m_UsedPreDefinedSexTip;
	CString		  m_HealthCertNo;

	CLongitudeEdit	m_Longitude;
	CLatitudeEdit	m_Latitude;


protected:
	
	
	Cow *m_pCow;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	virtual BOOL OnInitDialog() ;
	virtual CString GetWindowCaption() = 0;
	virtual void SetupControls() = 0;
	virtual bool CommitCow() = 0;
	

	bool Validate();
	Cow* CreateNewCow();
	

	virtual void OnOK();
protected:
	afx_msg void OnStockSelChange();
	afx_msg void OnAddStock();
	afx_msg void OnStockComboLostFocus();
	
	
	DECLARE_MESSAGE_MAP()

};

#endif