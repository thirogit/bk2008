#include "stdafx.h"
#include "ImportExportOptDlg.h"
#include "MainCfg.h"

// CImportExportOptDlg dialog

IMPLEMENT_DYNAMIC(CImportExportOptDlg, CPropertyPage)
CImportExportOptDlg::CImportExportOptDlg()
	: CPropertyPage(CImportExportOptDlg::IDD)
{

		m_bExportPassImgs = AppCfg().GetBool(CFG_EXPORTPASSIMGS);
		m_bExportPriceAsMyPrice = AppCfg().GetBool(CFG_EXPORTSELLBUYPRICE);
		//m_bExportMyPricePerKg = AppCfg().GetBool(CFG_EXPORTMYPRICEPERKG);
		m_bExportBuySellValues = AppCfg().GetBool(CFG_EXPORTBUYSELLVALUES);
		m_bImportPassImgs = AppCfg().GetBool(CFG_IMPORTPASSIMGS);
		m_bInsertMissingStocks = AppCfg().GetBool(CFG_INSERTMISSINGSTOCKS);
		m_bInsertMissingHents = AppCfg().GetBool(CFG_INSERTMISSINGHENTS);
		m_bSaveOriginalDocDates = AppCfg().GetBool(CFG_SAVEDOCDATES);
		m_bSaveOriginalHents = AppCfg().GetBool(CFG_SAVEORIGINALHENT);
		m_bExportMyPriceAsMyPrice = AppCfg().GetBool(CFG_EXPORTMYPRICEASMYPRICE) && !m_bExportPriceAsMyPrice;

}

CImportExportOptDlg::~CImportExportOptDlg()
{
}

void CImportExportOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);


	DDX_Check(pDX,IDC_EXPORTPRICEASMYPRICE,m_bExportPriceAsMyPrice);
	DDX_Check(pDX,IDC_EXPORTMYPRICEPERKG,m_bExportMyPricePerKg);
	DDX_Check(pDX,IDC_EXPPORTBUYSELLVALUES,m_bExportBuySellValues);
	DDX_Check(pDX,IDC_EXPORTPASSIMGS,m_bExportPassImgs);
	DDX_Check(pDX,IDC_IMPORTPASSIMGS,m_bImportPassImgs);
	DDX_Check(pDX,IDC_INSERTMISSINGSTOCKS,m_bInsertMissingStocks);
	DDX_Check(pDX,IDC_INSERTMISSINGHENTS,m_bInsertMissingHents);
	DDX_Check(pDX,IDC_SAVEORIGINALDOCDATES,m_bSaveOriginalDocDates);
	DDX_Check(pDX,IDC_SAVEORIGINALHENT,m_bSaveOriginalHents);
	DDX_Check(pDX,IDC_EXPORTMYPRICEASMYPRICE,m_bExportMyPriceAsMyPrice);
}


BEGIN_MESSAGE_MAP(CImportExportOptDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_EXPORTMYPRICEASMYPRICE, UpdateControls)
	ON_BN_CLICKED(IDC_EXPORTPRICEASMYPRICE, UpdateControls)
END_MESSAGE_MAP()

BOOL CImportExportOptDlg::OnApply()
{
	 UpdateData();
	 AppCfg().SetBool(CFG_EXPORTPASSIMGS,m_bExportPassImgs);
	 AppCfg().SetBool(CFG_EXPORTSELLBUYPRICE,m_bExportPriceAsMyPrice);
	// AppCfg().SetBool(CFG_EXPORTMYPRICEPERKG,m_bExportMyPricePerKg);
	 AppCfg().SetBool(CFG_EXPORTBUYSELLVALUES,m_bExportBuySellValues);
	 AppCfg().SetBool(CFG_IMPORTPASSIMGS,m_bImportPassImgs);
	 AppCfg().SetBool(CFG_INSERTMISSINGSTOCKS,m_bInsertMissingStocks);
	 AppCfg().SetBool(CFG_INSERTMISSINGHENTS,m_bInsertMissingHents);
	 AppCfg().SetBool(CFG_SAVEDOCDATES,m_bSaveOriginalDocDates);
	 AppCfg().SetBool(CFG_SAVEORIGINALHENT,m_bSaveOriginalHents);
	 AppCfg().SetBool(CFG_EXPORTMYPRICEASMYPRICE,m_bExportMyPriceAsMyPrice);
	 return TRUE;
}

BOOL CImportExportOptDlg::OnInitDialog()
{
	if(!CPropertyPage::OnInitDialog()) return FALSE;
	
	UpdateData(FALSE);
	UpdateControls();
	
	return TRUE;
}

void CImportExportOptDlg::UpdateControls()
{
	UpdateData();

	m_bExportMyPriceAsMyPrice = !m_bExportPriceAsMyPrice && m_bExportMyPriceAsMyPrice;
	SendDlgItemMessage(IDC_EXPORTMYPRICEASMYPRICE,BM_SETCHECK,m_bExportMyPriceAsMyPrice);
	GetDlgItem(IDC_EXPORTMYPRICEASMYPRICE)->EnableWindow(!m_bExportPriceAsMyPrice);
	
}

