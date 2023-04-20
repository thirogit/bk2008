// ExListBox.cpp : implementation file
//

#include "stdafx.h"

#include "RuleListBox.h"
#include "global_arrays.h"
#include "../../datatypes/CowPriceData.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRuleListBox

CRuleListBox::CRuleListBox(const InvoiceContext* pContext) : m_pInvoiceContext(pContext) 
{
}

CRuleListBox::~CRuleListBox()
{
}


BEGIN_MESSAGE_MAP(CRuleListBox, CExListBox)
	//ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuleListBox message handlers

BOOL CRuleListBox::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	//TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//CString strTipText;
	UINT nID = pNMHDR->idFrom;

	 if(!m_bToolTipCtrlCustomizeDone)
	{
		AFX_MODULE_THREAD_STATE* pThreadState = AfxGetModuleThreadState();
		CToolTipCtrl *pToolTip = pThreadState->m_pToolTip;
		// Set max tip width in pixel.
		// you can change delay time, tip text or background
		//   color also. enjoy yourself!
		pToolTip->SetMaxTipWidth(500);
		m_bToolTipCtrlCustomizeDone = TRUE;
	 }

	
	CString ruleDetails;
	if(nID >= 0)
	{
		Rule *rule  = (Rule*)GetItemData(nID);
		RulePart *rulePart = m_pInvoiceContext->GetRulePart(rule);
	
		ruleDetails.Format(IDS_RULEDETAILS,
					   rulePart->GetLimWeight(),
					   (LPCSTR)rulePart->GetPriceUnderWeightXY().ToString(),
					   (LPCSTR)rulePart->GetPriceUnderWeightXX().ToString(),
					   (LPCSTR)rulePart->GetPriceOverWeightXY().ToString(),
					   (LPCSTR)rulePart->GetPriceOverWeightXY().ToString());
		
	}
	else ruleDetails.LoadString(IDS_NORULESELECTED);
	

//	GetText( nID ,strTipText);

	strTipText = ruleDetails;

	//if (pNMHDR->code == TTN_NEEDTEXTA)
		pTTTA->lpszText = strTipText.GetBuffer();
	
	*pResult = 0;

	return TRUE;    
}

int CRuleListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CExListBox::OnCreate(lpCreateStruct) == -1) return -1;

	Reload();
	return 0;

}
void CRuleListBox::PreSubclassWindow()
{
	CExListBox::PreSubclassWindow();
	Reload();
}


void CRuleListBox::Reload()
{
	int i,item;
	Rule *rule;
	ResetContent();

	for(i=0;i<ary_rules.GetSize();i++)
	{
		rule = ary_rules[i];
		item = AddString(rule->GetRuleName());
		SetItemData(item,(DWORD_PTR)rule);
	}
}