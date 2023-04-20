#ifndef __IMAGECOMBOBOX_H__
#define __IMAGECOMBOBOX_H__

#include "PtrFlushArray.h"

class CImageComboBox : public CComboBox
{
public:
	struct ImageComboItem
	{
		COLORREF crTextColor;
		int nImageIndex;
		CString itemText;
		BOOL bBold;
		DWORD dwItemData;
	};

public:
	CImageComboBox();
	virtual ~CImageComboBox();

	void SetImageList(CImageList* pImageList);
	void SetItemBold(int nItemIndex, BOOL bBold = TRUE);
	void SetItemImage(int nItemIndex, int nImageIndex);
	void SetItemColor(int nItemIndex, COLORREF rcTextColor);
	void SetItemText(int nItemIndex, const CString& itemText);
	void SetItemData(int nItemIndex, DWORD dwData);
	DWORD GetItemData(int nItemIndex);
	CString GetItemText(int nItemIndex);
	BOOL GetItemBold(int nItemIndex);
	ImageComboItem* GetImageComboItem(int nItemIndex);

	int AddItem(const CString& itemText,int nImageIndex);


private:
	CImageList* m_pImageList;

	PtrFlushArray<ImageComboItem> m_items;

	CString m_itemSearchBuffer;


	bool            m_bLastSearch;


	
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
//	void CreateToolTip();
//	void ShowToolTip(bool bShow);
//	void SetToolTipText(LPCSTR szText,bool bRedBackground);
	void UpdateToolTip();
	int SearchForItem(const CString &searchFor);
	
	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCloseup();
	afx_msg void OnCBKillFocus();
	
	DECLARE_MESSAGE_MAP()
};


#endif 
