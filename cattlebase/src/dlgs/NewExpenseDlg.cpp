#include "stdafx.h"
#include "NewExpenseDlg.h"
#include "../classes/TempStringLoader.h"
#include "../dispacher/ExpenseDispatcher.h"
#include "../CattleSQL.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNewExpenseDlg::CNewExpenseDlg(CWnd* pParent/* = NULL*/) : CExpenseDlg(pParent)
{
}

CString CNewExpenseDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_NEWEXPENSEDLGCAPTION);
}

void CNewExpenseDlg::InitDialogControls()
{
	CExpenseDlg::InitDialogControls();
	m_btnaction.SetWindowText((LPCSTR)CTempStringLoader(IDS_BTNADD));
	
}

bool CNewExpenseDlg::CommitExpense()
{
	Expense* pNewExpense = CreateExpense();	
	if(pNewExpense->InsertThisExpense())
	{
		ExpenseDispatcher::Dispatch(pNewExpense);
		return true;
	}
	else
	{
		SQLDB.SQLMessageBox(IDS_ERRINSERTINGEXPENSE);
		delete pNewExpense;
		return false;
	}
	
}