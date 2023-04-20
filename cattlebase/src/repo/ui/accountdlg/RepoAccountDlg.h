#ifndef __REPOACCOUNTDLG_H__
#define __REPOACCOUNTDLG_H__

#include "../../RepoAccount.h"

class CRepoAccountDlg : public CDialog
{
	DECLARE_DYNAMIC(CRepoAccountDlg)

public:
	CRepoAccountDlg(RepoAccount* pAccount,CWnd* pParent = NULL);   
	virtual ~CRepoAccountDlg();
	void SetDlgCaption(const CString& sCaption);
		
	enum { IDD = IDD_REPO_ACCOUNTDLG };

private:
	CString m_sAccountName;
	CString m_sRepoAddress;
	CString m_sRepoUserName;
	CString m_sRepoPassword;

	CString m_sDlgCaption;
	RepoAccount* m_pAccount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	
};

#endif 