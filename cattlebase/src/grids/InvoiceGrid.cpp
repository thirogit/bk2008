#include "stdafx.h"
#include "InvoiceGrid.h"
#include "MainCfg.h"
#include "global_arrays.h"

#include "../dlgs/invoice/BigInvoiceDlg.h"
#include "../datatypes/invoice/ChangeVATInvoiceSession.h"
#include "../datatypes/invoice/CorrectVATInvoiceSession.h"
#include "../datatypes/invoice/ChangeRRInvoiceSession.h"
#include "../datatypes/invoice/CorrectRRInvoiceSession.h"
#include "../datatypes/InvoiceDataComparator.h"
#include "../dlgs/AskForDateDlg.h"
#include "../classes/TempStringLoader.h"
#include "../classes/validators/InvoicePaidDateValidator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


GRIDHEADERS CInvoiceGrid::InvoiceHeader[] =
{
	{IDS_HEADERDOCNO,INVS_SORT_BY_NUMBER},
	{IDS_HEADERDOCDATE,INVS_SORT_BY_INVDATE},	
	{IDS_HEADERHENT,INVS_SORT_BY_HENT},
	{IDS_HEADERAMOUNT,INVS_SORT_BY_QTY},
	{IDS_HEADERVAT,INVS_SORT_BY_VAT},
	{IDS_HEADERPAYWAY,INVS_SORT_BY_PAYWAY},
	{IDS_HEADERCUSTNO,INVS_SORT_BY_CUSTOMNO},
	{IDS_HEADERBRUTTO,INVS_SORT_BY_GROSSVALUE},
	{IDS_HEADERCORRECT,INVS_SORT_BY_CORRECT},
	{IDS_HEADERVALAFTERCORRECT,INVS_SORT_BY_GROSSVALUEAFTERCORRECT},
	{IDS_HEADERPAYDUEDAYSDATE,INVS_SORT_BY_PAYDUEDAYSDATE},
	{IDS_HEADERPAID,INVS_SORT_BY_PAID},
	{IDS_HEADEREXTRAS,INVS_SORT_BY_EXTRAS},
	{0,0}
};

CInvoiceGrid::CInvoiceGrid(CInvoiceDataArray &invArray,int iConfigItem) : 
	DataGrid<CInvoiceDataArray,Invoice>(invArray,InvoiceHeader,	iConfigItem),
		m_textProvider(invArray)
{
}


bool CInvoiceGrid::GetDataCellColor(int iItem,int nCol,COLORREF &cr)
{
	Invoice *pInv = GetRowData(iItem);
	switch(GetColID(nCol))
	{
		case INVS_SORT_BY_NUMBER:
			switch(pInv->GetThisInvoiceType())
			{
			case IN_INV_RR:
			case IN_INV_VAT:
				cr = RGB(138,197,255);
			break;
			case CORRCT_INV:
				cr = RGB(250,189,107);
			break;
			case OUT_INV_VAT:
				cr = RGB(250,137,202);
			break;
			}
			return true;
		case INVS_SORT_BY_PAYDUEDAYSDATE:
			if(!pInv->IsPaid() && pInv->GetPayDueDate() < CDateTime::now())
			{
				cr = RGB(255,0,0);
				return true;
			}
			break;
	}
	return false;

}
BOOL CInvoiceGrid::GetToolTipText(int iItem,int iSubItem,CString &tooltipText)
{
	//char tooltipTextBuffer[80];
	//char szSuffix[] = "\n.\n.\n.";
	//InvoiceData *pInv = (InvoiceData*)GetItemPtr(iItem);
	//InvoiceTypes invtype = pInv->GetInvoiceType();
//	int size = pInv->invDocs.GetSize(); 
//	int curTipLen = strlen(szSuffix);

	//if(!pInv || GetColID(iSubItem) != INVS_SORT_BY_LINK) return FALSE;
	//if(size > 1)
	//{
	//	
	//	tooltipText.Empty();
	//	for(int i = 0;i < size;i++)
	//	{
 //           _snprintf(tooltipTextBuffer,sizeof(tooltipTextBuffer),g_szLinkFmt,
	//			g_szDocPrefix[invtype == OUT_INV_VAT],pInv->invDocs[i]->docid);
	//		curTipLen += strlen(tooltipTextBuffer)+1;
	//		if((curTipLen) < 80)
	//		{
	//			tooltipText += tooltipTextBuffer;
	//			tooltipText += '\n'; 				 
	//		}
	//		else
	//		{
	//			tooltipText += szSuffix;
	//			break;
	//		}
	//	}
	//	tooltipText.TrimRight('\n'); 
	//	return TRUE;

	//}
	return FALSE;

}


