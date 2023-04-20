#ifndef __EXPENSESLISTCTRL_H__
#define __EXPENSESLISTCTRL_H__

#include "DataTypes.h"
#include "BaseListCtrl.h"
#include "../datatypes/ExpenseDataArray.h"
#include "../textproviders/ExpenseDataTextProvider.h"
#include "../textproviders/BaseArrayTextProvider.h"

class CExpensesListCtrl : public CBaseListCtrl
{
public:
	CExpensesListCtrl(CExpenseDataArray *pExpenseArray,
						const GRIDHEADERS *ppHeaders,
						int iConfigId = -1); 
	void UpdateItemCount();
protected:
	CExpenseDataArray *m_pExpenseArray;
	BaseArrayTextProvider<CExpenseDataArray,ExpenseDataTextProvider> m_textProvider;	
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
protected:
	DECLARE_MESSAGE_MAP()

};




#endif