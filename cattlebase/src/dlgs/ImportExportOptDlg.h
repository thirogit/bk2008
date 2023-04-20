#ifndef __IMPORTEXPORTOPTDLG_H__
#define __IMPORTEXPORTOPTDLG_H__

// CImportExportOptDlg dialog

class CImportExportOptDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CImportExportOptDlg)

public:
	CImportExportOptDlg();   // standard constructor
	virtual ~CImportExportOptDlg();

// Dialog Data
	enum { IDD = IDD_IMPORTEXPORTOPT };
protected:
	BOOL m_bExportPriceAsMyPrice;
	BOOL m_bExportMyPricePerKg;
	BOOL m_bExportBuySellValues;
	BOOL m_bExportPassImgs;
	BOOL m_bImportPassImgs;
	BOOL m_bInsertMissingStocks;
	BOOL m_bInsertMissingHents;
	BOOL m_bSaveOriginalDocDates;
	BOOL m_bSaveOriginalHents;
	BOOL m_bExportMyPriceAsMyPrice;

	virtual BOOL OnApply();
	BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void UpdateControls();
	DECLARE_MESSAGE_MAP()
};

#endif