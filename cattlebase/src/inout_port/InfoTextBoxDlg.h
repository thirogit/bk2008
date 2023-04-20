#ifndef  __INFOTEXTBOX_H__
#define __INFOTEXTBOX_H__
#include "../ctrls/ConEdit/ConEdit.h"

// CInfoTextBoxDlg dialog

class CInfoTextBoxDlg : public CDialog
{
public:
	static void ShowTextInfoBox(const CString & text,CWnd *pParent = NULL);
public:
	CInfoTextBoxDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfoTextBoxDlg();
	enum { IDD = IDD_INFOTEXTBOX };
	void SetText(const CString &text);
private:
	CConEdit m_textBox;
	CString m_text;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
#endif