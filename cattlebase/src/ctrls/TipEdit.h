#ifndef __TIPEDIT_H__
#define __TIPEDIT_H__

#include "../maskededit/OXMaskedEdit.h"


class CTipEdit : public COXMaskedEdit
{
public :
	CTipEdit(LPCSTR pszMask = "");
	void SetAbortWnd(CWnd *pWnd);
	
protected:	
	virtual void PreSubclassWindow();
	virtual void PrepareToolTip(CWnd *pParent);

	// Tooltip functions
	void CreateToolTip (CWnd *pParent, const TCHAR *pszText,
		const int iIconType = TTI_NONE, const TCHAR *pszTitle = "");
	void SetToolTipText (const TCHAR *pszText);
	void SetToolTipTitle (const int iIconType, const TCHAR *pszTitle);
	void DisplayToolTip (const bool bDisplay);

	CToolTipCtrl m_ToolTip;
	CWnd *pAbortWnd;

	afx_msg void OnKillFocus (CWnd *pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	DECLARE_MESSAGE_MAP()

};


#endif