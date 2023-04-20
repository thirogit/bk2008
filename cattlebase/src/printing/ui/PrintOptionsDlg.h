#if !defined(__PRINTDOCOPTDLG_H__INCLUDED_)
#define __PRINTDOCOPTDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../combo/FontCombo.h"
#include "../../ctrls/MarginsCtrl.h"
#include "../../printdocs/printdocs.h"
#include "../../printdocs/CustomPrintDocOptDlg.h"
#include "../../datatypes/CowDataComparator.h"
#include "../../Ctrls/floatformat/FloatFormatCtrl.h"

class CPrintDocOptDlg : public CDialog
{
public:
	CPrintDocOptDlg(PRINTDOCS *const pDoc,CWnd* pParent = NULL);   

	enum { IDD = IDD_PRINTOPT };
	~CPrintDocOptDlg();
protected:
	CFontCombo m_PrintFont;
	CSpinButtonCtrl m_HeightSpin;
	CMarginsCtrl m_OffMargs;
	CComboBox m_sortCombo;
	BOOL m_bSortOrder;
	CFloatFormatCtrl m_MoneyFormat;
	CFloatFormatCtrl m_WeightFormat;
	

	CCustomPrintDocOptDlg * m_pDocOptDlg;

	PRINTDOCS *const m_pDoc;

	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog() ;
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};


#endif 
