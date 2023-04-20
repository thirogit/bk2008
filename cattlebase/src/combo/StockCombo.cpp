#include "stdafx.h"
#include "StockCombo.h"
#include "global_arrays.h"

CStockCombo::CStockCombo() : CBasePtrCombo<CComboBox,Stock>(ary_stocks.GetSize())
{
}

Stock *CStockCombo::GetItemText(int item,CString &itemText)
{
	Stock *itemStock = ary_stocks[item];
	itemText = itemStock->GetStockCode();
	return itemStock;
}

int CStockCombo::GetNewItemCount()
{
	return ary_stocks.GetSize();

}

/*
CFindStockCombo::CFindStockCombo() : CStockCombo(),m_neurtalItem(-1){};

void CFindStockCombo::InitFindStockCombo()
{
	ReloadCombo();
	m_neurtalItem  = AddString("*");
	SetItemData(m_neurtalItem,NULL);
	SetCurSel(m_neurtalItem);
}
bool CFindStockCombo::IsNeutral()
{
	return (GetCurSel() == m_neurtalItem);
}*/
