#ifndef __FINDDLG_H__
#define __FINDDLG_H__

#include "stdafx.h"
#include "findcowdlg.h"
#include "findhentdlg.h"
#include "../Doc/InDoc/FindInDocDlg.h"
#include "findoutdocdlg.h"
#include "findbasedlg.h"
#include "findinvdlg.h"
#include "findexpensedlg.h"

#include "imgbutton\ImageButton.h"
#include "global_arrays.h"

#include "ctrls\DateSpan.h"
#include "combo\UserCombo.h"

#define ITEM_NEUTRAL (0)


class CFindDlg : public  CDialog,public CFindSetup
{
public:
	CFindDlg ();
	enum { IDD = IDD_SEARCH };

	~CFindDlg();

	virtual bool GetAddToResult();
	virtual bool GetNewSearch();
	virtual bool GetInResult();

	virtual bool ByCreationTime();
	virtual TIMESPAN CreationTimeSpan(CDateTime &time1,CDateTime &time2);
	virtual bool ByLastModificationTime();
	virtual TIMESPAN LastModificationTimeSpan(CDateTime &time1,CDateTime &time2);

	virtual UserData* SelectedCreator();
	virtual UserData* SelectedLastModifier();

	virtual bool ByExtras();
	virtual CString GetExtras();

	virtual void ChangeMainGrid(UINT gridId);


protected:
	CPropertySheet m_dlgPropSheet;

	CFindCowDlg  m_findcow;
	CFindHentDlg m_findhent;
	CFindInDocDlg  m_findindoc;
	CFindOutDocDlg  m_findoutdoc;
	CFindInvDlg  m_findinv;
	CFindExpenseDlg m_findexpense;

	BOOL      m_bByCreatTime;
	BOOL	  m_bByModifTime;
	CDateSpan m_creattimespan;
	CDateSpan m_modiftimespan;
	CFindUserCombo m_creator;
	CFindUserCombo m_modifier;

	CImageButton m_gosearch;
	CImageButton m_cancel;

	CString		 m_extras;

	BOOL m_bSortFound;
	BOOL m_bChangeGridTab;

	BOOL m_bNewSearch;
	BOOL m_bInResult;
	BOOL m_bAddToFound;
	
		
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	void OnOK();
	//void UpdateVisibleWindow();
	

// Implementation
protected:
	//{{AFX_MSG(CFindDlg)

	
	afx_msg void OnGoSearch(void);
	afx_msg void OnSetNewSearch(void);
	afx_msg void OnSetAddToFound(void);
	afx_msg void OnSetInResult(void);
	

	afx_msg void OnCheckClicked(void);
	afx_msg void OnDestroy();
	
		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif