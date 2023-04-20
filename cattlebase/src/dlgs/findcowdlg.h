#ifndef __FINDCOWDLG_H__
#define __FINDCOWDLG_H__

#include "stdafx.h"
#include "..\ctrls\DateSpan.h"
#include "..\FilterEdit\FilterEdit.h"
#include "..\classes\types\DateTime.h"
#include "..\cattleSQL.h"
#include "..\combo\FindSexCombo.h"
#include "..\combo\StockCombo.h"
#include "..\combo\TTComboBox.h"
#include "..\ctrls\NumFindCtrl.h"
#include "..\combo\SimpleHentCombo.h"
#include "findbasedlg.h"
#include "findbasedlg.h"
#include "../ctrls/WildFilterEdit.h"

class CFindCowDlg : public  CFindBaseDlg
{
public:
	CFindCowDlg ();

// Dialog Data
	//{{AFX_DATA(CFindCowDlg)
	enum { IDD = IDD_FINDCOW };
	//}}AFX_DATA

	CDateSpan	 m_passdatespan;
	CDateSpan	 m_birthdatespan;
	CWildFilterEdit	 m_EAN;
	CWildFilterEdit	 m_motherEAN;
	CWildFilterEdit  m_PassNo;
	CFindSexCombo  m_sex;
//	CString		 m_weight;
//	CComboBox	 m_rangeweight;
	CString		 m_birthplace;
	CFindHentCombo  m_fstownr;
	CFindHentCombo	 m_deliver;
	CFindHentCombo  m_buyer;
	CFindStockCombo	 m_stock;
	CFloatFindCtrl    m_weightrange;
	CFloatFindCtrl  m_buypricerange;
	CFloatFindCtrl  m_sellpricerange;

	CFloatFindCtrl    m_buyweightrange;
	CFloatFindCtrl    m_sellweightrange;
	CFindStockCombo	 m_buystock;
	CFindStockCombo	 m_sellstock;

	
	//CRegEx m_RegEx;

	BOOL		m_bPassEnable;
	BOOL	    m_bBirthEnable;
	BOOL        m_bInCow;

	int GoSearch(CFindSetup& setup);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindCowDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL
// Implementation
protected:
	//{{AFX_MSG(CFindCowDlg)
	
	afx_msg void OnSpanCheck(void);
		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif