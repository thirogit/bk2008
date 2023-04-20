#include "stdafx.h"
#include "PassportSegregationDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(CPassportSegregationDlg, CDialog)
CPassportSegregationDlg::CPassportSegregationDlg(CDocDataArray& docs,CWnd* pParent /*=NULL*/)
	: CDialog(CPassportSegregationDlg::IDD, pParent)
{	

	

}

CPassportSegregationDlg::~CPassportSegregationDlg()
{
	
}

void CPassportSegregationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CPassportSegregationDlg, CDialog)
//ON_BN_CLICKED(IDC_UPDATEREPO, OnUpdateRepo)
	
		
END_MESSAGE_MAP()


BOOL CPassportSegregationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	
	return TRUE;
}

