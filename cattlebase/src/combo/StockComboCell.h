#ifndef __STOCKCOMBOCELL_H__
#define __STOCKCOMBOCELL_H__


#include "StockCombo.h"

class gxStockComboCell : public CStockCombo
{
public:

	gxStockComboCell (CListCtrl* pCtrl, int iItem, int iSubItem, Stock *curSel = NULL);
    virtual ~gxStockComboCell();
    void    SetListSubItem();
 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void ReloadCombo(Stock *curSel = NULL);

protected:
	CListCtrl* pListCtrl;
    int			Item;
    int			SubItem;
    Stock	*initSelection;

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
