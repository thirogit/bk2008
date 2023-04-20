#pragma once


class CTestHentListBoxDlg : public CDialog
{

public:
	CTestHentListBoxDlg(CWnd* pParent = NULL);   
	virtual ~CTestHentListBoxDlg();
	enum { IDD = IDD_DIALOG3 };	
private:
  
		

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	
	DECLARE_MESSAGE_MAP()
};
