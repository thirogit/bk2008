// AsHexadecimalEdit.h : header file
//
#if !defined(_CM_HEXADECIMALEDIT_H_)
#define _CM_HEXADECIMALEDIT_H_

#include "cmString.h"
#include "cmGenericText.h"

/////////////////////////////////////////////////////////////////////////////
// CAsNumericEdit window

class cmHexadecimalEdit : public cmGenericText
{
// Construction
public:
	cmHexadecimalEdit();

// Attributes
public:
	void SetBitCount(long bitCount);
	long GetBitCount() const;

	void SetValue(long value);
	long GetValue();
	
	long Validate();

	cmString FormatValue(long value);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsNumericEdit)
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmHexadecimalEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmHexadecimalEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillfocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

	long ValidateString(const cmString& valueStr);
	long ValidateValue(long value);
	

	long bitCount_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CM_HEXADECIMALEDIT_H_)
