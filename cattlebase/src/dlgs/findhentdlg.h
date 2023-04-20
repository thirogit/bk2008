#ifndef __FINDHENTDLG_H__
#define __FINDHENTDLG_H__

#include "resource.h"
#include "..\FilterEdit\FilterEdit.h"
#include "CattleSQL.h"
#include "findbasedlg.h"
#include "..\combo\HentTypeCombo.h"
#include "../ctrls/WildFilterEdit.h"
#include "../ctrls/curdateedit/curdateedit.h"



class CFindHentDlg : public  CFindBaseDlg
{
public:
	CFindHentDlg ();

// Dialog Data
	//{{AFX_DATA(CFindHentDlg)
	enum { IDD = IDD_FINDHENT };
	//}}AFX_DATA

	CString m_name;
	CString m_alias;
	CWildFilterEdit m_arimrno;
	CWildFilterEdit m_wetno;
	CWildFilterEdit m_nip;
	CString m_street;
	CString m_pobox;
	CString m_city;
	CString m_zip;
	CString m_phone;
	CComboBox m_country;
	CString m_plate;

	CWildFilterEdit m_pesel;
	CWildFilterEdit m_regon;
	CWildFilterEdit m_idno;
	CCurDateEdit m_issuedate;
	CString m_issuepost;
	CWildFilterEdit m_accountno;
	CString m_bankname;


    CFindHentTypeCombo m_htype;

	//CRegEx m_EANRegEx;	
	//CRegEx m_WetNoRegEx;
	//CRegEx m_NIPRegEx;
	//CRegEx m_ZipRegEx;

	int GoSearch(CFindSetup& setup);


// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindHentDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CFindHentDlg)

//	afx_msg void OnSelChangeFindTab(NMHDR* pNMHDR, LRESULT* pResult) ;
	
		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif