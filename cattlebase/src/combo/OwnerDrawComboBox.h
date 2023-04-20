#ifndef __OWNERDRAWCOMBOBOX_H__
#define __OWNERDRAWCOMBOBOX_H__

#include "PtrFlushArray.h"

class ItemDrawer
{
public:
	virtual void Draw( LPDRAWITEMSTRUCT lpDrawItemStruct ) = 0;
};

class COwnerDrawListBox : public CListBox
{
public:
	void SetItemCount(int iCount);
	void SetItemDrawer(ItemDrawer* pItemDrawer);
private:
	ItemDrawer*		m_pItemDrawer;

};


class COwnerDrawComboBox : public CComboBox
{
public:
	
public:
	COwnerDrawComboBox();
	virtual ~COwnerDrawComboBox();
	void SetItemCount(int iCount);
	void SetItemDrawer(ItemDrawer* pItemDrawer);

private:

	CString			m_itemSearchBuffer;
	bool            m_bLastSearch;
	ItemDrawer*		m_pItemDrawer;
	COwnerDrawListBox m_listbox;

	// Tooltip functions
	void CreateToolTip (CWnd *pParent);
	void SetToolTipText (const TCHAR *pszText,bool bRedBackgroud);
	void DisplayToolTip (const bool bDisplay);

	CToolTipCtrl m_ToolTip;
	

protected:

	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	void MeasureItem(LPMEASUREITEMSTRUCT);
	int CompareItem(LPCOMPAREITEMSTRUCT);

	BOOL PreTranslateMessage(MSG* pMsg);
	void PreSubclassWindow();
	void UpdateToolTip();
	virtual int SearchForItem(const CString &searchFor) = 0;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCloseup();
	afx_msg void OnCBKillFocus();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDestroy();
	
	DECLARE_MESSAGE_MAP()
};


#endif 
