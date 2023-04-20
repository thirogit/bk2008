#ifndef __KEYSTATEDLG_H__
#define __KEYSTATEDLG_H__


class CKeyStateDlg : public CDialog
{
public:
	CKeyStateDlg(CWnd* pParent = NULL);   
	enum { IDD = IDD_KEYSTATE };
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	bool CheckKeyState();

	CString m_sKeyState;


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};


#endif