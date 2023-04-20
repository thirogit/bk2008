#ifndef __ARCHIVEOPT_H__
#define __ARCHIVEOPT_H__

#include <afxeditbrowsectrl.h>

class CArchiveOpt : public CPropertyPage
{
	DECLARE_DYNAMIC(CArchiveOpt)

public:
	CArchiveOpt();
	virtual ~CArchiveOpt();

// Dialog Data
	enum { IDD = IDD_ARCHIVE };

protected:
	CMFCEditBrowseCtrl   m_archiveDir;
	CListBox        m_archivesList;
	CString         m_archiveDirStr;
	BOOL			m_doPassDBArchive;

	void FillListBox();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	BOOL OnApply();
protected:
	afx_msg void OnBnClickedDoArchive();
	afx_msg void OnBnClickedDelArchiveBtn();
	afx_msg LRESULT OnDirChange(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};
#endif