// StyleEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DRAWCLI.h"
#include "StyleEdit.h"


// CStyleEdit dialog

IMPLEMENT_DYNAMIC(CStyleEdit, CDialog)
CStyleEdit::CStyleEdit(CTextStyle *pStyle,CWnd* pParent /*=NULL*/) : CDialog(CStyleEdit::IDD, pParent)
{
	m_pStyle = pStyle;
	VERIFY(pStyle);

	
}

CStyleEdit::~CStyleEdit()
{

}

void CStyleEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_HORCOMBO,m_HAlignCombo);
	DDX_Control(pDX,IDC_VERCOMBO,m_VAlignCombo);
	DDX_Text(pDX,IDC_STYLENAME,m_stylename);
	DDX_Control(pDX,IDC_STYLEFONT,m_fonts);
	DDX_Control(pDX, IDC_STYLECOLOR, m_BtnTextColor);

	DDX_Control(pDX, IDC_HSPIN, m_HeightSpin);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDV_MinMaxUInt(pDX, m_Height, MIN_FONT_HEIGHT, MAX_FONT_HEIGHT);
	DDX_Control(pDX,IDC_ORIENTATION,m_orientation);
}


BEGIN_MESSAGE_MAP(CStyleEdit, CDialog)
	ON_COMMAND(IDC_BOLD,OnBold)
	ON_COMMAND(IDC_ITALIC,OnItalic)
	ON_COMMAND(IDC_UNDERLINE,OnUnderline)
	ON_COMMAND(IDC_STRIKEOUT,OnStrikeout)
	ON_WM_DESTROY()
	
END_MESSAGE_MAP()


void CStyleEdit::OnDestroy()
{
	delete m_styleBar;
}
BOOL CStyleEdit::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;

	m_HAlignCombo.InitHAlignCombo();
	m_VAlignCombo.InitVAlignCombo();
	m_fonts.InitFonts();

	CRect rc;
    m_orientation.GetWindowRect( &rc );
	ScreenToClient( &rc);
	int y = rc.top;


	m_styleBar = new CTfontTbar();

	m_styleBar->Create(this);
	m_styleBar->SetWindowPos(&wndTop,5,y, 0, 0, SWP_NOSIZE);
	m_styleBar->ShowWindow(SW_SHOWNOACTIVATE);

	m_BtnTextColor.m_bEnableBtnColorDefault = false; 
		
	SetStyle(m_pStyle);


	return TRUE;
}
void CStyleEdit::OnBold()
{
	m_bBold = !m_bBold;
}
void CStyleEdit::OnItalic()
{
	m_bItalic = !m_bItalic;
}
void CStyleEdit::OnStrikeout()
{
	m_bStrikeout = !m_bStrikeout;
}
void CStyleEdit::OnUnderline()
{
	m_bUnderline = !m_bUnderline;
}

void CStyleEdit::SetStyle(CTextStyle *pStyle)
{
	CWnd *WndStyleName = GetDlgItem(IDC_STYLENAME);
	WndStyleName->SetWindowText(pStyle->m_stylename);
	if(pStyle->m_styleid == USER_DEF_STYLE_ID)
		WndStyleName->EnableWindow(FALSE);

	m_BtnTextColor.m_clrDefault = m_BtnTextColor.m_clrSelected = pStyle->m_txtclr;
	m_BtnTextColor.RedrawWindow();
	
	SetDlgItemInt(IDC_HEIGHT,-pStyle->m_lf.lfHeight,0);
	m_fonts.SelectString(-1,pStyle->m_lf.lfFaceName);

	m_bBold = pStyle->m_lf.lfWeight != FW_NORMAL;
	m_bItalic = pStyle->m_lf.lfItalic;
	m_bUnderline = pStyle->m_lf.lfUnderline;
	m_bStrikeout = pStyle->m_lf.lfStrikeOut;

	m_styleBar->SetBarButtons(m_bBold,m_bItalic,m_bUnderline,m_bStrikeout);

	m_HAlignCombo.SelectAlign(pStyle->m_halign);
	m_VAlignCombo.SelectAlign(pStyle->m_valign);

	m_orientation.SetOrientationAngle(pStyle->m_TextAngle);

	if(pStyle->m_styleid == USER_DEF_STYLE_ID)
			GetDlgItem(IDC_STYLENAME)->EnableWindow(FALSE);
	
}

void CStyleEdit::OnOK()
{
		UpdateData();
		
		m_fonts.GetSelFont(m_pStyle->m_lf);
        m_pStyle->m_txtclr = m_BtnTextColor.m_clrSelected;
		m_pStyle->m_lf.lfHeight = -(int)m_Height;	
		m_pStyle->m_halign = m_HAlignCombo.GetSelHAlign();
		m_pStyle->m_valign = m_VAlignCombo.GetSelVAlign();
		m_pStyle->m_stylename = m_stylename;

		m_pStyle->m_lf.lfWeight = m_bBold ? FW_BOLD : FW_NORMAL;
		m_pStyle->m_lf.lfItalic = m_bItalic;
		m_pStyle->m_lf.lfUnderline = m_bUnderline;
		m_pStyle->m_lf.lfStrikeOut = m_bStrikeout;
		m_pStyle->m_lf.lfCharSet = DEFAULT_CHARSET;
		m_pStyle->m_TextAngle = m_orientation.GetOrientationAngle();
		m_pStyle->RealizeStyle(true);
        
		EndDialog(IDOK);
}
