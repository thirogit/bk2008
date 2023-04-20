#ifndef _CCHECKSK_H_
#define _CCHECKSK_H_

#include <afxcmn.h>
#include "TextStyle.h"
//#define	ICNST_OK				0
//#define	ICNST_INVALIDRESOURCE   1

enum ORIENTATION 
{
ORIENTATION_HORIZONTAL	=	0,
ORIENTATION_VERTICAL_LEFT	= 900,
ORIENTATION_VERTICAL_RIGHT	= -900,
ORIENTATION_HORIZONTAL_FLIPED = 1800
};



//  ===========================================================================
//  class   CCheckSK
//  desc    This class can be used to skin check boxes
//          To use this class draw check boxe with the dialog editor and create
//          control variable of type CCheckSK for it and then use 
//          the public methods
//  ===========================================================================
class CCheckSK : public CButton
{
    
public:
  
   
    //  =======================================================================
    //  desc    Retrieves check status of the Led
    //  =======================================================================
	BOOL GetCheck();

    //  =======================================================================
    //  desc    set/reset check mark or Led on condition
    //  arg1    TRUE sets, FALSE resets
    //  arg2    TRUE repaints the control
    //  =======================================================================
	void SetCheck(BOOL bCheck, BOOL bRepaint=TRUE);
        
 	virtual ~CCheckSK();
	CCheckSK(ORIENTATION iOrientation = ORIENTATION_HORIZONTAL);

	int Create(CRect &rc,CWnd *pParentWnd,UINT nID);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckSK)
	private:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	private:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	
	// Generated message map functions
private:
	//{{AFX_MSG(CCheckSK)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void FreeResources();
	LONG m_nStyle;
	void CancelHover();
	BOOL m_bMouseOver;
    LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
    LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam);

	BOOL m_bCheck;
	BOOL m_bIsDisabled;
	BOOL m_bIsFocussed;
	BOOL m_bIsPressed;

	ORIENTATION m_Orientation;
};

#endif // _CCHECKSK_H_
