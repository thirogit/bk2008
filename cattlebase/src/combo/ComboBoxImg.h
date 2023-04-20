#pragma once

class CComboBoxImg : public CComboBoxEx
{
	DECLARE_DYNAMIC(CComboBoxImg)
public:
	CComboBoxImg();
	virtual ~CComboBoxImg();

	int AddItem(LPCTSTR pszText, int iImage);
	BOOL SelectString(LPCTSTR pszText);

protected:
	bool SelectFirstStringIfStartsWith(const CString& startStr);
	void UpdateHorzExtent(int iIconWidth);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void PreSubclassWindow();
	
private:
	DWORD m_LastKeyPressTick;
	CString m_itemSearchBuffer;


	CToolTipCtrl m_ToolTip;
	TOOLINFO m_ToolInfo;
	void DisplayToolTip (const bool bDisplay);
	void CreateToolTip();
	void SetToolTipText (const TCHAR *pszText);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCBKillFocus();
	afx_msg void OnCloseup();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};


