#include "stdafx.h"
#include "ExpensesGrid.h"
#include "../global_arrays.h"
#include "MainCfg.h"
#include "../datatypes/ExpenseDataComparator.h"
#include "../dlgs/EditExpenseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


MY_IMPLEMENT_DYNCREATE(CExpensesGrid,CGrid)

GRIDHEADERS CExpensesGrid::ExpensesGridHeader[] =
{
	
	{IDS_HEADEREXPENSETITLE,	EXPENSEDATA_EXPENSETITLE},
	{IDS_HEADEREXPENSECATEGORY,	EXPENSEDATA_EXPENSECATEGORY},
	{IDS_HEADERNETTO,			EXPENSEDATA_NETVALUE},
	{IDS_HEADERVATVALUE,		EXPENSEDATA_VATVALUE},
	{IDS_HEADERBRUTTO,			EXPENSEDATA_GROSVALUE},
	{IDS_HEADERVAT,				EXPENSEDATA_VATRATE},
	{IDS_HEADEREXPENSETYPE,		EXPENSEDATA_EXPENSETYPE},	
	{IDS_HEADERINVOICE,			EXPENSEDATA_INVOICENO},
	{IDS_HEADERDOCDATE,			EXPENSEDATA_EXPENSEDATE},
	{IDS_HEADEREXTRAS,			EXPENSEDATA_EXTRAS},
	{0,0}
};

CExpensesGrid::CExpensesGrid() : DataGrid<CExpenseDataArray,Expense>(ary_expenses,ExpensesGridHeader,CFGID_EXPENSESGRID),
										m_textProvider(ary_expenses)
{
}
void CExpensesGrid::GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData)
{
	m_textProvider.GetText(iItem,GetColID(nCol),pszText);
}

bool CExpensesGrid::HandleMessage(UINT message)
{
	if(DataGrid<CExpenseDataArray,Expense>::HandleMessage(message))
		return true;

	int iSelItem = GetCurSel();

	switch(message)
	{
		case IDM_GRID_EDITROW:
		{
			Expense *pExpense = GetRowData(iSelItem);
			CEditExpenseDlg expenseEdit(pExpense);
			expenseEdit.DoModal();					
			break;
		}
		default:
			return false;
	}

	return true;

}