// InputBox.h : header file
//
#if !defined(_INPUTBOX_H_)
#define _INPUTBOX_H_

#include "resource.h"
#include "cmString.h"

/////////////////////////////////////////////////////////////////////////////
// CInputBox dialog

class CInputBox : public CDialog
{
// Construction
public:
	CInputBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputBox();

	void Init(const cmString& title, const cmString& caption, const cmString& initialValue);
	void Init(const cmString& title, const cmString& caption, double initialValue, int precision);
	
	cmString GetValueString() const;
	double GetValueDouble() const;

// Dialog Data
	//{{AFX_DATA(CInputBox)
	enum { IDD = IDD_INPUT_BOX };
	CStatic	lblCaption_;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBox)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	cmString title_;
	cmString caption_;
	cmString valueString_;
	double valueDouble_;	
	int precision_;
	CEdit* txtValue_;
	bool stringMode_;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBOX_H__FDAB2F83_4EA6_4355_B48E_7121B8D9E8EE__INCLUDED_)
