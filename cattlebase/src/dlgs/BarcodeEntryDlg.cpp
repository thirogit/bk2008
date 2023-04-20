// BarcodeEntryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cattlebase.h"
#include "BarcodeEntryDlg.h"
#include "../classes/TempStringLoader.h"

#define DLG_FONT_SIZE 20
#define DLG_FONT_SIZE_STATUS 15
#define DLG_DELAY_BEFORE_CLOSE_MS 500
#define DLG_BC_BG_COLOR RGB(255,255,255)

// CBarcodeEntryDlg dialog

IMPLEMENT_DYNAMIC(CBarcodeEntryDlg, CDialog)

CBarcodeEntryDlg::CBarcodeEntryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBarcodeEntryDlg::IDD, pParent),m_pValidator(NULL)
{

}

CBarcodeEntryDlg::~CBarcodeEntryDlg()
{
}

void CBarcodeEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ENTRYTEXT,m_entryText);
	DDX_Control(pDX,IDC_BARCODE,m_barcodeValue);
	DDX_Control(pDX,IDC_STATUS,m_barcodeStatus);
}


BEGIN_MESSAGE_MAP(CBarcodeEntryDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OVERRIDE,OnOverride)
END_MESSAGE_MAP()


BOOL CBarcodeEntryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CBarcodeScannerDriver::AddReceiver(this);

	m_entryText.SetFont((LPCSTR)NULL,DLG_FONT_SIZE);
	m_barcodeValue.SetBackgroundColor(DLG_BC_BG_COLOR);
	m_barcodeValue.SetFont((LPCSTR)NULL,DLG_FONT_SIZE);
	m_barcodeValue.SetWindowText("______________");

	m_entryText.SetWindowText(m_entryTextStr);
	m_barcodeStatus.SetFont(NULL,DLG_FONT_SIZE_STATUS,FALSE);
	m_barcodeStatus.SetWindowText("");

	GetDlgItem(IDC_OVERRIDE)->EnableWindow(FALSE);
	return TRUE;
}

void CBarcodeEntryDlg::OnDestroy()
{	
	CBarcodeScannerDriver::RemoveReceiver(this);
	CDialog::OnDestroy();
}

void CBarcodeEntryDlg::SetEntryText(const CString& entryText)
{
	m_entryTextStr = entryText;
	if(m_hWnd)
		m_entryText.SetWindowText(m_entryTextStr);
}
void CBarcodeEntryDlg::OnBarcode(const CString& sBarcode)
{
	m_barcodeValue.SetWindowText(sBarcode);
	m_barcodeValue.Invalidate(TRUE);
	m_barcodeValueStr = sBarcode;

	if(m_pValidator && !m_pValidator->IsValid(sBarcode))
	{
		m_barcodeStatus.SetTextColor(RGB(255,0,0),FALSE);
		m_barcodeStatus.SetWindowText(m_pValidator->TellMeWhy(sBarcode));
		m_barcodeStatus.Invalidate(TRUE);

		CWnd *pOverrideBtn = GetDlgItem(IDC_OVERRIDE);
		pOverrideBtn->EnableWindow(m_pValidator->IsSoftViolation(sBarcode));
		pOverrideBtn->SetFocus();		
	}
	else
	{
		m_barcodeStatus.SetTextColor(RGB(0,0,0),FALSE);
		m_barcodeStatus.SetWindowText(CTempStringLoader(IDS_GOODBC));
		m_barcodeStatus.Invalidate(TRUE);		
		Sleep(DLG_DELAY_BEFORE_CLOSE_MS);
		EndDialog(IDOK);
	}

	
}

const CString& CBarcodeEntryDlg::GetBarcode()
{
	return m_barcodeValueStr;
}
void CBarcodeEntryDlg::SetValidator(CValidator<const CString&> *pValidator)
{
	m_pValidator = pValidator;
}

void CBarcodeEntryDlg::OnOverride()
{
	EndDialog(IDOK);
}