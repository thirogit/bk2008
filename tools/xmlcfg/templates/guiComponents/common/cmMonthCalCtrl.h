// cmMonthCalCtrl.h : header file
//
#if !defined(_CMMONTHCALCTRL_H_)
#define _CMMONTHCALCTRL_H_

/////////////////////////////////////////////////////////////////////////////
// cmMonthCalCtrl window
class cmDateCell;

class cmMonthCalCtrl : public CMonthCalCtrl
{
// Construction
public:
	cmMonthCalCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmMonthCalCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmMonthCalCtrl();

	void SetParentCell(cmDateCell* parent);

	void SetDate(const COleDateTime& dt);
	COleDateTime GetDate() const;

	// Generated message map functions
protected:
	//{{AFX_MSG(cmMonthCalCtrl)
	afx_msg void OnSelect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

	void ChangeSelection(SYSTEMTIME& t);

	cmDateCell* parentCell_;
	COleDateTime date_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CMMONTHCALCTRL_H_)
