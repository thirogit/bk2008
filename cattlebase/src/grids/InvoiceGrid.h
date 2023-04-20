#ifndef __INVOICEGRID_H__
#define __INVOICEGRID_H__

#include "popmenunode.h"
#include "myruntimeclass.h"
#include "../datatypes/InvoiceDataArray.h"
#include "DataGrid.h"
#include "../DataTypes.h"
#include "../textproviders/BaseArrayTextProvider.h"
#include "../textproviders/InvoiceDataTextProvider.h"
#include "../dlgs/invoice/invoiceviewdlg.h"
#include "../MoneyTransfer/ui/MoneyTransferDlg.h"
#include "../classes/match/InvMatcher.h"
#include "../classes/forallactions/SelectorAction.h"
#include "../datatypes/invoice/InvoiceContext.h"
#include "../datatypes/invoice/SellVATInvoiceContext.h"
#include "../datatypes/invoice/BuyVATInvoiceContext.h"
#include "../datatypes/invoice/RRInvoiceContext.h"
#include "CattleSQL.h"

class CInvoiceGrid : public DataGrid<CInvoiceDataArray,Invoice>
{
public:
	CInvoiceGrid(CInvoiceDataArray &invArray,int iConfigItem);
protected:
	void GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData);
	virtual void AddPopMenuItem(CPopMenuNodeList &popmenulist);
	bool GetDataCellColor(int iItem,int nCol,COLORREF &cr);
	BOOL GetToolTipText(int iItem,int iSubItem,CString &tooltipText);
	virtual bool HandleMessage(UINT message);
	virtual UINT GetGridDataTypeId() { return DATATYPE_INVS;  };
	bool ChangePaidDate(Invoice* pInvoice,const CDateTime& dt,bool bCommit);
	BaseArrayTextProvider<CInvoiceDataArray,InvoiceDataTextProvider> m_textProvider;
	virtual void DoCorrectEdit(Invoice *pInvoice) = 0;
	virtual bool HandleUpdateMessage(CCmdUI *pCmdUI);
	virtual InvoiceContext* GetInvoiceContext() = 0;
private:
	static GRIDHEADERS InvoiceHeader[];
		
};

class CVATInvoiceGrid : public CInvoiceGrid
{
public:
	CVATInvoiceGrid(CInvoiceDataArray &invArray,int iConfigItem);
protected:
	virtual bool HandleMessage(UINT message);
	virtual UINT GetGridDataTypeId() { return CInvoiceGrid::GetGridDataTypeId() | DATATYPE_VATINVS; };
	virtual void DoCorrectEdit(Invoice *pInvoice);
	
};


template<class BASEINVCLASS>
class CBuyInvoiceGrid : public BASEINVCLASS
{
public:

	CBuyInvoiceGrid(CInvoiceDataArray &invArray,int iConfigItem) : BASEINVCLASS(invArray,iConfigItem) {};

	virtual UINT GetGridDataTypeId() { return BASEINVCLASS::GetGridDataTypeId() | DATATYPE_BUYINVS; };
	virtual void AddPopMenuItem(CPopMenuNodeList &popmenulist)
	{
		BASEINVCLASS::AddPopMenuItem(popmenulist);
		popmenulist.Add(new CPopMenuNode(IDM_GRID_INV_MONEYTRANSFER,IDS_MONEYTRANSFER,0));
	}

	bool HandleMessage(UINT message)
	{
		if(BASEINVCLASS::HandleMessage(message))
			return true;

		switch(message)
		{
			
			case IDM_GRID_INV_MONEYTRANSFER:
			{				
				CInvoiceDataArray selectedInvs;
				GetSelectedRows(selectedInvs);
				DoMoneyTransfer(selectedInvs);
				break;
			}	

		
			default:
				return false;
		}
		return true;
	}


	void DoMoneyTransfer(CInvoiceDataArray& invoices)
	{

		if(!HAS_PRIVLEDGE(INVOICE))
		{
			AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
			return;
		}

		CMoneyTransferDlg moneyTransferDlg(invoices,this);
		moneyTransferDlg.DoModal();
	}
};


class CBuyVATInvoiceGrid : public CBuyInvoiceGrid<CVATInvoiceGrid>
{
	MY_DECLARE_DYNCREATE(CBuyVATInvoiceGrid)
public:
	CBuyVATInvoiceGrid();
protected:
	virtual UINT GetGridDataTypeId() { return CBuyInvoiceGrid<CVATInvoiceGrid>::GetGridDataTypeId() | DATATYPE_BUYVATINVS; };	
	virtual bool HandleMessage(UINT message);	
	virtual InvoiceContext* GetInvoiceContext();
private:
	BuyVATInvoiceContext m_InvoiceContext;
};

class CSellVATInvoiceGrid : public CVATInvoiceGrid
{
	MY_DECLARE_DYNCREATE(CSellVATInvoiceGrid)
public:
	CSellVATInvoiceGrid();
protected:
	
	virtual UINT GetGridDataTypeId() { return CVATInvoiceGrid::GetGridDataTypeId() | DATATYPE_SELLINVS | DATATYPE_SELLVATINVS; };		
	virtual InvoiceContext* GetInvoiceContext();
private:
	SellVATInvoiceContext m_InvoiceContext;
};


class CRRInvoiceGrid : public CBuyInvoiceGrid<CInvoiceGrid>
{
	MY_DECLARE_DYNCREATE(CRRInvoiceGrid)
public:
	CRRInvoiceGrid();
protected:
	virtual UINT GetGridDataTypeId() { return CBuyInvoiceGrid<CInvoiceGrid>::GetGridDataTypeId()  | DATATYPE_RRINVS; };		
	virtual void DoCorrectEdit(Invoice *pInvoice);
	virtual InvoiceContext* GetInvoiceContext();
private:
	RRInvoiceContext m_InvoiceContext;
};





#endif