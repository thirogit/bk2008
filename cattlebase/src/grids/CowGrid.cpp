#include "stdafx.h"
#include "CowGrid.h"
#include "../consts.h"
#include "../MainCfg.h"
#include "../global_arrays.h"

#include "../PassportViewing/PassImgViewDlg.h"
#include "../Cow/EditCowDlg.h"
#include "../CattleSQL.h"
#include "../classes/match/CowMatcher.h"
#include "../classes/forallactions/CountAction.h"
#include "../datatypes/CowDataComparator.h"
#include "../Doc/InDoc/EditInDocDlg.h"
#include "../dlgs/EditOutDocDlg.h"
#include "../dlgs/invoice/InvoiceViewDlg.h"
#include "../dlgs/EditHentDlg.h"
#include "../datatypes/invoice/NewRRInvoiceSession.h"
#include "../dlgs/logindlg.h"
#include "../dlgs/CowStockStatDlg.h"
#include "../classes/IntString.h"
#include "../datatypes/invoice/RRInvoiceContext.h"
#include "../dlgs/invoice/BigInvoiceDlg.h"
#include "../dlgs/invoice/SmallInvoiceDlg.h"
#include "../Zebra/ZebraException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUY_SELL_CHNG_CLR RGB(125,125,255);

CCowGrid::CCowGrid(CCowArray &cowarray, const GRIDHEADERS *ppHeaders, int pConfigItemId) :
		DataGrid<CCowArray,Cow>(cowarray,ppHeaders,pConfigItemId)
{
	m_mws_opts = CMarkWarnShowOptions::GetInstance();
}

