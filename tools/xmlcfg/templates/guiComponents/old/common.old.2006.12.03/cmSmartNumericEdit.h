// cmSmartNumericEdit.h : header file
//
#if !defined(_CMSMARTNUMERICEDIT_H_)
#define _CMSMARTNUMERICEDIT_H_

#include "cmString.h"

/////////////////////////////////////////////////////////////////////////////
// cmNumericEdit window

class cmSmartNumericEdit : public CEdit
{
// Construction
public:
	cmSmartNumericEdit();

// Attributes
public:
	void SetMinValue(long minValue);
	long GetMinValue() const;

	void SetMaxValue(long maxValue);
	long GetMaxValue() const;

	void SetValue(long value);
	long GetValue();
	
	void SetMinMaxCheckEnable(bool enable);
	bool GetMinMaxCheckEnable() const;

	long Validate();

	void SetHexDisplayMode(bool hexDisplay);
	bool GetHexDisplayMode() const;

	cmString FormatValue(long value, bool hexDisplayMode = false);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmSmartNumericEdit)
	public:
		virtual BOOL PreTranslateMessage( MSG* pMsg );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmSmartNumericEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmSmartNumericEdit)
	afx_msg void OnKillfocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

	long ValidateString(const cmString& valueStr);
	long ValidateValue(long value);
	BOOL ProcessChar(UINT nChar);

	cmString ConvertToHex(const cmString& txt);
	cmString ConvertToDec(const cmString& txt);
	long GetNumericValue();

	long minValue_;
	long maxValue_;
	bool minMaxCheckEnable_;
	bool hexDisplayMode_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CMSMARTNUMERICEDIT_H_)
