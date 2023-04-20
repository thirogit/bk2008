#ifndef __EXPENSESGRID_H__
#define __EXPENSESGRID_H__

#include "popmenunode.h"
#include "myruntimeclass.h"
#include "DataGrid.h"
#include "../datatypes/ExpenseDataArray.h"
#include "../DataTypes.h"
#include "../taskprogress/ProgressIFace.h"
#include "../textproviders/ExpenseDataTextProvider.h"
#include "../textproviders/BaseArrayTextProvider.h"

class CExpensesGrid : public DataGrid<CExpenseDataArray,Expense>
{
	MY_DECLARE_DYNCREATE(CExpensesGrid)
public:
	CExpensesGrid();
protected:
	void GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData);
	virtual UINT GetGridDataTypeId() { return DATATYPE_EXPENSES; }
	//virtual void AddPopMenuItem(CPopMenuNodeList &popmenulist);
	virtual bool HandleMessage(UINT message);
private:
	static GRIDHEADERS ExpensesGridHeader[];
	BaseArrayTextProvider<CExpenseDataArray,ExpenseDataTextProvider> m_textProvider;
};

#endif