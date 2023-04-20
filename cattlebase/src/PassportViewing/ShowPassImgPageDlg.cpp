#include "stdafx.h"
#include "ShowPassImgPageDlg.h"


// CShowPassImgPageDlg dialog

IMPLEMENT_DYNAMIC(CShowPassImgPageDlg, CPropertyPage)
CShowPassImgPageDlg::CShowPassImgPageDlg()
	: CPropertyPage(CShowPassImgPageDlg::IDD)
{
}

CShowPassImgPageDlg::~CShowPassImgPageDlg()
{
}

bool CShowPassImgPageDlg::UpdateWithImage(fipWinImageEx &pix)
{
	return m_passShow.UpdateImage(pix);
}

void CShowPassImgPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SHOWPASSIMG,m_passShow);
}


BEGIN_MESSAGE_MAP(CShowPassImgPageDlg, CPropertyPage)
END_MESSAGE_MAP()


// CShowPassImgPageDlg message handlers
