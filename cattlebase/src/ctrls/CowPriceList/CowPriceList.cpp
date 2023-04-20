#include "stdafx.h"
#include "CowPriceList.h"
#include "../../Cow/EditCowDlg.h"
#include "../../combo/GtLtCombo.h"
#include "MainCfg.h"
#include "../stocklistctrl/RuleComboCell.h"
#include "../../classes/TempStringLoader.h"
#include "CattleSQL.h"
#include "../../datatypes/SellCowPriceDataComparator.h"
#include "../../datatypes/BuyCowPriceDataComparator.h"
#include "../../datatypes/invoice/InvoiceContext.h"




#define CB_DROPDOWNHEIGHT 100
#define MIN_CB_COL_WIDTH 50
#define MIN_EDIT_COL_WIDTH 50

#define IDC_RULECOMBOCELL 9001
#define IDC_STOCKCOMBOCELL 9002
#define IDC_INTEDITCELL 9003
#define IDC_FLOATEDITCELL 9004


BEGIN_MESSAGE_MAP(CCowPriceList,CBaseListCtrl)
	ON_WM_HSCROLL()
    ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK , OnDblClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw )
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO,GetDispInfo)

END_MESSAGE_MAP()


CCowPriceList::CCowPriceList(const GRIDHEADERS *ppHeaders,UINT pConfigItemId,CowPriceInvoiceSession *pInvoiceSession) : CBaseListCtrl(ppHeaders,pConfigItemId),m_pInvoiceSession(pInvoiceSession)
{
	m_mws_opts = CMarkWarnShowOptions::GetInstance();

	m_starImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,1,1);
	CBitmap termStar,question;
	termStar.LoadBitmap(IDB_TERMSTAR);
	m_starImageList.Add(&termStar,RGB(255,0,255));
	question.LoadBitmap(IDB_QUESTION);
	m_starImageList.Add(&question,RGB(255,0,255));



}
void CCowPriceList::SetListCols()
{
	CBaseListCtrl::SetListCols();
	SetImageList(&m_starImageList,LVSIL_SMALL);
}
void CCowPriceList::UpdateItemCount()
{
	SetItemCount(m_pInvoiceSession->GetCowCount());
}


void CCowPriceList::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	CowPriceData *cowPrice = m_pInvoiceSession->GetCowPrice(nItem);
	
	const char noData[] = "-";

	CString colText;
	Cow* pCow = cowPrice->GetCow();

	const InvoiceContext* pInvoiceContext = m_pInvoiceSession->GetInvoiceContext();
	Doc* otherDoc = pInvoiceContext->GetCowOtherDoc(pCow);
	
	switch(GetColID(nSubItem))
	{		
		case COL_HASTERM:
			colText.Empty();		
		break;
		case COL_EAN:
			colText = pCow->GetEAN();
		break;
		case COL_STOCK:
			colText = cowPrice->GetStock()->GetStockCode();
		break;
		case COL_WEIGHT:
			colText = cowPrice->GetWeight().ToString();
		break;
		case COL_MYPRICE:
			colText = pCow->GetMyPrice().ToString();
		break;
		case COL_HENT:
			if(otherDoc)
				colText = otherDoc->GetHent()->GetAlias();
			else
				colText = noData;
			break;
		case COL_OTHERPRICE:
			colText = pInvoiceContext->GetCowOtherPrice(pCow).ToString();
			break;
		case COL_PRICEPERKG:
			colText = cowPrice->GetPricePerKg().ToString();
		break;
		case COL_RULE:
		{
			Rule *rule = cowPrice->GetPriceRule();	
			if(rule)
				colText = rule->GetRuleName();
			else
				colText = noData;
		break;
		}
	

		case COL_NETTO:
			colText = cowPrice->GetCowPriceNetto().ToString();
		break;
		case COL_VAT:
			colText = cowPrice->GetVATValue().ToString();
		break;
		
		case COL_BRUTTO:
			colText = cowPrice->GetCowPriceBrutto().ToString();
		break;	

		default:
			ASSERT(0);

	}

	strcpy(pszItemText,(LPCSTR)colText);

}


