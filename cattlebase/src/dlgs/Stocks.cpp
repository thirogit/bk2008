#include "stdafx.h"
#include "Stocks.h"
#include "NewStockDlg.h"
#include "EditStockDlg.h"
#include "../global_arrays.h"
#include "../dispacher/NewStockDispatcher.h"
#include "../CattleSQL.h"
#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CStocksDlg, CDialog)
	ON_BN_CLICKED(IDC_ADDBTN, OnAddNewStock)
	ON_BN_CLICKED(IDC_DELBTN, OnDelStock)
	ON_BN_CLICKED(IDC_EDITBTN, OnEditStock)
	ON_NOTIFY(NM_DBLCLK , IDC_LIST, OnDblClick)
END_MESSAGE_MAP()


CStocksDlg::CStocksDlg(CWnd* pParent/* = NULL*/) : CDialog(CStocksDlg::IDD,pParent)
{
}

void CStocksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control (pDX, IDOK  , m_StockOK ) ;
	DDX_Control (pDX, IDC_LIST  , m_StockList ) ;
	DDX_Control (pDX, IDC_EDITBTN,m_StockEdit);
	DDX_Control (pDX, IDC_ADDBTN,m_StockAdd);
	DDX_Control (pDX, IDC_DELBTN,m_StockDelete);
}


BOOL CStocksDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_StockOK.SetButtonImage( IDB_STOCKOK);	
	m_StockAdd.SetButtonImage( IDB_STOCKADD);	
	m_StockDelete.SetButtonImage( IDB_STOCKDEL);	
	m_StockEdit.SetButtonImage( IDB_STOCKEDIT);	
	SetWindowText(CTempStringLoader(IDS_EDITSTOCKS));
	return TRUE;
}

Stock* CStocksDlg::AddNewStock(CWnd* pParent)
{
	if(!(HAS_PRIVLEDGE(ADDMODOWN)))
	{
		AfxMessageBox(IDS_NOPRIVS);
		return NULL;
	}

	Stock* pNewStock = new Stock();
	std::auto_ptr<Stock> autoDeleteNewStock(pNewStock);
	CNewStockDlg newStockDlg(pNewStock,pParent);
	if(newStockDlg.DoModal() == IDOK)
	{
		if(IsDuplicate(pNewStock->GetStockCode()))
		{
			AfxMessageBox(IDS_DUPLSTOCKCODE);
			return NULL;
		}

		if(pNewStock->InsertThisStock(true))
		{
			autoDeleteNewStock.release();
			NewStockDispatcher::Dispatch(pNewStock);
			return pNewStock;
		}
		else
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGSTOCK);
		}
	}
	return NULL;

}

void  CStocksDlg::OnAddNewStock()
{
	AddNewStock(this);
	m_StockList.UpdateItemCount();
}

void  CStocksDlg::OnDelStock()
{
	
	int iSelStock = -1;
	Stock *pStock = NULL;

	if(!(HAS_PRIVLEDGE(DELFORG) || HAS_PRIVLEDGE(DELOWN) ))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}
	iSelStock = m_StockList.GetCurSel();
	if(iSelStock < 0) return;

	m_StockList.Lock();
	pStock = ary_stocks[iSelStock];

	if(!pStock->DeleteThisStock())
	{
		m_StockList.Unlock();
		AfxMessageBox(IDS_ERRCANTDELSTOCK,MB_OK,0);
		return;
	}
	
	ary_stocks.RemoveAt(iSelStock);
	m_StockList.UpdateItemCount();
	delete pStock;

	m_StockList.Unlock();
	
}
bool CStocksDlg::IsDuplicate(const CString &sStockCode)
{
	return ary_stocks.GetStockDataByStockCode(sStockCode) != NULL;
}

void CStocksDlg::OnEditStock()
{
	int iSelStock = -1;
	Stock *pStock = NULL;

	if(!HAS_PRIVLEDGE(ADDMODOWN))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}

	iSelStock = m_StockList.GetCurSel();
	if(iSelStock < 0) return;

	pStock = ary_stocks[iSelStock];
	CEditStockDlg editStockDlg(pStock,this);
	if(editStockDlg.DoModal() == IDOK)
	{
		if(!pStock->UpdateThisStock())
		{
			SQLDB.SQLMessageBox(IDS_ERRUPDATEINGSTOCK);
			pStock->RollbackChanges();
		}
		else
		{
			pStock->CommitChanges();
			m_StockList.Invalidate(FALSE);
		}
	}	
	

}
void CStocksDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnEditStock();
	*pResult = 0;
}
