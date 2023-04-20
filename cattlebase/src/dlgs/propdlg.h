#ifndef __TIMESDLG_H__
#define __TIMESDLG_H__

#include "resource.h"
#include "../DataTypes.h"

class CPropDlg : public  CDialog 
{
public:
	CPropDlg (RawData *raw);

// Dialog Data
	//{{AFX_DATA(CPropDlg)
	enum { IDD = IDD_PROP };
	//}}AFX_DATA

	CString m_creator;
	CString m_creattime;
	CString m_lmodifier;
	CString m_lmodifytime;

// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	//{{AFX_MSG(CPropDlg)

	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif