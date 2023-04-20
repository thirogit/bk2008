#pragma once

#include "SyncHelper.h"
#include "SyncCowRejectedDlg.h"
#include "SyncDocRejectedDlg.h"

// CRejectedBaseDlg dialog



class CRejectedBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CRejectedBaseDlg)

public:
	CRejectedBaseDlg(CSyncDocDataArray &rejUpdInDocs,
					CSyncDocDataArray &rejNewInDocs,
					CSyncDocDataArray &rejUpdOutDocs,
					CSyncDocDataArray &rejNewOutDocs,
					CSyncDocDataArray &newInDocs,
					CSyncDocDataArray &newOutDocs,
					CSyncDocDataArray &updInDocs,
					CSyncCowDataArray &newCowArray,
					CSyncCowDataArray &updCowArray,
					CSyncCowDataArray &rejNewCowArray,
					CSyncCowDataArray &rejUpdCowArray,
					CWnd* pParent = NULL);   // standard constructor
	virtual ~CRejectedBaseDlg();

// Dialog Data
	enum { IDD = IDD_REJECTEDBASE };
protected:

	CPropertySheet m_rejSheet;


	CSyncDocDataArray &m_rejUpdInDocs;
	CSyncDocDataArray &m_rejNewInDocs;
	CSyncDocDataArray &m_rejUpdOutDocs;
	CSyncDocDataArray &m_rejNewOutDocs;
	CSyncDocDataArray &m_newInDocs;
	CSyncDocDataArray &m_newOutDocs;
	CSyncDocDataArray &m_updInDocs;
	CSyncCowDataArray &m_newCowArray;
	CSyncCowDataArray &m_updCowArray;
	CSyncCowDataArray &m_rejNewCowArray;
	CSyncCowDataArray &m_rejUpdCowArray;
		
	CSyncRejectedUpdDocsDlg m_rejuInDocsDlg;
	CSyncRejectedNewDocsDlg m_rejnInDocsDlg;
	CSyncRejectedNewDocsDlg m_rejnOutDocsDlg;
	CSyncCowRejectedDlg m_rejnCowsDlg;
	CSyncCowRejectedDlg m_rejuCowsDlg;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void UpdatePropertyPageTitles();
	void RollbackChangesToRejectedUpdatedCows();
	virtual void OnCancel();
	virtual void OnOK();

	afx_msg void OnBnClickedRefresh();

	afx_msg LRESULT OnSetRejectText(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
};
