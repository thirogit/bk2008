#include "stdafx.h"
#include "stocklistctrl.h"
#include "DataTypes.h"
#include "MainCfg.h"
#include "../../datalimits.h"
#include "../../global_arrays.h"
#include "../../datatypes/StockDataComparator.h"


const GRIDHEADERS CStockListCtrl::szColHeaders[] =
{
	{IDS_HEADERSHORT,STOCKS_SORT_BY_STOCKCODE},
	{IDS_HEADERNAME,STOCKS_SORT_BY_STOCKNAME},
	{IDS_HEADERRULE,STOCKS_SORT_BY_RULE},
	{IDS_HEADERMYPERKGPRICE,STOCKS_SORT_BY_MYPERKGPRICE},	
	{IDS_HEADERPREDEFSEX,STOCK_SORT_BY_PREDEFSEX},
	{IDS_HEADEROFFICIALSTOCKCODE,STOCK_SORT_BY_OFFICIALCODE},
	{0,0}
};

BEGIN_MESSAGE_MAP(CStockListCtrl, CBaseListCtrl)
   END_MESSAGE_MAP()

CStockListCtrl::CStockListCtrl() : CBaseListCtrl(szColHeaders,CFGID_STOCKLIST) 
{

}
void CStockListCtrl::UpdateItemCount()
{
	SetItemCount(ary_stocks.GetSize());
}

void CStockListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	CString sText;
	Stock *stock = ary_stocks[nItem];
	switch(GetColID(nSubItem))
	{
	case STOCKS_SORT_BY_STOCKCODE:
		sText = stock->GetStockCode();
	break;
	case STOCKS_SORT_BY_STOCKNAME:
		sText = stock->GetStockName();
	break;
	case STOCKS_SORT_BY_RULE:
		if(stock->GetRule())
			sText = stock->GetRule()->GetRuleName();
		else sText = "-";
	break;
	case STOCKS_SORT_BY_MYPERKGPRICE:
		sText = stock->GetMyPerKgPrice().ToString();
	break;
	case STOCK_SORT_BY_PREDEFSEX:
		sText = (LPCSTR)stock->GetPredefSex();
	break;
	case STOCK_SORT_BY_OFFICIALCODE:
		sText = stock->GetOfficialCode();
	break;
	default:
		ASSERT(0);
	}
	strcpy(pszItemText,sText);
}

void CStockListCtrl::SortByCol(int Column,bool bAsc)
{
	ary_stocks.SortBy(GetColID(Column),bAsc);
}
