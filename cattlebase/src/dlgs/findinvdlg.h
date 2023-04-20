#ifndef __FINDINVDLG_H__
#define __FINDINVDLG_H__


#include "resource.h"

#include "ctrls\DateSpan.h"
#include "CattleSQL.h"

#include "findbasedlg.h"
#include "..\combo\ReasonCombo.h"
#include "..\ctrls\NumFindCtrl.h"
#include "..\combo\PayWayCombo.h"

class CFindInvDlg : public  CFindBaseDlg
{
public:
	CFindInvDlg ();

	enum { IDD = IDD_FINDINVOICE };


	enum RADIO_BTNS
	{
		FIND_VAT_OUTINVOICE  = 0,
		FIND_RR_ININVOICE = 2,
		FIND_VAT_ININVOICE = 1
	};

	enum PAID_SEARCH_TYPE
	{
		PAID_NO = 0,
		PAID_YES = 1,
		PAID_ONDATE = 2,
		PAID_NOSEARCH = -1
	};

	CString      m_invnofrom;
	CString      m_invnoto;
	BOOL      m_bByInvDate;
	int      m_invTypeRadio;
	
	BOOL	  m_bSearchForCorrect;
	
		
	CDateSpan m_invdatespan;
	

	CFloatFindCtrl  m_vatrate;

	CFindPaywayCombo m_payway;

	CDateSpan m_paiddate;
	CComboBox m_paid;

	CFloatFindCtrl m_total;
	
	int GoSearch(CFindSetup& setup);	

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:

	afx_msg void UpdateDateSpans();
	afx_msg void OnPaidChange();
	DECLARE_MESSAGE_MAP()
};


#endif