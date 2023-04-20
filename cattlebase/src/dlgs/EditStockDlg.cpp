#include "stdafx.h"
#include "EditStockDlg.h"
#include "../missing/util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEditStockDlg::CEditStockDlg(Stock* pStock,CWnd* pParent) : CBaseStockDlg(pParent),m_pStock(pStock)
{
}

CString CEditStockDlg::GetDlgCaption()
{
	CString dlgCaption;
	dlgCaption.Format(IDS_EDITINGRACE,(LPCSTR)m_pStock->GetStockCode());
	return dlgCaption;
}

BOOL CEditStockDlg::OnInitDialog()
{
	CBaseStockDlg::OnInitDialog();
	
	m_StockCode.SetWindowText(m_pStock->GetStockCode());
	m_StockName = m_pStock->GetStockName();
	m_RuleListCombo.ReloadCombo(m_pStock->GetRule());
	m_StockPricePerKg.SetWindowText(m_pStock->GetMyPerKgPrice().ToString());
	m_predefSexCombo.SelSex(m_pStock->GetPredefSex().getCOWSEX());
	m_OfficialCode = m_pStock->GetOfficialCode();

	UpdateData(FALSE);
	return TRUE;
}
void CEditStockDlg::UpdateStock(Stock* pStock)
{
	CString sStockCode;
	m_StockCode.GetWindowText(sStockCode);
	pStock->ChangeStockCode(sStockCode);
	CString sStockName = m_StockName;
	pStock->ChangeStockName(sStockName);
	Rule* pRule = m_RuleListCombo.GetSelItemData();
	pStock->ChangeRule(pRule);
	
	CString sPricePerKg;
	m_StockPricePerKg.GetWindowText(sPricePerKg);

	pStock->ChangeMyPerKgPrice(Money(CUtil::myatof((LPCSTR)sPricePerKg)));

	CowSex cowsex = CowSex(m_predefSexCombo.GetSelSex());
	pStock->ChangePredefSex(cowsex);

	pStock->ChangeOfficialCode(m_OfficialCode);
}
bool CEditStockDlg::CommitStock()
{
	UpdateStock(m_pStock);
	return true;
}

void CEditStockDlg::CancelStock()
{
}
