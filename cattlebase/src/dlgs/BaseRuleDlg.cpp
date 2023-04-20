#include "stdafx.h"
#include "BaseRuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(CBaseRuleDlg, CDialog)
CBaseRuleDlg::CBaseRuleDlg(CWnd* pParent/* = NULL*/) : CDialog(CBaseRuleDlg::IDD, pParent), 
							m_buyrule(IDS_BUYCAPTION), 
							m_sellrule(IDS_SELLCAPTION)
			
{	
}

CBaseRuleDlg::~CBaseRuleDlg()
{
}

void CBaseRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_RULENAME,m_RuleName);
	DDX_Control(pDX,IDC_NOSEXVALUECB,m_noSexValueCB);
}
void CBaseRuleDlg::LoadNoSexValueCB(COWSEX cowSex)
{
	int xxItem,xyItem;
	CowSex XX(SEX_XX);
	CowSex XY(SEX_XY);

	ASSERT(cowSex != SEX_UNK);
	m_noSexValueCB.ResetContent();
	
	xxItem = m_noSexValueCB.AddString((LPCSTR)XX);
	m_noSexValueCB.SetItemData(xxItem,XX.getCOWSEX());

	xyItem = m_noSexValueCB.AddString((LPCSTR)XY);
	m_noSexValueCB.SetItemData(xyItem,XY.getCOWSEX());

	if(XX.getCOWSEX() == cowSex)
	{
		m_noSexValueCB.SetCurSel(xxItem);
	}
	else if(XY.getCOWSEX() == cowSex)
	{
		m_noSexValueCB.SetCurSel(xyItem);
	}
}

BEGIN_MESSAGE_MAP(CBaseRuleDlg, CDialog)

END_MESSAGE_MAP()



BOOL CBaseRuleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
			
	m_dlgPartSheet.AddPage(&m_buyrule);
	m_dlgPartSheet.AddPage(&m_sellrule);

	//m_buyrule.FillWithData(m_ruleData->GetRulePart(BUY));
	//m_sellrule.FillWithData(m_ruleData->GetRulePart(SELL));

	m_dlgPartSheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	
	m_dlgPartSheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_dlgPartSheet.ModifyStyle( 0, WS_TABSTOP );

	

	CRect rcSheet,cbRect;

	GetClientRect( &rcSheet );
	m_noSexValueCB.GetWindowRect(&cbRect);	
	ScreenToClient(&cbRect);
	
	int y = cbRect.bottom+5;
	m_dlgPartSheet.SetWindowPos( &m_noSexValueCB,0,y, 0,0,	 SWP_NOACTIVATE | SWP_NOSIZE);

	
	SetWindowText(GetWindowCaption());
	SetDlgItemText(IDOK,GetBtnCaption());
	LoadNoSexValueCB(SEX_XX);

	SetupControls();
	UpdateData(FALSE);

	return TRUE;
}
void CBaseRuleDlg::OnOK()
{
	UpdateData();
	
	if(!m_RuleName.GetLength())
	{
		AfxMessageBox(IDS_EMPTYRULENAME,MB_OK,0);
		GetDlgItem(IDC_RULENAME)->SetFocus();
		return;
	}

	int nosexvalueItemSelected = m_noSexValueCB.GetCurSel();
	if(nosexvalueItemSelected < 0)
	{
		AfxMessageBox(IDS_MISSINGNOSEXVALUE);
		m_noSexValueCB.SetFocus();
		return;
	}

	if(m_buyrule.m_hWnd)
		m_buyrule.UpdateData();

	if(m_sellrule.m_hWnd)
		m_sellrule.UpdateData();

	if(!Validate())
		return;

	
	if(CommitRule())
		EndDialog(IDOK);	
}
