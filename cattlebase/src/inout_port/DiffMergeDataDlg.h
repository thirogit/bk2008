#ifndef __DIFFMERGEDATADLG_H__
#define __DIFFMERGEDATADLG_H__

#include "DataDiffList.h"


class CDiffMergeDataDlg : public CDialog
{
protected:
	DECLARE_DYNAMIC(CDiffMergeDataDlg)

public:
	CDiffMergeDataDlg(DiffMemberArray& diffArray,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDiffMergeDataDlg();

// Dialog Data
	enum { IDD = IDD_DIFFMERGEDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	CDataDiffList m_diffList;
	DiffMemberArray &m_diffArray;

	afx_msg void OnSelectAll();
	DECLARE_MESSAGE_MAP()
};

#endif