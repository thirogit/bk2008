#include "stdafx.h"
#include "EditRuleDlg.h"

#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEditRuleDlg::CEditRuleDlg(Rule* pRule,CWnd* pParent/* = NULL*/) : CBaseRuleDlg(pParent),
			m_pRule2Edit(pRule)
{
}
CString CEditRuleDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_EDITRULECAPTION);
}
CString CEditRuleDlg::GetBtnCaption()
{
	return CTempStringLoader(IDS_BTNEDIT);
}
bool CEditRuleDlg::CommitRule()
{
	m_pRule2Edit->ChangeNoSexValue(CowSex((COWSEX)m_noSexValueCB.GetItemData(m_noSexValueCB.GetCurSel())));
	m_pRule2Edit->ChangeRuleName(m_RuleName);

	if(m_buyrule.m_hWnd)
	{
		RulePart buyRulePart,*pBuyPart2Edit = m_pRule2Edit->GetRulePart(BUY);
		m_buyrule.UpdateRulePart(&buyRulePart);

		pBuyPart2Edit->ChangeLimWeight(buyRulePart.GetLimWeight());
		pBuyPart2Edit->ChangePriceOverWeightXX(buyRulePart.GetPriceOverWeightXX());
		pBuyPart2Edit->ChangePriceUnderWeightXX(buyRulePart.GetPriceUnderWeightXX());
		pBuyPart2Edit->ChangePriceOverWeightXY(buyRulePart.GetPriceOverWeightXY());
		pBuyPart2Edit->ChangePriceUnderWeightXY(buyRulePart.GetPriceUnderWeightXY());
		
	}
	if(m_sellrule.m_hWnd)
	{
		RulePart sellRulePart,*pSellPart2Edit = m_pRule2Edit->GetRulePart(SELL);
		m_sellrule.UpdateRulePart(&sellRulePart);

		pSellPart2Edit->ChangeLimWeight(sellRulePart.GetLimWeight());
		pSellPart2Edit->ChangePriceOverWeightXX(sellRulePart.GetPriceOverWeightXX());
		pSellPart2Edit->ChangePriceUnderWeightXX(sellRulePart.GetPriceUnderWeightXX());
		pSellPart2Edit->ChangePriceOverWeightXY(sellRulePart.GetPriceOverWeightXY());
		pSellPart2Edit->ChangePriceUnderWeightXY(sellRulePart.GetPriceUnderWeightXY());
	}
	return true;	

}
void CEditRuleDlg::SetupControls()
{
		LoadNoSexValueCB(m_pRule2Edit->GetNoSexValue().getCOWSEX());
		m_RuleName = m_pRule2Edit->GetRuleName();
		m_buyrule.InitWith(m_pRule2Edit->GetRulePart(BUY));
		m_sellrule.InitWith(m_pRule2Edit->GetRulePart(SELL));
}


bool CEditRuleDlg::Validate()
{
	if(m_buyrule.m_hWnd)
	{
		if(!m_buyrule.ValidateFields(false))
		{
			m_dlgPartSheet.SetActivePage(&m_buyrule);
			return m_buyrule.ValidateFields(true);
		}
	}
	

	if(m_sellrule.m_hWnd)
	{
		if(!m_sellrule.ValidateFields(false))
		{
			m_dlgPartSheet.SetActivePage(&m_sellrule);
			return m_sellrule.ValidateFields(true);
		}
	}
	
	return true;
}