void CInvoiceGrid::GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData)
{
	m_textProvider.GetText(iItem,GetColID(nCol),pszText);
}

void CInvoiceGrid::AddPopMenuItem(CPopMenuNodeList &popmenulist)
{
	popmenulist.Add(new CPopMenuNode(0,0,0));
	popmenulist.Add(new CPopMenuNode(0,IDS_POPMARKPAID,IDR_MARKPAIDMENU));
	popmenulist.Add(new CPopMenuNode(IDM_GRID_INV_EDITCORRECT,IDS_POPDOCORRECTEDIT,0));

}

bool CInvoiceGrid::ChangePaidDate(Invoice* pInvoice,const CDateTime& dt,bool bCommit)
{	
	Invoice *pInv = pInvoice;
	pInv->ChangePaidDate(dt);
	if(!pInv->UpdateThisInvoice(false))
	{		
		SQLDB.Rollback();
		pInv->RollbackChanges();
		SQLDB.SQLMessageBox(IDS_ERRFAILTOMARKPAID);			
		return false;
	}	
	if(bCommit)
	{
		SQLDB.Commit();
		pInv->CommitChanges();
	}
	return true;
}

bool CInvoiceGrid::HandleMessage(UINT message)
{
	if(DataGrid<CInvoiceDataArray,Invoice>::HandleMessage(message))
		return true;

	switch(message)
	{
		case IDM_GRID_MARKPAID_YES_NOW:
		{
			if(!HAS_PRIVLEDGE(INVOICE))
			{
				AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
				break;
			}

			Invoice* pInvoice = GetSelectedRow();
			if(pInvoice)
			{
				if(pInvoice->HasCorrect())
				{
					AfxMessageBox(IDS_CANTCHNGPAIDDTDUETOCORRECT);
					break;
				}
				if(AfxMessageBox(IDS_MARKPAIDQUESTION,MB_YESNO,0) == IDNO)
				{
					break;
				}
			
				ChangePaidDate(pInvoice,CDateTime::now(),true);
			}		
			break;
		}
		case IDM_GRID_MARKPAID_YES_DATE:
		{
			if(!HAS_PRIVLEDGE(INVOICE))
			{
				AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
				break;
			}
			Invoice* pInvoice = GetSelectedRow();
			if(pInvoice)
			{
				if(pInvoice->HasCorrect())
				{
					AfxMessageBox(IDS_CANTCHNGPAIDDTDUETOCORRECT);
					break;
				}
				if(!pInvoice->IsPaid())
				{
					if(AfxMessageBox(IDS_MARKPAIDQUESTION,MB_YESNO,0) == IDNO)
						break;
				}
				CInvoicePaidDateValidator invPaidDtValidator(pInvoice);
				CAskForDateDlg askForPaidDt(this);
				askForPaidDt.SetLabel(CTempStringLoader(IDS_ASKFORPAIDDATELBL));
				askForPaidDt.SetWindowCaption(CTempStringLoader(IDS_ASKFORPAIDDATECAPTION));
				askForPaidDt.SetDate(pInvoice->GetPaidDate());
				askForPaidDt.SetValidator(&invPaidDtValidator);
				

				CDateTime paidDt = askForPaidDt.AskForDate();

				if(!paidDt.IsNull())
				{
					ChangePaidDate(pInvoice,paidDt,true);
				}
			}
			break;
		}
		case IDM_GRID_MARKPAID_NO:
		{
			if(!HAS_PRIVLEDGE(INVOICE))
			{
				AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
				break;
			}

			Invoice* pInvoice = GetSelectedRow();
			
			if(pInvoice)
			{
				if(pInvoice->HasCorrect())
				{
					AfxMessageBox(IDS_CANTCHNGPAIDDTDUETOCORRECT);
				}
				else
				{
					CDateTime nullDt;
					nullDt.MakeNull();
					ChangePaidDate(pInvoice,nullDt,true);
				}
			}				
			break;
		}
		case IDM_GRID_EDITROW:
		{
			CInvoiceViewDlg invview(GetRowData(GetCurSel()));
			invview.DoModal();
			break;
		}
		case IDM_GRID_INV_EDITCORRECT:
		{
			Invoice* pInvoice = (Invoice*)GetRowData(GetCurSel());
			DoCorrectEdit(pInvoice);			
			break;
		}
		
		default:
			return false;
	}
	return true;
}

