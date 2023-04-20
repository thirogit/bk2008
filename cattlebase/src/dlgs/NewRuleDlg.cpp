#include "stdafx.h"
#include "NewRuleDlg.h"

#include "../classes/TempStringLoader.h"
#include "../Utils/StringFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNewRuleDlg::CNewRuleDlg(Rule* pNewRule,CWnd* pParent/* = NULL*/) : CBaseRuleDlg(pParent), m_pNewRule(pNewRule)
{
}
CString CNewRuleDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_NEWRULECAPTION);
}
CString CNewRuleDlg::GetBtnCaption()
{
	return CTempStringLoader(IDS_BTNADD);
}
bool CNewRuleDlg::CommitRule()
{
	Rule* pNewRule = new Rule(NULL_RULEID);

	pNewRule->SetNoSexValue(CowSex((COWSEX)m_noSexValueCB.GetItemData(m_noSexValueCB.GetCurSel())));
	pNewRule->SetRuleName(m_RuleName);

	if(m_buyrule.m_hWnd && m_sellrule.m_hWnd)
	{
		m_buyrule.UpdateRulePart(pNewRule->GetRulePart(BUY));
		m_sellrule.UpdateRulePart(pNewRule->GetRulePart(SELL));
	}
	else
	{
		ASSERT("VALIDATE SHOULD NOT ALLOW TO GET HERE" && FALSE);
		delete pNewRule;
		return false;
	}

	m_pNewRule->CopyFrom(*pNewRule);
	delete pNewRule;
	return true;	
}
void CNewRuleDlg::SetupControls()
{
}

bool CNewRuleDlg::Validate()
{
	if(m_buyrule.m_hWnd)
	{
		if(!m_buyrule.ValidateFields(false))
		{
			m_dlgPartSheet.SetActivePage(&m_buyrule);
			return m_buyrule.ValidateFields(true);
		}
	}
	else
	{
		AfxMessageBox(CStringFormat(CTempStringLoader(IDS_ASKFILLTAB),(LPCSTR)CTempStringLoader(IDS_BUYCAPTION)));
		return false;
	}

	if(m_sellrule.m_hWnd)
	{
		if(!m_sellrule.ValidateFields(false))
		{
			m_dlgPartSheet.SetActivePage(&m_sellrule);
			return m_sellrule.ValidateFields(true);
		}
	}
	else
	{
		AfxMessageBox(CStringFormat(CTempStringLoader(IDS_ASKFILLTAB),(LPCSTR)CTempStringLoader(IDS_SELLCAPTION)));
		return false;
	}
	return true;
}