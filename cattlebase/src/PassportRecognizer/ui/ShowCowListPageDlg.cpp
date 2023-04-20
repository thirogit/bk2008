#include "stdafx.h"
#include "ShowCowListPageDlg.h"


// CShowCowListPageDlg dialog

IMPLEMENT_DYNAMIC(CShowCowListPageDlg, CPropertyPage)
CShowCowListPageDlg::CShowCowListPageDlg(CCowDataArray *cowArray,int iConfigId/* = -1*/)
	: CPropertyPage(CShowCowListPageDlg::IDD) , m_ScanList(cowArray,iConfigId)
{
}

CShowCowListPageDlg::~CShowCowListPageDlg()
{
}

void CShowCowListPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SHOWCOWLIST,m_ScanList);
}


BEGIN_MESSAGE_MAP(CShowCowListPageDlg, CPropertyPage)
END_MESSAGE_MAP()


// CShowCowListPageDlg message handlers
