#include "stdafx.h"
#include "FastFindHent.h"
#include "../classes/TempStringLoader.h"

IMPLEMENT_DYNAMIC(CFastFindHent, CDialog)

BEGIN_MESSAGE_MAP(CFastFindHent, CDialog)
END_MESSAGE_MAP()

CFastFindHent::CFastFindHent(CWnd* pParent /*=NULL*/): CDialog(CFastFindHent::IDD, pParent)
{
}

CFastFindHent::~CFastFindHent()
{
}

void CFastFindHent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDITBOX,m_hentNoEdit);
}


BOOL CFastFindHent::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(CTempStringLoader(IDS_FASTFINDHENTCAPTION));
	SetDlgItemText(IDC_EDITBOXDLGLBL,CTempStringLoader(IDS_FASTFINDHENTLBL));

	return TRUE;
}

void CFastFindHent::OnOK()
{
	UpdateData();
	if(m_hentNoEdit.IsInputEmpty() || !m_hentNoEdit.IsFullData()) 
	{
		AfxMessageBox(IDS_NOHENTNO,MB_OK,0);
	}
	else
	{
		m_hentNoEdit.GetWindowText(m_hentNo);
		EndDialog(IDOK);
	}

}

BOOL  CFastFindHent::GetHentNo(CString &hentNo)
{

	if(DoModal() == IDOK)
	{
		hentNo = m_hentNo;
		return TRUE;
	}
	return FALSE;
}



