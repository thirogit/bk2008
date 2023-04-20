#ifndef __MAINFRM_CTRL_H__
#define __MAINFRM_CTRL_H__

#include "msg_defs.h"
#include "gridtabs.h"

void SendMainFrmMessage(UINT msg,WPARAM wParam, LPARAM lParam);

template<class ARRAY_TYPE> 
void GetGridSelectedRows (UINT grid,ARRAY_TYPE & array)
{
	SendMainFrmMessage(WM_GETSELROWS,(WPARAM)&array,(LPARAM)grid);
}


#define GetSelCows(ptrarray) GetGridSelectedRows<CCowArray>(ALLCOWSGRID,ptrarray)
#define GetSelInCows(ptrarray) GetGridSelectedRows<CCowArray>(INCOWSGRID,ptrarray)
#define GetSelInDocs(ptrarray) GetGridSelectedRows<CDocDataArray>(INDOCSGRID,ptrarray)
#define GetSelOutDocs(ptrarray) GetGridSelectedRows<CDocDataArray>(OUTDOCSGRID,ptrarray)
#define GetSelHents(ptrarray) GetGridSelectedRows<CHentArray>(HENTSGRID,ptrarray)
#define GetSelBuyInvoices(ptrarray) GetGridSelectedRows<CInvoiceDataArray>(BUYINVSGRID,ptrarray)
#define GetSelSellInvoices(ptrarray) GetGridSelectedRows<CInvoiceDataArray>(SELLINVSGRID,ptrarray)
#define GetSelRRInvoices(ptrarray) GetGridSelectedRows<CInvoiceDataArray>(RRINVSGRID,ptrarray)
#define GetSelExpenses(ptrarray) GetGridSelectedRows<CExpenseDataArray>(EXPENSESGRID,ptrarray)


#define UpdateGrid(grid) SendMainFrmMessage(WM_UPDATEGRID,grid,0)
#define ChangeGrid(grid) SendMainFrmMessage(WM_SETACTIVEPANE,grid,0)
#define LockGrid(grid) SendMainFrmMessage(WM_LOCKGRID,grid,0)


#endif
