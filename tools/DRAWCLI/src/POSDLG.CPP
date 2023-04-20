
#include "stdafx.h"
#include "drawcli.h"
#include "posdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define __LOCALMSG_InternalUpdateControls (WM_USER+333)

/////////////////////////////////////////////////////////////////////////////
// CPosDlg dialog

IMPLEMENT_DYNCREATE( CPosDlg, CBaseDlg )

CPosDlg::CPosDlg(CWnd* /*pParent = NULL*/)	: CBaseDlg(CPosDlg::IDD)
{
		
}


CPosDlg::CPosDlg(RECT* rc)
	: CBaseDlg(CPosDlg::IDD)
{
	SetPos(rc);	
}

void CPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosDlg)

	DDX_Control(pDX, IDC_LEFTSPIN, m_LeftSpin);
	DDX_Control(pDX, IDC_TOPSPIN, m_TopSpin);
	DDX_Control(pDX, IDC_RIGHTSPIN, m_RightSpin);
	DDX_Control(pDX, IDC_BOTTOMSPIN, m_BottomSpin);
	
	DDX_Text(pDX, IDC_LEFTEDIT, m_nLeft);
	DDX_Text(pDX, IDC_TOPEDIT, m_nTop);
	DDX_Text(pDX, IDC_RIGHTEDIT, m_nRight);
	DDX_Text(pDX, IDC_BOTTOMEDIT, m_nBottom);


	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPosDlg, CDialog)
	//{{AFX_MSG_MAP(CPosDlg)


	//}}AFX_MSG_MAP
		
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPosDlg message handlers

#define MAXX (3000)
#define MAXY MAXX
#define MINX (-3000)
#define MINY MINX

BOOL CPosDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

m_LeftSpin.SetBuddy(GetDlgItem(IDC_LEFTEDIT));
m_LeftSpin.SetRange32(MINX,MAXX);

m_TopSpin.SetBuddy(GetDlgItem(IDC_TOPEDIT));
m_TopSpin.SetRange32(MINY,MAXY);

m_RightSpin.SetBuddy(GetDlgItem(IDC_RIGHTEDIT));
m_RightSpin.SetRange32(MINX,MAXX);

m_BottomSpin.SetBuddy(GetDlgItem(IDC_BOTTOMEDIT));
m_BottomSpin.SetRange32(MINY,MAXY);



	return TRUE;
}

void CPosDlg::SetPos(RECT *rect)
{

m_nLeft = rect->left;
m_nTop = rect->top;
m_nRight = rect->right;
m_nBottom = rect->bottom;

}


void CPosDlg::GetPos(RECT *rect)
{

rect->left = m_nLeft;
rect->top = m_nTop;
rect->right = m_nRight;
rect->bottom = m_nBottom;

}

void CPosDlg::DoChanges(CDrawObj *pObj)
{
	GetPos(pObj->m_position);
}