bool CInvoiceGrid::HandleUpdateMessage(CCmdUI *pCmdUI)
{
	if(pCmdUI->m_nID == IDM_GRID_EDITROW)
	{
		CString itemText;
		itemText.LoadString(IDS_POPINVVIEW);
		pCmdUI->SetText(itemText);			
		pCmdUI->Enable(TRUE);
		return true;
	}
	return false;
}


CVATInvoiceGrid::CVATInvoiceGrid(CInvoiceDataArray &invArray,int iConfigItem) : 
	CInvoiceGrid(invArray,iConfigItem)
{
}


void CVATInvoiceGrid::DoCorrectEdit(Invoice *pInvoice)
{

	VATInvoice* pInvoiceVAT = (VATInvoice*)pInvoice;
	CowPriceInvoiceSession* pInvoiceSession = NULL;

	if(pInvoiceVAT->CanEditThisInvoice())
	{
		pInvoiceSession = new CChangeVATInvoiceSession(pInvoiceVAT,GetInvoiceContext());		
	}
	else
	{
		pInvoiceSession = new CCorrectVATInvoiceSession(pInvoiceVAT,GetInvoiceContext());		
	}	

	CBigInvoiceDlg bigInvoiceDlg(pInvoiceSession,this);
	bigInvoiceDlg.DoModal();
	delete pInvoiceSession;


}
bool CVATInvoiceGrid::HandleMessage(UINT message)
{
	if(CInvoiceGrid::HandleMessage(message))
		return true;
	
	return false;

}

MY_IMPLEMENT_DYNCREATE(CBuyVATInvoiceGrid,CBuyInvoiceGrid<CVATInvoiceGrid>)
CBuyVATInvoiceGrid::CBuyVATInvoiceGrid() : CBuyInvoiceGrid<CVATInvoiceGrid>(ary_buy_invs,CFGID_BUYINVGRID)
{
}

bool CBuyVATInvoiceGrid::HandleMessage(UINT message)
{
	if(CBuyInvoiceGrid<CVATInvoiceGrid>::HandleMessage(message))
		return true;

	
	return false;
	

}

InvoiceContext* CBuyVATInvoiceGrid::GetInvoiceContext()
{
	return &m_InvoiceContext;
}



MY_IMPLEMENT_DYNCREATE(CSellVATInvoiceGrid,CVATInvoiceGrid)
CSellVATInvoiceGrid::CSellVATInvoiceGrid() : CVATInvoiceGrid(ary_sell_invs,CFGID_SELLINVGRID)
{
}

InvoiceContext* CSellVATInvoiceGrid::GetInvoiceContext()
{
	return &m_InvoiceContext;
}

MY_IMPLEMENT_DYNCREATE(CRRInvoiceGrid,CBuyInvoiceGrid<CInvoiceGrid>)
CRRInvoiceGrid::CRRInvoiceGrid() : CBuyInvoiceGrid<CInvoiceGrid>(ary_rr_invs,CFGID_RRINVGRID)
{
}

void CRRInvoiceGrid::DoCorrectEdit(Invoice* pInvoice)
{

	RRInvoice* pInvoiceRR = (RRInvoice*)pInvoice;
	CowPriceInvoiceSession* pInvoiceSession = NULL;

	if(pInvoiceRR->CanEditThisInvoice())
	{
		pInvoiceSession = new CChangeRRInvoiceSession(pInvoiceRR,GetInvoiceContext());		
	}
	else
	{
		pInvoiceSession = new CCorrectRRInvoiceSession(pInvoiceRR,GetInvoiceContext());		
	}
	CBigInvoiceDlg bigInvoiceDlg(pInvoiceSession,this);
	bigInvoiceDlg.DoModal();
	delete pInvoiceSession;

}

InvoiceContext* CRRInvoiceGrid::GetInvoiceContext()
{
	return &m_InvoiceContext;
}





