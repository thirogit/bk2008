// RulesView.cpp : implementation file
//
#include "stdafx.h"
#include "RulesView.h"
#include "EditRuleDlg.h"
#include "NewRuleDlg.h"
#include "DataTypes.h"
#include "MainCfg.h"
#include "CattleSQL.h"
#include "../classes/TempStringLoader.h"

const GRIDHEADERS CRuleListCtrl::szRuleListCols[] = 
{
	{IDS_HEADERNAME,COL_RULENAME},
	{IDS_HEADERLIMWEIGHT,COL_LIMWEIGHT},
	{IDS_HEADERPRICEUNDERXY,COL_PRICEUNDERXY},
	{IDS_HEADERPRICEOVERXY,COL_PRICEOVERXY},
	{IDS_HEADERPRICEUNDERXX,COL_PRICEUNDERXX},
	{IDS_HEADERPRICEOVERXX,COL_PRICEOVERXX},
	{0,0}
};

BEGIN_MESSAGE_MAP(CRuleListCtrl,CBaseListCtrl)
	

END_MESSAGE_MAP()

CRuleListCtrl::CRuleListCtrl() : CBaseListCtrl(szRuleListCols,CFGID_RULELIST)
{
}

void CRuleListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	const char intFmt[] = "%d / %d",strFmt[] = "%s / %s";
	CString sText;

	Rule *rule = ary_rules[nItem];
	RulePart* pBuyRule = rule->GetRulePart(BUY);
	RulePart* pSellRule = rule->GetRulePart(SELL);

	switch(nSubItem)
	{
		case COL_RULENAME:
			sText = rule->GetRuleName();
		break;
		case COL_LIMWEIGHT:
			sText.Format(intFmt,pBuyRule->GetLimWeight(),pSellRule->GetLimWeight());
			break;
		case COL_PRICEUNDERXY:
			sText.Format(strFmt,(LPCSTR)(pBuyRule->GetPriceUnderWeightXY().ToString()),
								(LPCSTR)(pSellRule->GetPriceUnderWeightXY()).ToString());			
		break;
		case COL_PRICEOVERXY:
			sText.Format(strFmt,(LPCSTR)(pBuyRule->GetPriceOverWeightXY().ToString()),
								(LPCSTR)(pSellRule->GetPriceOverWeightXY().ToString()));			
		break;
		case COL_PRICEUNDERXX:
			sText.Format(strFmt,(LPCSTR)(pBuyRule->GetPriceUnderWeightXX().ToString()),
								(LPCSTR)(pSellRule->GetPriceUnderWeightXX().ToString()));			
		break;
		case COL_PRICEOVERXX:
			sText.Format(strFmt,(LPCSTR)(pBuyRule->GetPriceOverWeightXX().ToString()),
								(LPCSTR)(pSellRule->GetPriceOverWeightXX().ToString()));
		break;
		default:
			ASSERT(0);
			
	}
	strcpy(pszItemText,(LPCSTR)sText);
}

void CRuleListCtrl::UpdateItemCount()
{
	SetItemCount(ary_rules.GetSize());
}



// CRulesView dialog

IMPLEMENT_DYNAMIC(CRulesView, CDialog)
CRulesView::CRulesView(CWnd* pParent /*=NULL*/)
	: CDialog(CRulesView::IDD, pParent)
{
}

CRulesView::~CRulesView()
{
}

void CRulesView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST,m_rules);
	DDX_Control (pDX, IDOK  , m_OKBtn ) ;
	DDX_Control (pDX, IDC_EDITBTN,m_EditBtn);
	DDX_Control (pDX, IDC_ADDBTN,m_AddBtn);
	DDX_Control (pDX, IDC_DELBTN,m_DeleteBtn);
}


BEGIN_MESSAGE_MAP(CRulesView, CDialog)
	ON_BN_CLICKED(IDC_ADDBTN,OnAddRule)
	ON_NOTIFY(NM_DBLCLK , IDC_LIST, OnDblClick)
	ON_BN_CLICKED(IDC_DELBTN,OnDelRule)
	ON_BN_CLICKED(IDC_EDITBTN,OnRuleEdit)

	
END_MESSAGE_MAP()


BOOL CRulesView::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_OKBtn.SetButtonImage( IDB_STOCKOK);	
	m_AddBtn.SetButtonImage( IDB_STOCKADD);	
	m_DeleteBtn.SetButtonImage( IDB_STOCKDEL);	
	m_EditBtn.SetButtonImage( IDB_STOCKEDIT);

	SetWindowText(CTempStringLoader(IDS_EDITRULES));

	return TRUE;
}


void CRulesView::OnAddRule()
{
	if(!(HAS_PRIVLEDGE(ADDMODOWN)  || HAS_PRIVLEDGE(INVOICE)))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}

	Rule *newRule = new Rule(NULL_RULEID);
	CNewRuleDlg  newRuleDlg(newRule,this);
	if(newRuleDlg.DoModal() == IDOK)
	{
		if(newRule->InsertThisRule())
		{	
			ary_rules.Add(newRule);
			m_rules.UpdateItemCount();
		}
		else 
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGRULE);
			delete newRule;
		}
	}
	else 
		delete newRule;
}

void CRulesView::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		if(!(HAS_PRIVLEDGE(ADDMODOWN)  || HAS_PRIVLEDGE(INVOICE)))
		{
			AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
			*pResult = 0;
			return;
		}

		nItem = pNMIA->iItem;

		if(nItem >= 0)
		{
			EditRule(nItem);	
		}
	}

	*pResult = 0;
}

void CRulesView::OnDelRule()
{
	int nSelItem;
	Rule *rule2del = NULL;


	if(!(HAS_PRIVLEDGE(DELFORG) || HAS_PRIVLEDGE(DELOWN) || HAS_PRIVLEDGE(INVOICE)))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}

	POSITION pos = m_rules.GetFirstSelectedItemPosition();
	if(pos)
	{
		nSelItem = m_rules.GetNextSelectedItem(pos);
		rule2del = ary_rules[nSelItem];
		if(rule2del->DeleteThisRule())
		{
			ary_rules.RemoveAt(nSelItem);
			delete rule2del;
			m_rules.UpdateItemCount();
		}
		else SQLDB.SQLMessageBox(IDS_ERRDELETINGRULE);
	
	}

}

void CRulesView::EditRule(int iListIndex)
{
	Rule* pRule = ary_rules[iListIndex];
	CEditRuleDlg editRuleDlg(pRule,this);
	if(editRuleDlg.DoModal() == IDOK)
	{
		if(!pRule->UpdateThisRule())
		{
			pRule->RollbackChanges();
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGRULE);					
		}
		else
		{
			pRule->CommitChanges();
		}
	}
}

void CRulesView::OnRuleEdit()
{
	int nItem = m_rules.GetCurSel();
	if(nItem >= 0)
		EditRule(nItem);
}
