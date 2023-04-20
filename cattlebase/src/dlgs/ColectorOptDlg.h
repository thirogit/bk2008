#pragma once


// CColectorOptDlg dialog

class CColectorOptDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CColectorOptDlg)

public:
	CColectorOptDlg();
	virtual ~CColectorOptDlg();

	CComboBox m_speed;
	CComboBox m_port;
	CComboBox m_databits;
	CComboBox m_parity;
	CComboBox m_stopbits;
	CComboBox m_flow;

// Dialog Data
	enum { IDD = IDD_COLECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	void FillComboInt(CComboBox &combo,int *ary,int nsize);
	void FillComboStr(CComboBox &combo,char *ary[],int nsize);

	DECLARE_MESSAGE_MAP()
};
