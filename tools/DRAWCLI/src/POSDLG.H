
/////////////////////////////////////////////////////////////////////////////
// CPosDlg dialog
#ifndef __POSDLG_H__
#define __POSDLG_H__

#include "LBSpinButtonCtrl.h"
#include "RectDlg.h"

class CPosDlg : public CBaseDlg
{
	void _UpdateControls();
	
// Construction
public:

	DECLARE_DYNCREATE(CPosDlg);

	CPosDlg(CWnd* pParent = NULL); // standard constructor
	CPosDlg(RECT* rc);
// Dialog Data
	//{{AFX_DATA(CPosDlg)
	enum { IDD = IDD_PROP_POS };
	//}}AFX_DATA

	void DoChanges(CDrawObj *pObj);
protected:
	CLBSpinButtonCtrl	m_LeftSpin;
	CLBSpinButtonCtrl	m_RightSpin;
	CLBSpinButtonCtrl	m_TopSpin;
	CLBSpinButtonCtrl	m_BottomSpin;
	
	int		m_nLeft;
	int		m_nTop;
	int		m_nRight;
	int		m_nBottom;

	void	SetPos(RECT *rect);	
	void	GetPos(RECT *rect);

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CPosDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnInternalUpdateControls(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

#endif //__POSDLG_H__
/////////////////////////////////////////////////////////////////////////////
