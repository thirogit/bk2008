#include "stdafx.h"
#include "ExpensesSummary.h"
#include "../consts.h"
#include "../datatypes/ExpenseDataArray.h"
#include "../Utils/StringFormat.h"

IMPLEMENT_DYNCREATE(CExpensesSummaryDoc, CCowDrwDoc)

CExpensesSummaryDoc::CExpensesSummaryDoc() : m_startIndex(0),m_endIndex(0),m_nPages(0),
							m_totalNet(ZEROMONEY),m_totalVAT(ZEROMONEY),m_totalGros(ZEROMONEY)
{
}


int CExpensesSummaryDoc::GetPrintPages() const
{
	return m_nPages;
}
int CExpensesSummaryDoc::LoadInputData(CPtrArray &docInputData)
{
	m_expensesToPrint.RemoveAll();
	m_expensesToPrint.Append((CExpenseDataArray&)docInputData);

	m_totalNet = ZEROMONEY;
	m_totalVAT = ZEROMONEY;
	m_totalGros = ZEROMONEY;

	Expense* pExpense = NULL;
	CExpenseDataArray::ArrayIterator it = m_expensesToPrint.iterator();

	Money expenseNetValue;

	while(it.hasNext())
	{
		pExpense = *(it++);
		expenseNetValue = pExpense->GetGrosValue()/(1+pExpense->GetVATRate());
		m_totalNet += expenseNetValue;
		m_totalVAT += pExpense->GetGrosValue()-expenseNetValue;
		m_totalGros += pExpense->GetGrosValue();
	}

	int iExpensesSize = m_expensesToPrint.GetCount();
	m_nPages = (iExpensesSize/EXPENSESSUMMARYPAGESIZE);
	if(iExpensesSize%EXPENSESSUMMARYPAGESIZE) m_nPages++;

	return  RELOAD_OK_CONTINUE;
}

void CExpensesSummaryDoc::PreparePage(int nPage)
{    
	ResetGroupsVisibility(false);
	CString grpname;
		
	int iExpensesSize = m_expensesToPrint.GetCount();
	m_startIndex = EXPENSESSUMMARYPAGESIZE*(nPage-1);
	m_endIndex = m_startIndex + EXPENSESSUMMARYPAGESIZE - 1;
							
	if(m_endIndex >= iExpensesSize) m_endIndex = iExpensesSize-1;
	
	for(int i = m_startIndex, iGrp = 0;i <= m_endIndex;i++,iGrp++)
	{
		grpname.Format("%d",iGrp);
		SetGroupVisible(grpname,true);
	}
	if(nPage == m_nPages) SetGroupVisible("BOTTOM",true);				

}
void CExpensesSummaryDoc::PrintPage(int page)
{
	CString fld,str;
	int i,row;

	 
	DrawText("PAGES",CStringFormat("%d/%d",page,m_nPages));
	
	if(page == m_nPages)
	{
		DrawText("TOTALNETVALUE",m_totalNet.ToString());
		DrawText("TOTALVATVALUE",m_totalVAT.ToString());
		DrawText("TOTALGROSVALUE",m_totalGros.ToString());
	}

    Money expenseNetValue;
	Expense *pExpense = NULL;
	row = 0;
	for(i = m_startIndex;i <= m_endIndex;i++,row++)
	{
		pExpense = m_expensesToPrint[i];
	    		
		DrawText(CStringFormat("TITLE%d",row),pExpense->GetExpenseTitle());
		DrawText(CStringFormat("CATEGORY%d",row),pExpense->GetExpenseCategory()->GetExpenseCategoryName());
		DrawText(CStringFormat("TYPE%d",row),GetExpenseType(pExpense));		
		DrawText(CStringFormat("INVOICE%d",row),pExpense->GetInvoiceNo());
		DrawText(CStringFormat("DATE%d",row),pExpense->GetExpenseDate().GetDayDate());
		DrawText(CStringFormat("VAT%d",row),CStringFormat("%.2f%%",pExpense->GetVATRate()*100.0));
		expenseNetValue = pExpense->GetGrosValue()/(1+pExpense->GetVATRate());
		DrawText(CStringFormat("NETVALUE%d",row),expenseNetValue.ToString());
		DrawText(CStringFormat("VATVALUE%d",row),(pExpense->GetGrosValue()-expenseNetValue).ToString());
		DrawText(CStringFormat("GROSVALUE%d",row),(pExpense->GetGrosValue()).ToString());
		
	}
		
}

LPCSTR CExpensesSummaryDoc::GetExpenseType(Expense* pExpense)
{
	if(pExpense->GetExpenseType() == Expense::EXPENSE_TYPE_WITH_INVOICE)
		return "FAK";
	else
		return "GOT";
}

