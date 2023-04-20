#ifndef __HENTCOMBOCELL_H__
#define __HENTCOMBOCELL_H__


#include "SimpleHentCombo.h"

class gxHentComboCell : public CSimpleHentCombo
{
public:

	gxHentComboCell (CListCtrl* pCtrl, int iItem, int iSubItem, Hent *curSel = NULL);
    virtual ~gxHentComboCell();
    void    SetListSubItem();
 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void ReloadCombo(Hent *curSel = NULL);

protected:
	CListCtrl* pListCtrl;
    int			Item;
    int			SubItem;
    Hent	*initSelection;

	BOOL m_bESC;
	int  m_prevItem;
    
    
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnNcDestroy();
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnCloseup();
	
	DECLARE_MESSAGE_MAP()

};

#endif 