BOOL CCowPriceList::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}
	
	
	if(nItem >= 0)
	{
		CowPriceData *cowPrice =  m_pInvoiceSession->GetCowPrice(nItem);
		switch(GetColID(nSubItem))
		{
			case COL_PRICEPERKG:
				FloatEditSubItem(nItem,nSubItem,cowPrice->GetPricePerKg().GetDouble());
			break;		
			case COL_NETTO:		
				FloatEditSubItem(nItem,nSubItem,cowPrice->GetCowPriceNetto().GetDouble());
			break;
			case COL_BRUTTO:
				FloatEditSubItem(nItem,nSubItem,cowPrice->GetCowPriceBrutto().GetDouble());
			break;

			case COL_RULE:
				if(!m_pInvoiceSession->GetRule4All() && cowPrice->GetPriceCustomInd() == CUSTOM_DISABLED &&
					!m_pInvoiceSession->IsUseCustomPricePerKgForAll() && !m_pInvoiceSession->IsAvgPriceEnabled() && !m_pInvoiceSession->IsUseStockMyPerKgPrice())
						RuleComboSubItem(nItem,nSubItem);
				//else
					
			break;

			case COL_WEIGHT:
				FloatEditSubItem(nItem,nSubItem,cowPrice->GetWeight().GetDouble());
			break;
			case COL_STOCK:
				StockComboSubItem(nItem,nSubItem);
			break;

			case COL_EAN:
			{	
				Cow *pCow = cowPrice->GetCow();
				CEditCowDlg editcow(pCow,this);
				editcow.LockEANEdit();
				if(editcow.DoModal() == IDOK)
				{
					if(pCow->CanEditThisCow())	
					{
						if(pCow->UpdateThisCow())
						{
							pCow->CommitChanges();							
						}
						else
						{
							pCow->RollbackChanges();
							SQLDB.SQLMessageBox(IDS_ERRUPDATEINGCOW);				
						}
						
						Invalidate(FALSE);
					}
					else 
					{
						AfxMessageBox(IDS_ERRCANTEDITCOW);						
					}
				}
				break;

			}
				

		}
	}

	*pResult = 0;
	return TRUE;
}



void CCowPriceList::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (GetFocus() != this) 
		SetFocus();
    
    CBaseListCtrl::OnHScroll (nSBCode, nPos, pScrollBar);
}

void CCowPriceList::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (GetFocus() != this) 
		SetFocus();
    CBaseListCtrl::OnVScroll (nSBCode, nPos, pScrollBar);
}

CComboBox *CCowPriceList::StockComboSubItem(int Item, int Column)
{
	CRect Rect;
	if (!GetCellRect(Item,Column,Rect,true,MIN_CB_COL_WIDTH)) 
    		return NULL;

		DWORD dwStyle =  WS_VSCROLL| WS_VISIBLE | CBS_DROPDOWNLIST |  WS_CHILD | WS_BORDER |  ES_AUTOHSCROLL | ES_AUTOVSCROLL |  CBS_DISABLENOSCROLL;

		CComboBox *pCombo = new gxStockComboCell(this, Item, Column,  m_pInvoiceSession->GetCowPrice(Item)->GetStock());

		Rect.bottom += CB_DROPDOWNHEIGHT;
		pCombo->Create(dwStyle, Rect,(CWnd*)this, IDC_STOCKCOMBOCELL);
		return pCombo;
}		



CComboBox *CCowPriceList::RuleComboSubItem(int Item, int Column)
{
	CRect Rect;
	if (!GetCellRect(Item,Column,Rect,true,MIN_CB_COL_WIDTH)) 
    		return NULL;

	DWORD dwStyle =  WS_VSCROLL |WS_VISIBLE | CBS_DROPDOWNLIST |  WS_CHILD | WS_BORDER |  ES_AUTOHSCROLL | ES_AUTOVSCROLL |  CBS_DISABLENOSCROLL;

		CComboBox *pCombo = new gxRuleComboCell(this, Item, Column, m_pInvoiceSession->GetCowPrice(Item)->GetPriceRule());

		Rect.bottom += CB_DROPDOWNHEIGHT;
		pCombo->Create(dwStyle, Rect,(CWnd*)this, IDC_RULECOMBOCELL);
		return pCombo;
}


