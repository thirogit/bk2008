#if !defined(__COMBOCELL_TEMPLATE_H__)
#define __COMBOCELL_TEMPLATE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EditCellTemplate.h"
#include "../combo/StockCombo.h"
#include "../combo/SexCombo.h"

template<class BASECLASS>
class CComboCellTemplate : public BASECLASS
{
public:

	CComboCellTemplate (CListCtrl* pCtrl, int iItem, int iSubItem, void *curSel = NULL);
	
    //virtual ~CComboCellTemplate();
protected:
    void    SetListSubItem();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
		
protected:
	CListCtrl* pListCtrl;
    int			Item;
    int			SubItem;
    void	*initSelection;

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
/////////////////////////////////////////////////////////////////////////////
//template CBasePtrCombo<CComboBox,StockData>;
//template CComboCellTemplate<CStockCombo>;

/*class gxStockComboCell : public CComboCellTemplate<CStockCombo>
{
public:
	gxStockComboCell(CListCtrl* pCtrl, int iItem, int iSubItem, void *curSel = NULL) :
		CComboCellTemplate<CStockCombo>(pCtrl,iItem,iSubItem,curSel)
		{};


		};*/

/*
class gxSexComboCell : public CComboCellTemplate<CSexCombo>
{
public:
	gxSexComboCell(CListCtrl* pCtrl, int iItem, int iSubItem, void *curSel = NULL) :
		CComboCellTemplate<CSexCombo>(pCtrl,iItem,iSubItem,curSel)
		{};

};*/

#endif // !defined(AFX_INPLACECOMBO_H__2E04D8D9_827F_4FBD_9E87_30AF8C31639D__INCLUDED_)