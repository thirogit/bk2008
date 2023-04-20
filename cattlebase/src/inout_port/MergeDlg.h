#ifndef __MERGEDLG_H__
#define __MERGEDLG_H__
// CMergeDlg dialog



class CMergeDlg : public CDialog
{
	DECLARE_DYNAMIC(CMergeDlg)

public:

	enum MERGE_ACTION
	{
		MERGE_ACTION_INSERT = 303,
		MERGE_ACTION_REPLACE = 404,
		MERGE_ACTION_INSERT_4ALL = 505,
		MERGE_ACTION_REPLACE_4ALL = 606
	};

	CMergeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMergeDlg();

	void SetMergeText(const CString &mergeText);
	MERGE_ACTION GetMergeAction();
	
// Dialog Data
	enum { IDD = IDD_MERGEDLG };

private:
	CString m_mergeText;
	BOOL m_bDo4All;
	int radioAction;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	afx_msg void OnReplaceOptRadioBtn();
	DECLARE_MESSAGE_MAP()
};

#endif