CIntEditCell*  CCowPriceList::IntegerEditSubItem (int Item, int Column,int initValue)
{
		CRect Rect;
		if (!GetCellRect(Item,Column,Rect,true,MIN_EDIT_COL_WIDTH)) 
    		return NULL;

		CIntEditCell *pEdit = new CIntEditCell (this, Item, Column,initValue);
		pEdit->Create (Rect,(CWnd*)this, IDC_INTEDITCELL);
	
		return pEdit;

}


CFloatEditCell*  CCowPriceList::FloatEditSubItem (int Item, int Column,double initValue,int prec/* = 4*/)
{
		CRect Rect;
		if (!GetCellRect(Item,Column,Rect,true,MIN_EDIT_COL_WIDTH)) 
    		return NULL;

		CFloatEditCell *pEdit = new CFloatEditCell (this, Item, Column,initValue,prec);
		pEdit->Create (Rect,(CWnd*)this, IDC_FLOATEDITCELL);
	
		return pEdit;

}


bool CCowPriceList::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
        COLORREF crBkg = RGB(255,255,255);
		int Item = nItem;
		CowPriceData *cowPrice = m_pInvoiceSession->GetCowPrice(Item);
		int customInd = cowPrice->GetPriceCustomInd();
		Cow* pCow = cowPrice->GetCow();
				
		switch(GetColID(nSubItem))
		{

			case COL_OTHERPRICE:
			{
				Money profit = cowPrice->GetProfit();
				if(!profit.IsNull())
				{
					if(profit < ZEROMONEY)
					{
						crBkg = ERROR_CLR;
					}
					else
					{
						if(m_mws_opts->GetWarnToBigProfit() && profit > m_mws_opts->GetMaxProfit())
						{
							crBkg = m_mws_opts->GetWarnToBigProfitColor();							
						}
					}
				}
			}						
			break;
			case COL_EAN:
				if(cowPrice->IsError())
					crBkg = ERROR_CLR;
			break;
			case COL_RULE:
				if(m_pInvoiceSession->GetRule4All() || customInd != CUSTOM_DISABLED || 
					m_pInvoiceSession->IsUseCustomPricePerKgForAll() || m_pInvoiceSession->IsAvgPriceEnabled() || m_pInvoiceSession->IsUseStockMyPerKgPrice())
					crBkg = DISABLED_CLR ;
			break;
			case COL_PRICEPERKG:
				
				if(customInd == CUSTOM_NETTO || customInd == CUSTOM_BRUTTO )					
				{
					crBkg = CUSTOM_PRICE_CLR;
				}
				else if(customInd == CUSTOM_PRICEPERKG)
				{
					crBkg = CUSTOM_SET_CLR;
				} 
				else if(m_pInvoiceSession->IsAvgPriceEnabled())
				{
					crBkg = AVERAGE_PRICE_WEIGHT_CLR;
				}
				else if(m_pInvoiceSession->IsUseCustomPricePerKgForAll())
				{
					crBkg = CUSTOM_PRICE_PER_KG_CLR;
				}
				else if(m_pInvoiceSession->IsUseStockMyPerKgPrice())
				{
					crBkg = STOCK_PER_KG_PRICE_CLR;
				}
													
								
			break;

			case COL_NETTO:
				if(customInd == CUSTOM_NETTO)
				{
					crBkg = CUSTOM_SET_CLR;
					
				}
				else if(customInd == CUSTOM_PRICEPERKG || customInd == CUSTOM_BRUTTO)
				{
					crBkg = CUSTOM_PRICE_CLR;
				}
				else if(m_pInvoiceSession->IsAvgPriceEnabled())
				{
					crBkg = AVERAGE_PRICE_WEIGHT_CLR;
				}
			break;
			
			case COL_BRUTTO:
				if(customInd == CUSTOM_BRUTTO)
				{
					crBkg = CUSTOM_SET_CLR;
				}
				else if(customInd == CUSTOM_PRICEPERKG || customInd == CUSTOM_NETTO)
				{
					crBkg = CUSTOM_PRICE_CLR;
				} 
				else if(m_pInvoiceSession->IsAvgPriceEnabled())
				{
					crBkg = AVERAGE_PRICE_WEIGHT_CLR;
				}
			break;
			case COL_VAT:
				if(customInd != CUSTOM_DISABLED)
					crBkg = CUSTOM_PRICE_CLR;
				else if(m_pInvoiceSession->IsAvgPriceEnabled())
					crBkg = AVERAGE_PRICE_WEIGHT_CLR;
			break;

			case COL_WEIGHT:
				
				if(cowPrice->IsCustomWeight())
				{
					crBkg = CUSTOM_SET_CLR;
				}
				else if(m_pInvoiceSession->IsAvgWeightEnabled())
				{
					crBkg = AVERAGE_PRICE_WEIGHT_CLR;
				}

					
			break;
			
			case COL_STOCK:
				if(cowPrice->IsCustomStock()) crBkg = CUSTOM_SET_CLR;
				
			break;					
			

			/*case COL_MYPRICE:

				if(CowPriceData::buy_sell == SELL)
				{
					UINT gtlt = m_mws_opts->GetMarkMyPriceGtLtSell();
					if(gtlt != GTLT_NONE)
					{
						switch(gtlt)
						{
							case IT_IS_GT:
								if(pCow->GetMyPrice() > cowPrice->GetCowPriceNetto())
									crBkg = ERROR_CLR;
							break;
							case IT_IS_LT:
								if(pCow->GetMyPrice() < cowPrice->GetCowPriceNetto())
									crBkg = ERROR_CLR;
							break;
						}
					}
				}
				else
				{
					UINT gtlt = m_mws_opts->GetMarkMyPriceGtLtBuy();
					if(gtlt != GTLT_NONE)
					{
						switch(gtlt)
						{
						case IT_IS_GT:
							if(pCow->GetMyPrice() > cowPrice->GetCowPriceNetto())
								crBkg = ERROR_CLR;
						break;
						case IT_IS_LT:
							if(pCow->GetMyPrice() < cowPrice->GetCowPriceNetto())
								crBkg = ERROR_CLR;
						break;
						}
					}
				}
			break;*/
		}

    *cellBkg = crBkg;
	return true;       
}


