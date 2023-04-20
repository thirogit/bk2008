

#include "stdafx.h"
#include "drawcli.h"
#include "textdlg.h"
#include "drawdoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define __LOCALMSG_InternalUpdateControls (WM_USER+333)

/////////////////////////////////////////////////////////////////////////////
// CTextDlg dialog
IMPLEMENT_DYNCREATE( CTextDlg, CBaseDlg );

CTextDlg::CTextDlg() : CBaseDlg(CTextDlg::IDD), m_stylecombo(CStyleList())
{
	
	throw; //cant use this constructor
	/*m_text = "";
	CTextStyle style; //default
	m_bOwnStyle = TRUE;
	DoStyle(style);		
	m_pcTfontTbar = new CTfontTbar();*/
}

CTextDlg::~CTextDlg()
{
 delete m_pcTfontTbar;
// delete m_pAlignBar;
}

CTextDlg::CTextDlg(CDrawText *pTxtObj) :  CBaseDlg(CTextDlg::IDD), m_stylecombo(*pTxtObj->m_pDocument->GetStyleList())
{
	m_bNotOwnStyle = !pTxtObj->m_bOwnStyle;
	m_text = pTxtObj->m_text;
	m_pStyle = NULL;
	m_pcTfontTbar = new CTfontTbar();

	DoStyle(pTxtObj->m_OwnStyle);
	m_pStyle = pTxtObj->m_pStyle;
	
}

/*
CTextDlg::CTextDlg(CString &text,CTextStyle &style,CStyleList &StyleList) : CBaseDlg(CTextDlg::IDD), m_stylecombo(StyleList)
{
	m_bNotOwnStyle = FALSE;
	m_text = text;
	DoStyle(style);	
	m_pcTfontTbar = new CTfontTbar();
}*/

void CTextDlg::DoStyle(CTextStyle &style)
{
	m_clrText = style.m_txtclr;
	LOGFONT *logfont = &style.m_lf;
	ChooseFont(logfont);
	m_bBold = (logfont->lfWeight == FW_BOLD);
	m_bItalic = (logfont->lfItalic);
	m_bStrikeout = (logfont->lfStrikeOut);
	m_bUnderline = (logfont->lfUnderline);	

	m_txtAngle = style.m_TextAngle;
	

	m_valign = style.m_valign;
	m_halign =  style.m_halign;
	
}

void CTextDlg::DoChanges(CDrawObj *pObj)
{
	CDrawText *txtObj = (CDrawText*)pObj;
	txtObj->m_text = m_text;	

	txtObj->m_pStyle = m_pStyle;
	txtObj->m_bOwnStyle = !m_bNotOwnStyle;

	GetSelectedFont(&txtObj->m_OwnStyle.m_lf);
	txtObj->m_OwnStyle.m_TextAngle = m_txtAngle;
	txtObj->m_OwnStyle.RealizeStyle(true);

	txtObj->m_OwnStyle.m_txtclr = m_clrText;

	txtObj->m_OwnStyle.m_halign = m_halign;
    txtObj->m_OwnStyle.m_valign = m_valign;	

	

}

void CTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextDlg)
	DDX_Text(pDX, IDC_TEXT, m_text);
	DDX_Control(pDX, IDC_TEXTCOLOR, m_BtnTextColor);
	DDX_Control(pDX,IDC_FONT,m_fontCombo);
	DDX_Control(pDX, IDC_HSPIN, m_HeightSpin);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDV_MinMaxUInt(pDX, m_Height, MIN_FONT_HEIGHT, MAX_FONT_HEIGHT);

	DDX_Control(pDX,IDC_VALIGN,m_valigncombo);
	DDX_Control(pDX,IDC_HALIGN,m_haligncombo);

	DDX_Check(pDX,IDC_NOTOWNSTYLE,m_bNotOwnStyle);
	DDX_Control(pDX,IDC_STYLECOMBO,m_stylecombo);
	DDX_Control(pDX,IDC_ORIENTATION,m_orientation);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextDlg, CDialog)
	//{{AFX_MSG_MAP(CTextDlg)
	//}}AFX_MSG_MAP
		
	ON_EN_CHANGE(IDC_HEIGHT, _UpdateControls)
	ON_CBN_SELCHANGE(IDC_FONT, _UpdateControls)	

	ON_COMMAND(IDC_BOLD,OnBold)
	ON_COMMAND(IDC_ITALIC,OnItalic)
	ON_COMMAND(IDC_UNDERLINE,OnUnderline)
	ON_COMMAND(IDC_STRIKEOUT,OnStrikeout)
	ON_BN_CLICKED(IDC_NOTOWNSTYLE,_UpdateControls)


	ON_MESSAGE(
		__LOCALMSG_InternalUpdateControls,
		OnInternalUpdateControls
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextDlg message handlers


BOOL CTextDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;

		CRect rc;
		m_fontCombo.GetWindowRect( &rc );
		ScreenToClient( &rc);

		int y = rc.bottom+5;
	
		m_pcTfontTbar->Create( this );   // can throw excpetion
		m_pcTfontTbar->SetWindowPos(&wndTop,0,y, 0, 0, SWP_NOSIZE);
	    m_pcTfontTbar->ShowWindow(SW_SHOWNOACTIVATE);

		m_pcTfontTbar->SetBarButtons(m_bBold,m_bItalic,m_bUnderline,m_bStrikeout);
   
		m_valigncombo.InitVAlignCombo(m_valign);
		m_haligncombo.InitHAlignCombo(m_halign);

		m_BtnTextColor.m_clrDefault = m_BtnTextColor.m_clrSelected = m_clrText;

		m_orientation.SetOrientationAngle(m_txtAngle);


		m_fontCombo.InitFonts();	
		m_fontCombo.SelectString(-1,m_LogFont.lfFaceName);
		m_stylecombo.InitStyleCombo(m_pStyle);

	PostMessage( __LOCALMSG_InternalUpdateControls );

	return TRUE;
}

