#include "stdafx.h"
#include "propdlg.h"


BEGIN_MESSAGE_MAP(CPropDlg, CDialog)
	//{{AFX_MSG_MAP(CPropDlg)
	ON_BN_CLICKED(IDOK,OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPropDlg::CPropDlg(RawData *raw) : CDialog(CPropDlg::IDD),
m_creator(raw->creation.user->username),
m_lmodifier(raw->modification.user->username),
m_creattime(raw->creation.eventDt.GetDayDate()),
m_lmodifytime(raw->modification.eventDt.GetDayDate())
{
}

void CPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CPropDlg)
	DDX_Text(pDX, IDC_CREATOR  , m_creator ) ;
	DDX_Text (pDX, IDC_CREATTIME  , m_creattime ) ;
	DDX_Text (pDX, IDC_LMODIFIER  , m_lmodifier ) ;
	DDX_Text (pDX, IDC_LMODIFYTIME  , m_lmodifytime ) ;
				
	//}}AFX_DATA_MAP
}

