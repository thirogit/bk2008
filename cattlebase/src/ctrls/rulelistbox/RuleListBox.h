#if !defined(RULELISTBOX_H__INCLUDED_)
#define RULELISTBOX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExListBox window
#include "ExListBox.h"
#include "../../datatypes/invoice/InvoiceContext.h"

class CRuleListBox : public CExListBox
{
// Construction
public:
	CRuleListBox(const InvoiceContext* pContext);

// Attributes
public:
	
	BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	// Operations
// Implementation
public:
	virtual ~CRuleListBox();
	void Reload();
protected:
	virtual void PreSubclassWindow();
	CString strTipText;
	const InvoiceContext* m_pInvoiceContext;
	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
