#ifndef __WELCOME_H__
#define __WELCOME_H__

#include "bitmapdialog\BDialog.h"
#include "resource.h"
#include "../ctrls/TransparentStatic.h"
#include "../DataTypes.h"

#define TIMER1 1
#define CLOSEDELAY 1000

class CWelcomeDlg : public  CBDialog 
{
public:
	CWelcomeDlg ();

// Dialog Data
	//{{AFX_DATA(CWelcomeDlg)
	enum { IDD = IDD_WELCOME };
	//}}AFX_DATA
	
protected:

	CString m_user;
	CString m_pass;

	CTransparentStatic m_Label_user;
	CTransparentStatic m_Label_pass;
	CTransparentStatic m_Label_status;

	CBitmap m_facelogo;
	CBitmap m_companyLogo;

	CString m_apptitle;
	CString m_verString;
	CFont m_titleFont;
	CFont m_buildFont;
	
	bool bUsersLoaded;
    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	bool Login(UserData *pUser,CPassword &password);
	bool LoadUsers();
	void EnableControls(BOOL bEnable);
	void PutBmp(CDC* pDC,int x,int y,CBitmap &bmp);
	void OnOK();	
// Implementation
protected:
	//{{AFX_MSG(CWelcomeDlg)
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedLogcancel();
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};


#endif