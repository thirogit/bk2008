#if !defined(AFX_INPLACECOMBO_H__2E04D8D9_827F_4FBD_9E87_30AF8C31639D__INCLUDED_)
#define AFX_INPLACECOMBO_H__2E04D8D9_827F_4FBD_9E87_30AF8C31639D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../combo/RuleCombo.h"

class gxRuleComboCell : public CRuleCombo
{
public:

	gxRuleComboCell (CListCtrl* pCtrl, int iItem, int iSubItem, Rule *curSel = NULL);
    virtual ~gxRuleComboCell();
    void    SetListSubItem();
 
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	CListCtrl* pListCtrl;
    int			Item;
    int			SubItem;
    Rule	*initSelection;

	BOOL m_bESC;
	int  m_prevItem;
    
    //{{AFX_MSG(gxEditCell)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnNcDestroy();
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnCloseup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////




#endif // !defined(AFX_INPLACECOMBO_H__2E04D8D9_827F_4FBD_9E87_30AF8C31639D__INCLUDED_)