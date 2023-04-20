// CmComboBox.h : header file
//
#if !defined(_CMCOMBOBOX_H_)
#define _CMCOMBOBOX_H_

#include <map>
#include "cmString.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CAsComboBox window

class cmComboBox : public CComboBox
{
// Construction
public:
	cmComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmComboBox)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmComboBox();

	void Clear();

	virtual void AddItem(const cmString& s);
	virtual void AddItem(const cmString& s, double sortKey);
	virtual void AddItem(const cmString& s, int sortKey);
	virtual void AddItem(const cmString& s, const cmString& sortKey);

	virtual void SelectItem(double sortKey);
	virtual void SelectItem(int sortKey);
	virtual void SelectItem(const cmString& sortKey);

	virtual bool GetSelKey(double& sortKey);
	virtual bool GetSelKey(int& sortKey);
	virtual bool GetSelKey(cmString& sortKey);
	virtual cmString GetSelString();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmComboBox)
	afx_msg void OnDropdown();
	afx_msg void OnCloseup();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSelchange();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();
	
	void Draw(CDC* pDC, CRect clientRect);
	void DrawComboButton(CDC* pDC, CRect clientRect);
	void DrawBackground(CDC* pDC, CRect clientRect);
	void BuildList();

	CRect listRect_;
	CRect basicRect_;
	HWND hwndList_;
	bool droppedDown_;
	int dropAreaHeight_;
	CRect buttonArea_;

	map<cmString, cmString> sortKeyToValueMap_;
	map<cmString, cmString> valueToSortKeyMap_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CMCOMBOBOX_H_)
