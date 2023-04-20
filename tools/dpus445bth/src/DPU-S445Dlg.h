#pragma once

#include "DPU-S445Driver.h"

class CDPUS445Dlg : public CDialogEx
{
public:
	CDPUS445Dlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_DPUS445_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	HICON m_hIcon;

	CComboBox m_printerCombo;
	BOOL m_bCancelPIN;

	DPUS445Driver m_Driver;
	DWORD m_dwPrinterSessionId;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void InitPrintersCombo();
	bool IsSIIPrinter(LPCSTR lpszDriverName) const;
	void EnableControls(bool bEnable);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetPINBtn();
	afx_msg void OnBnClickedSetBtnNameBtn();
	afx_msg void OnBnClickedCancelPINCheck();
	afx_msg void OnBnClickedActionPrinterBtn();
};
