#include "stdafx.h"
#include "NewStockDlg.h"
#include "../missing/util.h"
#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNewStockDlg::CNewStockDlg(Stock* pStock,CWnd* pParent) : 
			CBaseStockDlg(pParent),m_pStock(pStock)
{
}

CString CNewStockDlg::GetDlgCaption()
{
	return CTempStringLoader(IDS_NEWRACE);
}

bool CNewStockDlg::CommitStock()
{
	Stock* pNewStock = CreateNewStock();
	m_pStock->CopyFrom(*pNewStock);
	delete pNewStock;
	return true;
}
void CNewStockDlg::CancelStock()
{

}
Stock* CNewStockDlg::CreateNewStock()
{
	Stock *pNewStock = new Stock;
	CString sStockCode;
	m_StockCode.GetWindowText(sStockCode);
	pNewStock->SetStockCode(sStockCode);
	pNewStock->SetStockName(m_StockName);
	pNewStock->SetRule(m_RuleListCombo.GetSelItemData());
	CString sPricePerKg;
	m_StockPricePerKg.GetWindowText(sPricePerKg);
	pNewStock->SetMyPerKgPrice(Money(CUtil::myatof((LPCSTR)sPricePerKg)));
	pNewStock->SetPredefSex(m_predefSexCombo.GetSelSex());
	pNewStock->SetOfficialCode(m_OfficialCode);
	return pNewStock;
}