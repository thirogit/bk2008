// CowStockStatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cattlebase.h"
#include "CowStockStatDlg.h"


// CCowStockStatDlg dialog

IMPLEMENT_DYNAMIC(CCowStockStatDlg, CDialog)

CCowStockStatDlg::CCowStockStatDlg(CCowArray &arrayCows,CWnd* pParent /*=NULL*/)
	: CDialog(CCowStockStatDlg::IDD, pParent),m_cowStockStat(arrayCows)
{

}

CCowStockStatDlg::~CCowStockStatDlg()
{
}

void CCowStockStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COWSTOCKSTATLIST,m_cowStockStat);
}


BEGIN_MESSAGE_MAP(CCowStockStatDlg, CDialog)
END_MESSAGE_MAP()


BOOL CCowStockStatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cowStockStat.UpdateStatistic();

	return TRUE;
}