void CCowGrid::GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData)
{
	Cow *cowd = GetRowData(iItem);
	CString temp;
	
	const char plus[] = "+";
	const char minus[] = "-";
	
	switch(GetColID(nCol))
	{
	case COWS_SORT_BY_STATE:
		temp = cowd->IsOut() ?  minus : plus;
		break;
	case COWS_SORT_BY_EAN:
		temp = cowd->GetEAN();
		break;
	case COWS_SORT_BY_PASSNO:
		temp = cowd->GetPassNo();
		break;
	case COWS_SORT_BY_DELIVER:
		temp = cowd->GetDeliver()->GetAlias();
		break;
	case COWS_SORT_BY_FSTOWNER:
		if(cowd->HasFirstOwner())
			temp = cowd->GetFirstOwner()->GetAlias();
		else
			temp = szMissing;
		break;
	case COWS_SORT_BY_BUYER:
		if(cowd->IsOut())
			temp = cowd->GetBuyer()->GetAlias();
		else temp = minus;
		break;
	case COWS_SORT_BY_INDATE:
		temp = cowd->GetInDoc()->GetLoadDate().GetDayDate();
		break;
	case COWS_SORT_BY_WEIGHT:
		temp = cowd->GetWeight().ToString();
		break;
	case COWS_SORT_BY_SEX:
		temp = (LPCSTR)cowd->GetSex();
		break;
	case COWS_SORT_BY_STOCK:
		temp = cowd->GetStock()->GetStockCode();			
		break;
	case COWS_SORT_BY_INDOCNO:
		temp = cowd->GetInDoc()->GetDocNoStr();
		break;
	case COWS_SORT_BY_OUTDOCNO:
		if(cowd->IsOut())
			temp = cowd->GetOutDoc()->GetDocNoStr();
		else temp = "-";
		break;

	case COWS_SORT_BY_OUT_DATE:
		if(cowd->IsOut())
			temp = cowd->GetOutDoc()->GetDocDate().GetDayDate();
		else
			temp = "-";
		break;
	case COWS_SORT_BY_MYPRICE:
		if(m_mws_opts->GetShowPrices())
		{
			temp = cowd->GetMyPrice().ToString();  			
		}
		else
		{
			temp = szHiddenCol;
		}
		break;
	case COWS_SORT_BY_BUYPRICE:
		if(m_mws_opts->GetShowPrices())
		{
			if(cowd->HasAnyBuyInvoice())
			{
				Money actBuyPrice = cowd->GetActBuyPrice();
				Money netBuyPrice = cowd->GetBuyPrice();
				if(actBuyPrice == netBuyPrice)
					temp = netBuyPrice.ToString();
				else
				{
					temp.Format("%.2f (%.2f)",actBuyPrice.GetDouble(),netBuyPrice.GetDouble());
				}
			}
			else temp = "-";
		}
		else
		{
			temp = szHiddenCol;
		}
		break;			
	case COWS_SORT_BY_SELLPRICE:
		if(m_mws_opts->GetShowPrices())
		{
			temp = "-";
			if(cowd->HasVATSellInvoice())
				temp = cowd->GetSellPrice().ToString();
		}
		else
		{
			temp = szHiddenCol;
		}
		break;
	case COWS_SORT_BY_EXTRAS:
		
		temp = cowd->GetExtras();
		temp.Replace('\n',';');
		temp.Remove('\r');
		
		break;

	case COWS_SORT_BY_BIRTHDATE:
		temp = cowd->GetBirthDate().GetDayDate();
		break;

	case COWS_SORT_BY_TERMBUYSTOCK:
	
		if(cowd->GetTermBuyStock())
			temp = cowd->GetTermBuyStock()->GetStockCode();
	break;

	case COWS_SORT_BY_TERMBUYWEIGHT:
		temp = cowd->GetTermBuyWeight().ToString();
		
	break;

	case COWS_SORT_BY_TERMBUYPRICE:
		if(m_mws_opts->GetShowPrices())
		{
			temp = cowd->GetTermBuyPrice().ToString();
		}
		else
		{
			temp = szHiddenCol;
		}
				
	break;
	case COWS_SORT_BY_TERMSELLSTOCK:
		if(cowd->GetTermSellStock())
			temp = cowd->GetTermSellStock()->GetStockCode();
	break;
	case COWS_SORT_BY_TERMSELLWEIGHT:
		temp = cowd->GetTermSellWeight().ToString();
		break;
	case COWS_SORT_BY_TERMSELLPRICE:
		if(m_mws_opts->GetShowPrices())
		{
			temp = cowd->GetTermSellPrice().ToString();
		}
		else
		{
			temp = szHiddenCol;
		}						
		break;
	case COWS_SORT_BY_RRINV:
		if(cowd->HasRRBuyInvoice())
		{
			temp = cowd->GetRRInvoice()->GetInvoiceNoStr();
		}
		else
			temp = szMissing;
		break;
	case COWS_SORT_BY_INAGENT:
		if(cowd->GetInDoc()->GetAgent() != NULL)
		{
			temp = cowd->GetInDoc()->GetAgent()->GetAgentCode();
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	lstrcpy(pszText,(LPCSTR)temp);
		
}

BOOL CCowGrid::GetToolTipText(int iItem,int iSubItem,CString &tooltipText)
{
	
		/*Cow *cowd = GetRowData(iItem);
		if(!cowd) return FALSE;
		bool bShowHint = false;

		Hent *hent = NULL;
		switch(GetColID(iSubItem))
		{
			case COWS_SORT_BY_DELIVER:
				hent = cowd->GetDeliver();
				break;
			case COWS_SORT_BY_FSTOWNER:
				hent = cowd->GetFirstOwner();
				break;
			case COWS_SORT_BY_BUYER:
				hent = cowd->GetBuyer();
				break;
			case COWS_SORT_BY_WEIGHT:
				if(cowd->docin->invoice || cowd->rrinv)
				{
					if(!cowd->GetBuyWeight().IsNull())
					{
						tooltipText.Format(IDS_BUYWEIGHTTOOLTIP,cowd->GetBuyWeight().ToInt());
						bShowHint = true;
					}
				}
				if(cowd->docout)
				{
					if(cowd->docout->invoice)
					{
						if(!cowd->GetSellWeight().IsNull())
						{
							if(bShowHint)
								tooltipText += '\n';
							tooltipText.AppendFormat(IDS_SELLWEIGHTTOOLTIP,cowd->GetSellWeight().ToInt());
							bShowHint = true;
						}
					}
				}
			break;
			case COWS_SORT_BY_STOCK:
				if(cowd->HasBuyVATInvoice() || cowd->HasRRBuyInvoice())
				{
						tooltipText.Format(IDS_BUYSTOCKTOOLTIP,(LPCSTR)cowd->GetBuyStock()->stockcode);
						bShowHint = true;
				}
				if(cowd->docout)
				{
					if(cowd->docout->invoice)
					{
						if(cowd->GetSellStock())
						{
							if(bShowHint)
								tooltipText += '\n';
							tooltipText.AppendFormat(IDS_SELLSTOCKTOOLTIP,(LPCSTR)cowd->GetSellStock()->stockcode);
							bShowHint = true;
						}
					}
				}
			break;
				

		}
		if(hent)
		{
			tooltipText.Format("%s\n%s %s\n%s %s",(LPCSTR)hent->name,(LPCSTR)hent->street,(LPCSTR)hent->pobox,(LPCSTR)hent->zip,(LPCSTR)hent->city);
			bShowHint = true;
		}
		return bShowHint;*/
	return FALSE;
	
}

void CCowGrid::AddPopMenuItem(CPopMenuNodeList &popmenulist)  
{
	popmenulist.Add(new CPopMenuNode());
	popmenulist.Add(new CPopMenuNode(IDM_GRID_COWS_SHOWHIDEMYPRICE,IDS_SHOWHIDEMYPRICE,0));
	popmenulist.Add(new CPopMenuNode(IDM_GRID_COWS_MAKERRINVOICE,IDS_MAKERRINVOICE,0));
	popmenulist.Add(new CPopMenuNode(IDM_GRID_COWS_SHOWSTOCKSTAT,IDS_COWSTOCKSTAT,0));
    popmenulist.Add(new CPopMenuNode());

	popmenulist.Add(new CPopMenuNode(0,IDS_SHOWRELATED,IDR_SHOWMENU));
	popmenulist.Add(new CPopMenuNode(0,IDS_COMPUTE,IDR_SUMMENU));
	popmenulist.Add(new CPopMenuNode(IDM_GRID_COWS_PRINTCOWLABEL,IDS_PRINTCOWLABEL,0));
	
}
bool CCowGrid::GetDataCellColor(int iItem,int nCol,COLORREF &cr)
{
	bool bRet = false;
	
	Cow *pCow = GetRowData(iItem);
	switch(GetColID(nCol))
	{
	case COWS_SORT_BY_STATE:
		cr = pCow->IsOut() ? g_SoldThisCowClr : g_GotThisCowClr;
		bRet = true;
	break;

	case COWS_SORT_BY_SELLPRICE:
	case COWS_SORT_BY_BUYPRICE:
		if((pCow->HasAnyBuyInvoice()) && pCow->HasVATSellInvoice())
		{
			if(m_mws_opts->GetMarkLoss() && pCow->GetActBuyPrice() > pCow->GetActSellPrice())
			{
				cr = m_mws_opts->GetMarkLossColor();
				bRet = true;
			}

			if(m_mws_opts->GetWarnToBigProfit() && (pCow->GetActSellPrice()-m_mws_opts->GetMaxProfit()) > pCow->GetActBuyPrice())
			{
				cr = m_mws_opts->GetWarnToBigProfitColor();
				bRet = true;
			}				
		}			
		break;
	case COWS_SORT_BY_BIRTHDATE:

		if(m_mws_opts->GetMarkAge())
		{
			if(pCow->HasBirthDate())
			{
				int diffInDays = pCow->GetBirthDate().GetDiffrenceInDays(CDateTime::now());

				if(diffInDays > m_mws_opts->GetMarkDays())
				{
					cr = m_mws_opts->GetMarkCowAgeColor();
					bRet = true;
				}
				else
				{
					if(m_mws_opts->GetWarnAge())
					{
						if(diffInDays > (m_mws_opts->GetMarkDays()-m_mws_opts->GetWarnDays()))
						{
							cr = m_mws_opts->GetWarnCowAgeColor();
							bRet = true;
						}
					}
				}
			}
		}

	break; 

	case COWS_SORT_BY_WEIGHT:
			if(pCow->HasAnyBuyInvoice())
			{
				if(pCow->GetBuyWeight() != pCow->GetWeight().GetDouble())
				{
					cr = BUY_SELL_CHNG_CLR;
					bRet = true;
				}
			}
			if(pCow->HasVATSellInvoice())
			{
				if(pCow->GetSellWeight() != pCow->GetWeight().GetDouble())
				{
					cr = BUY_SELL_CHNG_CLR;
					bRet = true;
				}
			}
		break;
		case COWS_SORT_BY_STOCK:
			if(pCow->HasAnyBuyInvoice())
			{
				if(pCow->GetBuyStock() != pCow->GetStock())
				{
					cr = BUY_SELL_CHNG_CLR;
					bRet = true;
				}
			}
			if(pCow->HasVATSellInvoice())
			{
				if(pCow->GetSellStock() != pCow->GetStock())
				{
					cr = BUY_SELL_CHNG_CLR;
					bRet = true;
				}
			}
		break;
		
	}
	return bRet;	
}


void CCowGrid::OnDoubleClick(int iItem,int iSubItem)
{
	if(GetAsyncKeyState( VK_CONTROL )&0x8000) 
		AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND,IDM_GRID_SHOWMENU_PASSIMG);
	else
		DataGrid<CCowArray,Cow>::OnDoubleClick(iItem,iSubItem);		
}

bool CCowGrid::HandleMessage(UINT message)
{
	if(DataGrid<CCowArray,Cow>::HandleMessage(message))
		return true;

	int iSelItem = GetCurSel();

	switch(message)
	{
		
		case IDM_GRID_EDITROW:
			DoCowEdit(GetRowData(iSelItem));
			Invalidate(FALSE);
			break;
			
		case IDM_GRID_COWS_SHOWSTOCKSTAT:
		{
			CCowStockStatDlg cowStockStatDlg(m_dataArray,this);
			cowStockStatDlg.DoModal();

			break;
		}

		case IDM_GRID_SHOWMENU_SHOWINDOC:
		case IDM_GRID_SHOWMENU_SHOWOUTDOC:
		case IDM_GRID_SHOWMENU_SHOWFSTOWNR:
		case IDM_GRID_SHOWMENU_SHOWDELIVER:
		case IDM_GRID_SHOWMENU_SHOWBUYER:
		case IDM_GRID_SHOWMENU_SHOWBUYINV:
		case IDM_GRID_SHOWMENU_SHOWSELLINV:
		case IDM_GRID_SHOWMENU_SHOWRRINV:
		case IDM_GRID_SHOWMENU_PASSIMG:
			OnShow(message);
			break;

		case IDM_GRID_SUMMENU_SUMMYPRICE:
		case IDM_GRID_SUMMENU_SUMBUYPRICE:
		case IDM_GRID_SUMMENU_SUMSELLPRICE:
			OnSumPrices(message);
			break;
		case IDM_GRID_SUMMENU_SUB_BUYMYPRICE:
		case IDM_GRID_SUMMENU_SUB_SELLBUYPRICE:
		case IDM_GRID_SUMMENU_SUB_SELLMYPRICE:
			OnSumSubPrices(message);		
			break;
		case IDM_GRID_COWS_SHOWHIDEMYPRICE:
			ShowHideMyPrice();
			break;
		case IDM_GRID_COWS_MAKERRINVOICE:
			OnMakeRRInvoice();
			break;
		case IDM_GRID_COWS_PRINTCOWLABEL:
			OnPrintCowLabel(GetRowData(iSelItem));
			break;


		default:
			return false;
	}
	return true;
}
bool CCowGrid::HandleUpdateMessage(CCmdUI *pCmdUI)
{
	int iCurSel = GetCurSel(); 
	BOOL bEnable = FALSE;
    Cow *pCow = NULL;
	if(iCurSel >= 0)
		pCow = (Cow*)GetItemPtr(iCurSel);


	switch(pCmdUI->m_nID)
	{
		
		case IDM_GRID_SHOWMENU_SHOWDELIVER:
		case IDM_GRID_SHOWMENU_SHOWFSTOWNR:
		case IDM_GRID_SHOWMENU_SHOWINDOC:
			bEnable = TRUE;
		break;
		case IDM_GRID_SHOWMENU_SHOWBUYER:
		case IDM_GRID_SHOWMENU_SHOWOUTDOC:
			bEnable = pCow->IsOut();
		break;
		
		case IDM_GRID_SHOWMENU_SHOWBUYINV:
			bEnable = pCow->HasVATBuyInvoice();
		break;
		case IDM_GRID_SHOWMENU_SHOWSELLINV:
			bEnable = pCow->HasVATSellInvoice();
		break;
		case IDM_GRID_SHOWMENU_SHOWRRINV:
			bEnable = pCow->HasRRBuyInvoice();
		break;
		case IDM_GRID_COWS_SHOWHIDEMYPRICE:
			bEnable = HAS_PRIVLEDGE(INVOICE);
			break;
		case IDM_GRID_SUMMENU_SUMBUYPRICE:
		case IDM_GRID_SUMMENU_SUMMYPRICE:
		case IDM_GRID_SUMMENU_SUMSELLPRICE:
		case IDM_GRID_SUMMENU_SUB_SELLBUYPRICE:
		case IDM_GRID_SUMMENU_SUB_SELLMYPRICE:
		case IDM_GRID_SUMMENU_SUB_BUYMYPRICE:
			bEnable = CMarkWarnShowOptions::GetInstance()->GetShowPrices();
			break;


		default:
			return false;
	}
	pCmdUI->Enable(bEnable);

	return true;
}


void CCowGrid::DoCowEdit(Cow *cowd)
{
	ASSERT(!cowd->IsUncommited());
	CEditCowDlg cowEditDlg(cowd,this);
	cowEditDlg.LockEANEdit();
	
	if(cowEditDlg.DoModal() == IDOK)
	{
		if(cowd->CanEditThisCow())
		{
			if(!cowd->UpdateThisCow())
			{
				SQLDB.SQLMessageBox(IDS_ERRUPDATEINGCOW);
				cowd->RollbackChanges();
				return;
			}
			cowd->CommitChanges();
		}
		else
		{
			cowd->RollbackChanges();
			AfxMessageBox(IDS_ERRCANTEDITCOW,MB_OK,0);
		}
	}		
}


void CCowGrid::OnShow(UINT nID)
{
	Cow *pCow = (Cow*)GetItemPtr(GetCurSel());

	switch(nID)
	{
		case IDM_GRID_SHOWMENU_SHOWINDOC:
		{
			CEditInDocDlg docEditDlg((InDoc*)pCow->GetInDoc());
			docEditDlg.DoModal();
		break;
		}
		case IDM_GRID_SHOWMENU_SHOWOUTDOC:
		{
			CEditOutDocDlg docedit((OutDoc*)pCow->GetOutDoc());
			docedit.DoModal();
		break;
		}
		case IDM_GRID_SHOWMENU_SHOWFSTOWNR:
			if(pCow->HasFirstOwner())
				ShowHent(pCow->GetFirstOwner());
			else
				AfxMessageBox(IDS_CANNOTSHOWFSTOWNR,MB_OK,0);
		break;
		case IDM_GRID_SHOWMENU_SHOWDELIVER:
			ShowHent(pCow->GetDeliver());
		break;
		case IDM_GRID_SHOWMENU_SHOWBUYER:
			if(pCow->IsOut())
				ShowHent(pCow->GetBuyer());
		break;		
					 		
		case IDM_GRID_SHOWMENU_SHOWBUYINV:
		{
			if(HAS_PRIVLEDGE(INVOICE) && pCow->HasVATBuyInvoice())
			{
				CInvoiceViewDlg invview(pCow->GetBuyVATInvoice());
				invview.DoModal();
			}
			else
				AfxMessageBox(IDS_NOPRIVS,MB_OK);
		break;
		}
		case IDM_GRID_SHOWMENU_SHOWSELLINV:
		{
			if(HAS_PRIVLEDGE(INVOICE) && pCow->HasVATSellInvoice())
			{
				CInvoiceViewDlg invview(pCow->GetSellVATInvoice());
				invview.DoModal();
			}
			else
				AfxMessageBox(IDS_NOPRIVS,MB_OK);

		break;
		}
		case IDM_GRID_SHOWMENU_SHOWRRINV:
		{
			if(HAS_PRIVLEDGE(INVOICE) && pCow->HasRRBuyInvoice())
			{
				CInvoiceViewDlg invview(pCow->GetRRInvoice());
				invview.DoModal();
			}
			else
				AfxMessageBox(IDS_NOPRIVS,MB_OK);
			break;
		}
		case IDM_GRID_SHOWMENU_PASSIMG:
		{
			CPassImgViewDlg passView(m_dataArray,GetCurSel());
			passView.DoModal();
			break;
		}

	}

}

void CCowGrid::ShowHent(Hent* pHent)
{
	CEditHentDlg hentedit(pHent);
	hentedit.DoModal();
}

void CCowGrid::OnSumPrices(UINT nID)
{
	int s,i;
	CCowArray cowAry;
	Money sum(ZEROMONEY);
	Money (Cow::*fGetPrice)() const = NULL;
	CString priceMsg;
	
	GetSelectedRows(cowAry);	
	

	s = cowAry.GetSize(); 
	if(s > 0)
	{
		switch(nID)
		{
		case IDM_GRID_SUMMENU_SUMMYPRICE:
			fGetPrice = &Cow::GetMyPrice;
			break;
		case IDM_GRID_SUMMENU_SUMBUYPRICE:
			fGetPrice = &Cow::GetActBuyPrice;
			break;
		case IDM_GRID_SUMMENU_SUMSELLPRICE:
			fGetPrice = &Cow::GetActSellPrice;
			break;
		}
		for(i=0;i<s;i++)
			sum += (cowAry[i]->*fGetPrice)();
		priceMsg.Format(IDS_PRICEMSG,sum.GetDouble());
		AfxMessageBox(priceMsg,MB_OK,0);

	}	
}
void CCowGrid::OnSumSubPrices(UINT nID)
{
	int s,i,cnt = 0;
	CCowArray cowAry;
	Money sum1(ZEROMONEY),sum2(ZEROMONEY);
	Money (Cow::*fGetPrice1)() const = NULL;
	Money (Cow::*fGetPrice2)() const = NULL;
	CString priceMsg;
	Money price1,price2;
		
	GetSelectedRows(cowAry);	
	
	s = cowAry.GetSize(); 
	if(s > 0)
	{
		switch(nID)
		{
		case IDM_GRID_SUMMENU_SUB_BUYMYPRICE:
			fGetPrice1 = &Cow::GetActBuyPrice;
			fGetPrice2 = &Cow::GetMyPrice;
			break;
		case IDM_GRID_SUMMENU_SUB_SELLBUYPRICE:
			fGetPrice1 = &Cow::GetActSellPrice;
			fGetPrice2 = &Cow::GetActBuyPrice;
			break;
		case IDM_GRID_SUMMENU_SUB_SELLMYPRICE:
			fGetPrice1 = &Cow::GetActSellPrice;
			fGetPrice2 = &Cow::GetMyPrice;
			break;
		}
		for(i=0;i<s;i++)
		{
			price1 = (cowAry[i]->*fGetPrice1)();
			price2 = (cowAry[i]->*fGetPrice2)();
			if(price1 >= 0.01 && price2 >= 0.01)
			{
				sum1 += price1;
				sum2 += price2;
				cnt++;
			}
		}
		sum1 -= sum2;
		priceMsg.Format(IDS_SUBPRICEMSG,cnt,sum1.GetDouble());
		AfxMessageBox(priceMsg,MB_OK,0);

	}	

}

//***************************************************************************************
void CCowGrid::OnMakeRRInvoice()
{
	CCowArray cows;
	
	GetSelectedRows(cows);		

	if(cows.GetSize() > 0)
	{
		CCowArray::ArrayIterator cowIt = cows.iterator();
		while(cowIt.hasNext())
		{
			if((*(cowIt++))->HasRRBuyInvoice())
			{
				AfxMessageBox(IDS_COWSHAVERRINV,MB_OK,0);
				return;
			}
		}

		if(!RRInvoice::DoesAllCowsBelongToSameInDoc(cows))
		{
			AfxMessageBox(IDS_COWSNOTINSAMEINDOC,MB_OK,0); 
			return;
		}
		if(!RRInvoice::DoesAllCowsHasOwners(cows))
		{
			AfxMessageBox(IDS_NOTALLCOWSHAVEOWNRS,MB_OK,0); 
			return;
		}

		if(!RRInvoice::HaveSameOwner(cows))
		{
			AfxMessageBox(IDS_COWSHAVEDIFFOWNR,MB_OK,0);
			return;
		}		
		if(!RRInvoice::CanMakeInvoice(cows))
		{
			AfxMessageBox(IDS_CANTMAKERRINV,MB_OK,0);
			return;
		}

		RRInvoiceContext rrInvContext;
		NewRRInvoiceSession rrInvoiceSession(cows,&rrInvContext);
		
		SmallInvoiceDlg smallInvoiceDlg(&rrInvoiceSession,this);
		smallInvoiceDlg.DoModal();
		//CBigInvoiceDlg bigInvoiceDlg(&rrInvoiceSession,this);
		//bigInvoiceDlg.DoModal();
	
	}

}


void CCowGrid::ShowHideMyPrice()
{
	CMarkWarnShowOptions *mwsopts = CMarkWarnShowOptions::GetInstance();
	bool bShow = !mwsopts->GetShowPrices();
	if(bShow)
	{
		CLoginDlg loginDlg(UserData::GetCurrentUser(),false);
		bShow = loginDlg.DoModal() == IDOK;
	}
	mwsopts->SetShowPrices(bShow);
	Invalidate(FALSE);
}


void CCowGrid::OnPrintCowLabel(const Cow* pCow)
{
	try
	{
		m_ZebraPOS.Print(pCow->GetEAN());
	}
	catch(ZebraException* e)
	{
		AfxMessageBox((LPCSTR)e->GetError(),MB_OK);
		e->Delete();
	}
}
//***************************************************************************************
GRIDHEADERS CAllCowsGrid::AllCowHeader[] =
{
	{IDS_HEADERSTATE,COWS_SORT_BY_STATE},
	{IDS_HEADERCOWNO,COWS_SORT_BY_EAN},
	{IDS_HEADERPASSNO,COWS_SORT_BY_PASSNO},
	{IDS_HEADERDELIVER,COWS_SORT_BY_DELIVER},
	{IDS_HEADERFSTOWNER,COWS_SORT_BY_FSTOWNER},
	{IDS_HEADERBUYER,COWS_SORT_BY_BUYER},
	{IDS_HEADERINDATE,COWS_SORT_BY_INDATE},
	{IDS_HEADEROUTDATE,COWS_SORT_BY_OUT_DATE},
	{IDS_HEADERWEIGHT,COWS_SORT_BY_WEIGHT},
	{IDS_HEADERSEX,COWS_SORT_BY_SEX},
	{IDS_HEADERSTOCK,COWS_SORT_BY_STOCK},
	{IDS_HEADERINDOCNO,COWS_SORT_BY_INDOCNO},
	{IDS_HEADEROUTDOCNO,COWS_SORT_BY_OUTDOCNO},
	{IDS_HEADERAGENT,COWS_SORT_BY_INAGENT},
	{IDS_HEADERMYPRICE,COWS_SORT_BY_MYPRICE},
	{IDS_HEADERRRINV,COWS_SORT_BY_RRINV},
	{IDS_HEADERBUYPRICE,COWS_SORT_BY_BUYPRICE},
	{IDS_HEADERSELLPRICE,COWS_SORT_BY_SELLPRICE},
	{IDS_HEADERTERMBUYSTOCK,COWS_SORT_BY_TERMBUYSTOCK},
	{IDS_HEADERTERMBUYWEIGHT,COWS_SORT_BY_TERMBUYWEIGHT},
	{IDS_HEADERTERMBUYPRICE,COWS_SORT_BY_TERMBUYPRICE},
	{IDS_HEADERTERMSELLSTOCK,COWS_SORT_BY_TERMSELLSTOCK},
	{IDS_HEADERTERMSELLWEIGHT,COWS_SORT_BY_TERMSELLWEIGHT},
	{IDS_HEADERTERMSELLPRICE,COWS_SORT_BY_TERMSELLPRICE},
	{IDS_HEADEREXTRAS,COWS_SORT_BY_EXTRAS},

	{0,0}
};

MY_IMPLEMENT_DYNCREATE(CAllCowsGrid,CCowGrid)

CAllCowsGrid::CAllCowsGrid() : CCowGrid(ary_cows,AllCowHeader,CFGID_AGRID)
{
}


//***************************************************************************************

GRIDHEADERS CInCowsGrid::InCowHeader[] =
{
	{IDS_HEADERCOWNO,COWS_SORT_BY_EAN},
	{IDS_HEADERPASSNO,COWS_SORT_BY_PASSNO},
	{IDS_HEADERDELIVER,COWS_SORT_BY_DELIVER},
	{IDS_HEADERFSTOWNER,COWS_SORT_BY_FSTOWNER},
	{IDS_HEADERBIRTHDATE,COWS_SORT_BY_BIRTHDATE},
	{IDS_HEADERINDATE,COWS_SORT_BY_INDATE},
	{IDS_HEADERWEIGHT,COWS_SORT_BY_WEIGHT},
	{IDS_HEADERSEX,COWS_SORT_BY_SEX},
	{IDS_HEADERSTOCK,COWS_SORT_BY_STOCK},
	{IDS_HEADERINDOCNO,COWS_SORT_BY_INDOCNO},
	{IDS_HEADERMYPRICE,COWS_SORT_BY_MYPRICE},
	{IDS_HEADERBUYPRICE,COWS_SORT_BY_BUYPRICE},
	{IDS_HEADERAGENT,COWS_SORT_BY_INAGENT},
	{IDS_HEADERRRINV,COWS_SORT_BY_RRINV},
	{IDS_HEADERTERMBUYSTOCK,COWS_SORT_BY_TERMBUYSTOCK},
	{IDS_HEADERTERMBUYWEIGHT,COWS_SORT_BY_TERMBUYWEIGHT},
	{IDS_HEADERTERMBUYPRICE,COWS_SORT_BY_TERMBUYPRICE},
	{IDS_HEADERTERMSELLSTOCK,COWS_SORT_BY_TERMSELLSTOCK},
	{IDS_HEADERTERMSELLWEIGHT,COWS_SORT_BY_TERMSELLWEIGHT},
	{IDS_HEADERTERMSELLPRICE,COWS_SORT_BY_TERMSELLPRICE},
	{IDS_HEADEREXTRAS,COWS_SORT_BY_EXTRAS},
	{0,0}
};



MY_IMPLEMENT_DYNCREATE(CInCowsGrid,CCowGrid)
CInCowsGrid::CInCowsGrid() : CCowGrid(ary_incows,InCowHeader,CFGID_INCOWSGRID)
{
}
