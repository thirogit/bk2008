#ifndef __ISSUEVALUEDLG_H__
#define __ISSUEVALUEDLG_H__

#include "../FilterEdit/FilterEdit.h"

// CIssueValueDlg dialog

class CIssueValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CIssueValueDlg)

public:
	CIssueValueDlg(CWnd* pParent = NULL); 
	enum { IDD = IDD_ISSUEDLG };

	void SetCaption(UINT nCaption);
	void SetCaption(LPCSTR lpszCaption);
	void SetIssueText(UINT nIssueText);
	void SetIssueText(LPCSTR lpszIssueText);
	void SetIssueValueName(UINT nValueName);
	void SetIssueValueName(LPCSTR lpszValueName);
	void SetRegEx(LPCSTR lpszRegEx);
	CString GetValue();
	void SetValue(LPCSTR lpszValue);

	virtual ~CIssueValueDlg();

protected:
	CFilterEdit m_valueEdit;
	CString m_caption;
	CString m_IssueText;
	CString m_ValueName;
	CString m_Value;
	CString m_ValueRegEx;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	void OnOK();

	DECLARE_MESSAGE_MAP()
};

#endif