void CCowPriceList::SortByCol(int Column,bool bAsc)
{
	switch(m_pInvoiceSession->GetInvoiceContext()->GetBuyOrSell())
	{
		case BUY:				
			m_pInvoiceSession->SortPrices(BuyCowPriceDataComparator(GetColID(Column),bAsc));
		break;
		case SELL:
			m_pInvoiceSession->SortPrices(SellCowPriceDataComparator(GetColID(Column),bAsc));				
		break;
	}
    
	Invalidate();
}


int CCowPriceList::GetItemImage(int iItem,int iSubItem)
{
	if(GetColID(iSubItem) == COL_HASTERM)
	{
		return m_pInvoiceSession->GetInvoiceContext()->HasAnyTermValue(m_pInvoiceSession->GetCowPrice(iItem)->GetCow()) ? 0 : 1;
	}
	return -1;
}

CString CCowPriceList::GetColText(UINT nColID)
{
	UINT nResText = IDS_HEADEREMPTY;
	if(nColID == COL_OTHERPRICE)
	{
		switch(m_pInvoiceSession->GetInvoiceContext()->GetBuyOrSell())
		{
			case BUY:				
				nResText = IDS_HEADERSOLDFOR;
			break;
			case SELL:
				nResText = IDS_HEADERBOUGHTFOR;				
			break;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return CTempStringLoader(nResText);
}
