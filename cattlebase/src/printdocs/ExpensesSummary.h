#ifndef __EXPENSESSUMMARY_H__
#define __EXPENSESSUMMARY_H__

#include "../printing/CowDrwDoc.h"
#include "../datatypes/Expense.h"
#include "../datatypes/ExpenseDataArray.h"

#define EXPENSESSUMMARYPAGESIZE 39

class CExpensesSummaryDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(CExpensesSummaryDoc)
protected:
	CExpensesSummaryDoc();
	LPCSTR GetExpenseType(Expense* pExpense);
public:
	int GetPrintPages() const;
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
	void PrintPage(int page);
	
private:
	CExpenseDataArray m_expensesToPrint;	
	Money m_totalNet;
	Money m_totalVAT;	
	Money m_totalGros;
	int m_startIndex;
	int m_endIndex;	
	int m_nPages;

};


#endif