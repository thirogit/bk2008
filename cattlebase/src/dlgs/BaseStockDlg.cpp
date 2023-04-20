#include "stdafx.h"
#include "BaseStockDlg.h"
#include "NewRuleDlg.h"
#include "../consts.h"
#include "../datalimits.h"
#include "../missing/util.h"
#include "../global_arrays.h"
#include "../CattleSQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CBaseStockDlg, CDialog)
		ON_BN_CLICKED(IDC_ADDRULE,OnAddRule)
END_MESSAGE_MAP()


CBaseStockDlg::CBaseStockDlg(CWnd* pParent/* = NULL*/) : CDialog(CBaseStockDlg::IDD,pParent),
								m_StockPricePerKg(g_szRegExUnsigned)
{
}

void CBaseStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text (pDX, IDC_STOCKNAME  , m_StockName ) ;
	DDX_Control (pDX, IDC_STOCKCODE  , m_StockCode ) ;
	DDX_Control(pDX,IDC_MYPRICEPERKG,m_StockPricePerKg);
	DDX_Control( pDX,IDC_STOCKRULECB,m_RuleListCombo);
	DDX_Control(pDX,IDC_PREDEFSEXCB,m_predefSexCombo);
	DDX_Text(pDX,IDC_OFFICIALSTOCKCODE,m_OfficialCode);
}
BOOL CBaseStockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(GetDlgCaption());
	SendDlgItemMessage(IDC_STOCKNAME, EM_LIMITTEXT, MAXSTOCKNAME, 0);	
	m_RuleListCombo.ReloadCombo();
	CString sStockCodeRegEx;
	sStockCodeRegEx.Format("[A-Za-z0-9_]{1,%d}",MAXSTOCKCODE);
	m_StockCode.SetRegEx((LPCSTR)sStockCodeRegEx);	
	m_predefSexCombo.ReloadCombo();
	return TRUE;
}

void CBaseStockDlg::OnAddRule()
{
	if(!(HAS_PRIVLEDGE(ADDMODOWN) ||  HAS_PRIVLEDGE(INVOICE)))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}

	Rule *pNewRule = new Rule(NULL_RULEID);
	CNewRuleDlg newRuleDlg(pNewRule,this);
	if(newRuleDlg.DoModal() == IDOK)
	{
		if(!pNewRule->InsertThisRule())
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGRULE);
			delete pNewRule;
			return;
		}
		ary_rules.Add(pNewRule);
		m_RuleListCombo.ReloadCombo(pNewRule);
	}
	else 
		delete pNewRule;
}

bool CBaseStockDlg::Validate()
{
	if(m_StockCode.GetWindowTextLength() == 0 || !m_StockCode.IsInputValid())
	{
		AfxMessageBox(IDS_ERRINVALIDSTOCKCODE);
		m_StockCode.SetFocus();
		return false;
	}
	return true;
}

void CBaseStockDlg::OnOK()
{
	UpdateData();

	if(Validate())
	{
		if(CommitStock())
			EndDialog(IDOK);
	}
}
void CBaseStockDlg::OnCancel()
{
	CancelStock();
	EndDialog(IDCANCEL);
}