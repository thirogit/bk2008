// AsNumericEdit.h : header file
//
#if !defined(_ASNUMERICEDIT_H_)
#define _ASNUMERICEDIT_H_

#include "cmString.h"

/////////////////////////////////////////////////////////////////////////////
// cmNumericEdit window

class cmNumericEdit : public CEdit
{
// Construction
public:
	cmNumericEdit();

// Attributes
public:
	void SetMinValue(double minValue);
	double GetMinValue() const;

	void SetMaxValue(double maxValue);
	double GetMaxValue() const;

	void SetPrecision(int precision);
	int GetPrecision() const;

	void SetMinMaxCheckEnable(bool enable);
	bool GetMinMaxCheckEnable() const;

	void SetDecimalSeparator(char decimalSeparator);
	char GetDecimalSeparator() const;

	void SetValue(double value);
	double GetValue();
	
	double Validate();

	cmString FormatValue(double value);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmNumericEdit)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmNumericEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmNumericEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillfocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

	double ValidateString(const cmString& valueStr);
	double ValidateValue(double value);
	

	int precision_;
	double minValue_;
	double maxValue_;
	bool minMaxCheckEnable_;

	char decimalSeparator_;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_ASNUMERICEDIT_H_)