LRESULT CTextDlg::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	m_clrText = m_BtnTextColor.m_clrSelected;
	PostMessage( __LOCALMSG_InternalUpdateControls );
	return 0;
}

LRESULT CTextDlg::OnInternalUpdateControls(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	UpdateData();

	m_LogFont.lfHeight = -(int)m_Height;
	
	int sel = m_fontCombo.GetCurSel();
	LOGFONT *lfPtr;
	if(sel >= 0)
	{
		lfPtr = (LOGFONT*)m_fontCombo.GetItemData(sel);

		m_LogFont.lfClipPrecision = lfPtr->lfClipPrecision;
		m_LogFont.lfEscapement =  lfPtr->lfEscapement;
		strcpy(m_LogFont.lfFaceName,lfPtr->lfFaceName);
		m_LogFont.lfOrientation =  lfPtr->lfOrientation;
		m_LogFont.lfOutPrecision =  lfPtr->lfOutPrecision;
		m_LogFont.lfPitchAndFamily =  lfPtr->lfPitchAndFamily;
		m_LogFont.lfQuality =  lfPtr->lfQuality;
		m_LogFont.lfWidth = 0;//lfPtr->lfWidth;
	}
	m_stylecombo.EnableWindow(m_bNotOwnStyle);

	
	//TRACE(" Italic = %d, Underline = %d, Strikeout = %d\n",m_bItalic,m_bUnderline,m_bStrikeout);
	

	


	return 0;
}

void CTextDlg::_UpdateControls()
{
	PostMessage( __LOCALMSG_InternalUpdateControls );
}

void CTextDlg::ChooseFont(LOGFONT* font)
{

	memcpy(&m_LogFont,font,sizeof(LOGFONT));
	m_bItalic = m_LogFont.lfItalic;
	m_bUnderline = m_LogFont.lfUnderline;
	m_bStrikeout = m_LogFont.lfStrikeOut;

	m_Height = (UINT)(-m_LogFont.lfHeight);
	
}

void CTextDlg::GetSelectedFont(LOGFONT *lf)
{
	memcpy(lf,&m_LogFont,sizeof(LOGFONT));

	lf->lfItalic = m_bItalic;
	lf->lfUnderline = m_bUnderline;
	lf->lfStrikeOut 	= m_bStrikeout;
	lf->lfWeight = m_bBold ? FW_BOLD : FW_NORMAL;
	
}


void CTextDlg::OnBold()
{
	m_bBold = !m_bBold;
}
void CTextDlg::OnItalic()
{
	m_bItalic = !m_bItalic;
}
void CTextDlg::OnStrikeout()
{
	m_bStrikeout = !m_bStrikeout;
}
void CTextDlg::OnUnderline()
{
	m_bUnderline = !m_bUnderline;
}

BOOL CTextDlg::OnApply()
{
	UpdateData();
	m_pStyle = m_stylecombo.GetSelStyle();
	m_halign = m_haligncombo.GetSelHAlign();
	m_valign = m_valigncombo.GetSelVAlign();
	m_txtAngle = m_orientation.GetOrientationAngle();

	if(m_bNotOwnStyle && !m_pStyle) 
	{
		MessageBox("Choose style");
		return FALSE;
	}
	return TRUE;
	

}/*

void CTextDlg::OnOK()
{
	
}*/