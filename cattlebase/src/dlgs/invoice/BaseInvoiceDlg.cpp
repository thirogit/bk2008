#include "stdafx.h"
#include "BaseInvoiceDlg.h"
#include "../../missing/util.h"
#include "../NewRuleDlg.h"
#include "../EditRuleDlg.h"
#include "mainfrm_ctrl.h"
#include "MainCfg.h"
#include "CattleSQL.h"
#include "../../PassportViewing\PassImgViewDlg.h"
#include "../../consts.h"
#include "../../datalimits.h"
#include "../../global_arrays.h"
#include "../../classes/TempStringLoader.h"
#include "../../Utils/StringFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBaseInvoiceDlg::CBaseInvoiceDlg(UINT nIdDlgTemplate,CowPriceInvoiceSession* pInvoiceSession,InvoiceUISettings* pUISettings,CWnd* pParent /*=NULL*/) :
	CDialog(nIdDlgTemplate, pParent),
	//m_Rule4All(FALSE),
	//m_useMyPerKgPrice(FALSE),	
	//m_bCustomPricePerKg(FALSE),
	m_totalnetto(g_szRegExUnsigned),	
	//m_CustomPricePerKg(g_szRegExUnsigned),	
	m_totalbrutto(g_szRegExUnsigned),	
	m_pInvoiceSession(pInvoiceSession),
	m_invoicedate(true),
	m_pUISettings(pUISettings),
	m_pricelist(m_pUISettings->GetListHeaderColumns(),m_pUISettings->GetListCfgItem(),pInvoiceSession),
	//m_stockpricelist(pInvoiceSession),	
	m_docList(&pInvoiceSession->GetInvoiceDocs())//,
	//m_statList(pInvoiceSession),	
	//m_RuleList(pInvoiceSession->GetInvoiceContext())
{
	/*switch(pInvoiceSession->GetInvoiceContext()->GetBuyOrSell())
	{
		case BUY:
			m_termPriceLabel = IDS_TERMBUYPRICE;
			m_termStockLabel = IDS_TERMBUYSTOCK;
			m_termWeightLabel = IDS_TERMBUYWEIGHT;
		break;
		case SELL:
			m_termPriceLabel = IDS_TERMSELLPRICE;
			m_termStockLabel = IDS_TERMSELLSTOCK;
			m_termWeightLabel = IDS_TERMSELLWEIGHT;
			
		break;
	}*/

	m_bSetTotalNetto = FALSE;
	m_bSetTotalBrutto = FALSE;
}


CBaseInvoiceDlg::~CBaseInvoiceDlg()
{
	
}

void CBaseInvoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX,IDC_RULELIST,m_RuleList);
	//DDX_Check(pDX,IDC_RULE4ALL,m_Rule4All);
	DDX_Control(pDX,IDC_VAT,m_vat);
	DDX_Control(pDX,IDC_PAYWAY,m_payway);
	DDX_Control(pDX,IDC_EXTRAS,m_extras);
	DDX_Control(pDX,IDC_INVOICEDATE,m_invoicedate);
	DDX_Control(pDX,IDC_TOTALNETTO,m_totalnetto);
	DDX_Control(pDX,IDC_SETTOTALNETTO,m_settotalnetto);

	DDX_Control(pDX,IDC_TOTALBRUTTO,m_totalbrutto);
	DDX_Control(pDX,IDC_SETTOTALBRUTTO,m_settotalbrutto);
	
	DDX_Control(pDX,IDOK,m_btnaccept);
	DDX_Control(pDX,IDCANCEL,m_btncancel);
	DDX_Text(pDX,IDC_CUSTOMNUMBER,m_customNumber);

	//DDX_Check(pDX,IDC_CHECKPRICEPERKG,m_bCustomPricePerKg);
	//DDX_Control(pDX,IDC_CUSTOMPRICEPERKG,m_CustomPricePerKg);
	DDX_Check(pDX,IDC_SETTOTALNETTO,m_bSetTotalNetto);
	DDX_Check(pDX,IDC_SETTOTALBRUTTO,m_bSetTotalBrutto);
	
	DDX_Control(pDX,IDC_NAVTAB,m_naviTab);
	//DDX_Check(pDX,IDC_USEMYPERKGPRICE,m_useMyPerKgPrice);
	DDX_Control(pDX,IDC_ADDITEM,m_btnadditem);
	DDX_Control(pDX,IDC_DELITEM,m_btndelitem);
	DDX_Control(pDX,IDC_PAYDUEDAYSSPIN,m_spnPayDueDays);

	DDX_Text(pDX,IDC_TRANSACTIONPLACE,m_sTransactionPlace);
	DDX_Control(pDX,IDC_TRANSACTIONDATE,m_TransactionDate);

}

BOOL CBaseInvoiceDlg::OnInitDialog()
{		
	CDialog::OnInitDialog();

	m_btncancel.SetButtonImage(IDB_CANCEL);
	m_btnaccept.SetButtonImage(IDB_CASH);
	m_btnadditem.SetButtonImage(IDB_REDPLUS);
	m_btndelitem.SetButtonImage(IDB_BLUEMINUS);
	
	m_pInvoiceSession->SetRule4All(NULL);
	m_pInvoiceSession->EnableAvgPrice(false);
	m_pInvoiceSession->SetUseCustomPricePerKgForAll(false);

	m_pInvoiceSession->SetUseStockMyPerKgPrice(false);
	
	m_settotalnetto.SetTitleStyle(BS_AUTOCHECKBOX);
	m_settotalnetto.SetCheck(FALSE);

	m_settotalbrutto.SetTitleStyle(BS_AUTOCHECKBOX);
	m_settotalbrutto.SetCheck(FALSE);
	
	m_vat.SetVATRate(m_pInvoiceSession->GetVATRate());

	m_payway.SetHent(m_pInvoiceSession->GetInvoiceHent());
	m_payway.ReloadCombo();

	m_totalbrutto.SendMessage(EM_SETREADONLY,TRUE);
	m_totalnetto.SendMessage(EM_SETREADONLY,TRUE);

	m_priceMenu.LoadMenu(IDR_PRICELISTMENU);

	SendDlgItemMessage(IDC_CUSTOMNUMBER,EM_LIMITTEXT,MAXCUSTOMNUMBER);

	CreateNaviTab();	
	
//	OnCustomPricePerKg();

//	SetDlgItemResText(IDC_TERMWEIGHT_LABEL,m_termWeightLabel);
//	SetDlgItemResText(IDC_TERMPRICE_LABEL,m_termPriceLabel);
//	SetDlgItemResText(IDC_TERMSTOCK_LABEL,m_termStockLabel);

//	SetTermValue(IDC_TERMPRICE_VALUE,szMissing,IDC_SETTERMPRICEBTN,FALSE);
//	SetTermValue(IDC_TERMWEIGHT_VALUE,szMissing,IDC_SETTERMWEIGHTBTN,FALSE);
//	SetTermValue(IDC_TERMSTOCK_VALUE,szMissing,IDC_SETTERMSTOCKBTN,FALSE);

	SetWindowText((LPCSTR)GetInvDlgCaption());	

	m_spnPayDueDays.SetRange32(0,MAX_PAYDUEDAYS);
	m_spnPayDueDays.SetPos(0);

	m_lastInvTip.SetColors(RGB(255,255,0),RGB(255,255,128));
	m_lastInvTip.Create(this);
	
	InitInvoiceDialog();

	UpdateTotal();			
	UpdateItemCount();

	TabChanged(GetCurrentTabIndex());

	m_docList.UpdateItemCount();
	
	return TRUE;
}

void CBaseInvoiceDlg::InitInvoiceDialog()
{
	SetInvoiceControls();
	UpdateData(FALSE);
}

void CBaseInvoiceDlg::CreateNaviTab()
{
		

	AddNaviTab(0,m_pricelist,IDC_COWLIST,IDS_INVNAVITABCOWPRICES,true);
	
//	AddNaviTab(1,m_stockpricelist,tabItemRect,IDC_STOCKLIST,IDS_INVNAVITABSTOCKPRICE,false);

	AddNaviTab(1,m_docList,IDC_DOCLIST,IDS_INVNAVITABDOCS,false);

//	AddNaviTab(3,m_statList,tabItemRect,IDC_STOCKSTATLIST,IDS_INVNAVITABSTAT,false);

	SetCurTab(0);

}

CRect CBaseInvoiceDlg::GetTabRect()
{
	CRect tabItemRect;
	m_naviTab.GetClientRect(&tabItemRect);
	tabItemRect.top += 28;
	tabItemRect.bottom -= 4;
	tabItemRect.left += 4;
	tabItemRect.right -= 8;
	return tabItemRect;
}

void CBaseInvoiceDlg::SetCurTab(int tabIndex)
{
	m_naviTab.SetCurSel(tabIndex);
}


void CBaseInvoiceDlg::AddNaviTab(int insertPos,CBaseListCtrl &tab,UINT tabID,UINT tabTextResID,bool bShow)
{
	CRect rect = GetTabRect();
	TCITEM item;
	CString tabText;
	item.mask = TCIF_TEXT|TCIF_PARAM;
	tab.Create(LVS_REPORT | LVS_OWNERDATA | WS_BORDER | WS_TABSTOP,rect,&m_naviTab,tabID);
	tabText.LoadString(tabTextResID);
	item.lParam = (LPARAM) &tab;
	item.pszText = tabText.GetBuffer();
	m_naviTab.InsertItem(insertPos, &item);
	tabText.ReleaseBuffer();
	tab.SetWindowPos(NULL, 0,0, 0, 0,SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);	
	tab.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}



BEGIN_MESSAGE_MAP(CBaseInvoiceDlg, CDialog)
	//ON_BN_CLICKED(IDC_RULE4ALL,OnRule4All)
	//ON_LBN_SELCHANGE(IDC_RULELIST,OnRuleChange)
	ON_BN_CLICKED(IDC_SETTOTALNETTO,OnCheckTotalNetto)
	ON_BN_CLICKED(IDC_SETTOTALBRUTTO,OnCheckTotalBrutto)
	//ON_BN_CLICKED(IDC_RULEEDIT,OnRuleEdit)
	ON_NOTIFY(LVN_ENDLABELEDIT,IDC_COWLIST, OnEndLabelEditCowPrice)
	//ON_NOTIFY(LVN_ENDLABELEDIT,IDC_STOCKLIST, OnEndLabelEditStockPrice)
	//ON_BN_CLICKED(IDC_ADDRULE,OnAddRule)
	//ON_BN_CLICKED(IDC_CHECKPRICEPERKG,OnCustomPricePerKg)
	
	ON_CBN_SELCHANGE(IDC_VAT,OnVATChange)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_TOTALNETTO,OnEditComplete)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_TOTALBRUTTO,OnEditComplete)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_VAT,OnEditComplete)
	//ON_NOTIFY(EN_EDITCOMPLETE,IDC_CUSTOMPRICEPERKG,OnEditComplete)

	ON_NOTIFY(NM_RCLICK, IDC_COWLIST, OnRClick)

	ON_COMMAND_RANGE(IDM_PRICELIST_MIN,IDM_PRICELIST_MAX,MenuHandler)
	
	ON_NOTIFY(TCN_SELCHANGE,IDC_NAVTAB, OnSelChange)
	ON_NOTIFY(TCN_SELCHANGING,IDC_NAVTAB, OnSelChanging)
	//ON_BN_CLICKED(IDC_USEMYPERKGPRICE,OnUseMyPerKgPrice)
	ON_BN_CLICKED(IDC_ADDITEM,OnAddItem)
	ON_BN_CLICKED(IDC_DELITEM,OnDelItem)


	//ON_BN_CLICKED(IDC_SETTERMWEIGHTBTN,OnSetTermWeight)
	//ON_BN_CLICKED(IDC_SETTERMSTOCKBTN,OnSetTermStock)
	//ON_BN_CLICKED(IDC_SETTERMPRICEBTN,OnSetTermPrice)

	//ON_BN_CLICKED(IDC_SETTERMWEIGHTALL,OnSetTermWeightAll)
	//ON_BN_CLICKED(IDC_SETTERMSTOCKALL,OnSetTermStockAll)
	ON_BN_CLICKED(IDC_SETTRANSACTIONPLACE,OnSetTransactionPlace)


	ON_NOTIFY(LVN_ITEMCHANGED,IDC_COWLIST,OnCowPriceListItemChanged)
	ON_NOTIFY(LVN_ITEMCHANGED,IDC_DOCLIST,OnDocListItemChanged)

	ON_EN_SETFOCUS(IDC_CUSTOMNUMBER,OnCustomNumberFocus)
	ON_EN_KILLFOCUS(IDC_CUSTOMNUMBER,OnCustomNumberLostFocus)

END_MESSAGE_MAP()

//
//void CBaseInvoiceDlg::OnRule4All()
//{
//	UpdateData();
//	if(!m_Rule4All)
//	{
//		m_pInvoiceSession->SetRule4All(NULL);
//		UpdateCowPriceList();
//		UpdateTotal();
//	}
//	else
//		OnRuleChange();
//}

//void CBaseInvoiceDlg::OnRuleChange()
//{
//	if(m_Rule4All)
//	{	
//		int nSel = m_RuleList.GetCurSel();
//		if(nSel >= 0)
//		{
//			Rule *rule  = (Rule*)m_RuleList.GetItemData(nSel);
//			m_pInvoiceSession->SetRule4All(rule);
//			UpdateCowPriceList();
//			UpdateTotal();
//		}
//	}
//}


void CBaseInvoiceDlg::TotalNettoChecked()
{

}

void CBaseInvoiceDlg::TotalBruttoChecked()
{

}

void CBaseInvoiceDlg::OnCheckTotalNetto()
{
	UpdateData();
	bool not_m_settotal = !m_bSetTotalNetto;

	if(not_m_settotal)
	{
		m_pInvoiceSession->EnableAvgPrice(false);
		UpdateCowPriceList();
		UpdateTotal();
	}
	
	m_totalnetto.SendMessage(EM_SETREADONLY,not_m_settotal);

//	m_RuleList.EnableWindow(not_m_settotal && m_Rule4All);
//	GetDlgItem(IDC_RULE4ALL)->EnableWindow(not_m_settotal);
//	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!not_m_settotal || !m_bCustomPricePerKg);
//	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(not_m_settotal);
//	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(not_m_settotal);
	m_settotalbrutto.EnableWindow(not_m_settotal);
	TotalNettoChecked();

}

void CBaseInvoiceDlg::OnCheckTotalBrutto()
{
	UpdateData();
	bool not_m_settotal = !m_bSetTotalBrutto;

	if(not_m_settotal)
	{
		m_pInvoiceSession->EnableAvgPrice(false);
		UpdateCowPriceList();
		UpdateTotal();
	}
	m_totalbrutto.SendMessage(EM_SETREADONLY,not_m_settotal);

//	m_RuleList.EnableWindow(not_m_settotal && m_Rule4All);
//	GetDlgItem(IDC_RULE4ALL)->EnableWindow(not_m_settotal);
//	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!not_m_settotal || !m_bCustomPricePerKg);
//	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(not_m_settotal);
//	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(not_m_settotal);
	m_settotalnetto.EnableWindow(not_m_settotal);
	TotalBruttoChecked();
}


void CBaseInvoiceDlg::UpdateCowPriceList()
{
	m_pricelist.Invalidate(FALSE);
}

void CBaseInvoiceDlg::UpdateTotal()
{	
	PutTotalNetto();
	PutTotalBrutto();
	PutTotalVAT();
}

void CBaseInvoiceDlg::OnEndLabelEditCowPrice (NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
    LV_ITEM	*plvItem = &plvDispInfo->item;
	CString floatStr;
	CowPriceData *cowPrice = NULL;
	
	if (plvItem->pszText != NULL)
    {
		cowPrice = m_pInvoiceSession->GetCowPrice(plvItem->iItem);
		switch(m_pricelist.GetColID(plvItem->iSubItem))
		{
			case CCowPriceList::COL_NETTO:
				cowPrice->SetCustomPriceNetto(Money(CUtil::myatof(plvItem->pszText))); //when empty string atof returns 0.0
				if(!strlen(plvItem->pszText)) cowPrice->DisableCustomPrice();
							
			break;
			case CCowPriceList::COL_BRUTTO:
				
				cowPrice->SetCustomPriceBrutto(Money(CUtil::myatof(plvItem->pszText))); //when empty string atof returns 0.0
				if(!strlen(plvItem->pszText)) cowPrice->DisableCustomPrice();
								 
				
			break;
			case CCowPriceList::COL_RULE:
				cowPrice->SetRule((Rule*)plvItem->lParam);				
			break;
			case CCowPriceList::COL_PRICEPERKG:
				cowPrice->SetCustomPricePerKg(Money(CUtil::myatof(plvItem->pszText))); //when empty string atof returns 0.0
				if(!strlen(plvItem->pszText)) cowPrice->DisableCustomPrice();
				
			break;
			case CCowPriceList::COL_WEIGHT:
				cowPrice->SetCustomWeight(NullDouble(CUtil::myatof(plvItem->pszText)));
			break;

			case CCowPriceList::COL_STOCK:
				cowPrice->SetCustomStock( (Stock*)plvItem->lParam);
			break;
		}
		UpdateTotal();	
		UpdateCowPriceList();
	}
	*pResult = 0;
}
//
//void CBaseInvoiceDlg::OnEndLabelEditStockPrice (NMHDR* pNMHDR, LRESULT* pResult) 
//{
//    LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
//    LV_ITEM	*plvItem = &plvDispInfo->item;
//	Stock *pStock = NULL;
//	
//    if (plvItem->pszText != NULL)
//    {		
//		pStock = m_stockpricelist.GetStockItem(plvItem->iItem);		
//		switch(m_stockpricelist.GetColID(plvItem->iSubItem))
//		{
//			case CStockPricePerKgList::STOCKPERKGPRICE_COL2:
//			{
//				pStock->ChangeMyPerKgPrice(CUtil::myatof(plvItem->pszText));
//				if(pStock->UpdateThisStock())
//				{
//					pStock->CommitChanges();
//				}
//				else
//				{
//					pStock->RollbackChanges();
//					SQLDB.SQLMessageBox(IDS_ERRUPDATINGSTOCK);
//				}
//				UpdateCowPriceList();
//				UpdateTotal();				
//			}
//			break;
//		}
//		m_stockpricelist.Invalidate(FALSE);
//	}
//	*pResult = 0;
//}

void CBaseInvoiceDlg::OnEditComplete(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString s;
	
	switch(pNMHDR->idFrom)
	{
		case IDC_TOTALNETTO:
			if(m_bSetTotalNetto)
			{
				
				m_totalnetto.GetWindowText(s);
				SetTotalNetto(Money(CUtil::myatof(s)));
				UpdateCowPriceList();
			
			}
		break;

		case IDC_TOTALBRUTTO:
			if(m_bSetTotalBrutto)
			{
				
				m_totalbrutto.GetWindowText(s);
				SetTotalBrutto(Money(CUtil::myatof(s)));
				UpdateCowPriceList();
			
			}
		

		break;
		/*case IDC_CUSTOMPRICEPERKG:
				m_CustomPricePerKg.GetWindowText(s);
				m_pInvoiceSession->SetCustomPricePreKgForAll(CUtil::myatof(s));
				m_pInvoiceSession->SetUseCustomPricePerKgForAll(true);
				UpdateCowPriceList();
				UpdateTotal();
		break;*/
		case IDC_VAT:
				OnVATChange();	
		break;
		
	}

	*pResult = (LRESULT)0;
}
//
//void CBaseInvoiceDlg::OnAddRule()
//{
//	if(!HAS_PRIVLEDGE(ADDMODOWN))
//	{
//		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
//		return;
//	}
//
//	Rule *newRule = new Rule(NULL_RULEID);
//	CNewRuleDlg newRuleDlg(newRule,this);
//	if(newRuleDlg.DoModal() == IDOK)
//	{
//		if(newRule->InsertThisRule())
//		{
//			ary_rules.Add(newRule);
//		}
//		else
//		{
//			SQLDB.SQLMessageBox(IDS_ERRUPDATINGRULE);
//			delete newRule;
//		}
//		m_RuleList.Reload();
//	}
//	else 
//		delete newRule;
//
//}


//
//void CBaseInvoiceDlg::OnCustomPricePerKg()
//{
//	UpdateData();
//	if(!m_bCustomPricePerKg) 
//	{
//		m_pInvoiceSession->SetCustomPricePreKgForAll(false);
//		UpdateCowPriceList();
//		UpdateTotal();
//	}
//	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!m_bCustomPricePerKg);
//	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(!m_bCustomPricePerKg);
//}

void CBaseInvoiceDlg::OnVATChange()
{	
	VATRate VAT = m_vat.GetVATRate();
	if(VAT.IsNull())
	{
		m_vat.SetVATRate(m_pInvoiceSession->GetVATRate());
	}
	else
	{
		m_pInvoiceSession->SetVATRate(VAT);
		UpdateCowPriceList();
		UpdateTotal();
	}

}


bool CBaseInvoiceDlg::Validate()
{
	if(!HAS_PRIVLEDGE(INVOICE))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return false;
	}

	if(m_payway.GetCurSel() < 0)
	{
		AfxMessageBox(IDS_ERRNOPAYWAY,MB_OK,0);
		m_payway.SetFocus();
		return false;
	}

	if(m_invoicedate.IsInputEmpty())
	{
		AfxMessageBox(IDS_ERRNOINVDATE,MB_OK,0);
		m_invoicedate.SetFocus();
		return false;
	}
	else
	{
		if(!m_invoicedate.IsDateValid())
		{
			AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
			m_invoicedate.SetFocus();
			return false;
		}
	}

	if(m_vat.GetVATRate().IsNull())
	{
		AfxMessageBox(IDS_ERRNOVATRATE,MB_OK,0);
		m_vat.SetFocus();
		return false;
	}

	if(!m_TransactionDate.IsInputEmpty())
	{
		if(!m_TransactionDate.IsDateValid())
		{
			AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
			m_TransactionDate.SetFocus();
			return false;
		}
	}

	bool gotError = false;

	for(int i = 0;i< m_pInvoiceSession->GetCowCount();i++)
	{		
		if(m_pInvoiceSession->GetCowPrice(i)->IsError())
		{	
			gotError = true;
		}
	}

	if(gotError)
	{
		AfxMessageBox(IDS_ERRINVALIDINVENTRIES,MB_OK,0);
		return false;
	}
	return true;
}

void CBaseInvoiceDlg::OnOK()
{
	UpdateData();
	UpdateCowPriceList();
	UpdateTotal();

	if(!Validate())
		return;
		
	if(AfxMessageBox(IDS_ASKINSERTINTODB,MB_YESNO,0) != IDYES)
		return;

	if(CommitInvoice())
		EndDialog(IDOK);
}


void CBaseInvoiceDlg::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMIA)
	{
	
		if( pNMIA->iItem >= 0)
		{
		
			CMenu* pPopupMenu = m_priceMenu.GetSubMenu(0);
			ASSERT(pPopupMenu); 
	
			if (m_pInvoiceSession->GetCowPrice(pNMIA->iItem)->GetPriceCustomInd() != CUSTOM_DISABLED)
				pPopupMenu->EnableMenuItem(IDM_PRICELIST_DELCUSTPRICE, MF_BYCOMMAND | MF_ENABLED);
			else
                pPopupMenu->EnableMenuItem(IDM_PRICELIST_DELCUSTPRICE, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED); 
	

			// Get the cursor position
			CPoint CursorPoint = (0, 0);
			GetCursorPos(&CursorPoint);
			// Track the popup menu
			pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, CursorPoint.x, 
									CursorPoint.y, this);

		}
		
	}


	*pResult = 0;
}

void CBaseInvoiceDlg::DeleteCustomPrice(CowPriceData *pCowPrice)
{
	pCowPrice->DisableCustomPrice();
	UpdateCowPriceList();
	UpdateTotal();
}
void CBaseInvoiceDlg::DeleteCustomStock(CowPriceData *pCowPrice)
{
	pCowPrice->SetCustomStock(NULL);
	UpdateCowPriceList();
	UpdateTotal();
}
void CBaseInvoiceDlg::DeleteCustomWeight(CowPriceData *pCowPrice)
{
	pCowPrice->SetCustomWeight(NullDouble());
	UpdateCowPriceList();
	UpdateTotal();
}

void CBaseInvoiceDlg::DeleteForOneRow(void (CBaseInvoiceDlg::*pDeleteAction)(CowPriceData*))
{
	CowPriceData *pPrice;
	POSITION pos;
	pos = m_pricelist.GetFirstSelectedItemPosition();
	if(pos)
	{	
		pPrice = m_pInvoiceSession->GetCowPrice(m_pricelist.GetNextSelectedItem(pos));
		(this->*pDeleteAction)(pPrice);	
	}
}

void CBaseInvoiceDlg::DeleteForAllRows(void (CBaseInvoiceDlg::*pDeleteAction)(CowPriceData*))
{
	CowPriceData *pPrice;
	for(int i = 0,s = m_pInvoiceSession->GetCowCount();i<s;i++)
	{
		pPrice = m_pInvoiceSession->GetCowPrice(i);
		(this->*pDeleteAction)(pPrice);
	}	
}


void CBaseInvoiceDlg::MenuHandler(UINT nId)
{	

	switch(nId)
	{
		case IDM_PRICELIST_DELCUSTPRICEALL:
			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomPrice);	
		break;		
		case IDM_PRICELIST_DELCUSTPRICE:
			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomPrice);		
		break;		
		case IDM_PRICELIST_DELCUSTSTOCKALL:
			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomStock);		
		break;		
		case IDM_PRICELIST_DELCUSTSTOCK:
			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomStock);		
		break;
		case IDM_PRICELIST_DELCUSTWEIGHT:
			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomWeight);		
		break;
		case IDM_PRICELIST_DELCUSTWEIGHTALL:
			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomWeight);		
		break;	
		case IDM_PRICELIST_SHOWPASSIMG:
		{
			CCowArray cows;
			for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
				cows.Add(m_pInvoiceSession->GetCowPrice(i)->GetCow());				
			int j = m_pricelist.GetCurSel();
			CPassImgViewDlg passView(cows,j);
			passView.DoModal();

		break;
		}

		case IDM_PRICELIST_DELALLCUSTVALUES:
			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomWeight);
			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomPrice);
			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomStock);
		break;
		case IDM_PRICELIST_DELALLCUSTVALUESALL:
			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomWeight);
			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomPrice);
			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomStock);
		break;

	}
	
}

void CBaseInvoiceDlg::SetTotalNetto(const Money& totalnetto)
{
	CString s;
	CCowPriceDataArray woCustomPrice;
	Money totalCustPrice(ZEROMONEY);
	CowPriceData *cowprice;
	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
	{
		cowprice =  m_pInvoiceSession->GetCowPrice(i);
		if(cowprice->GetPriceCustomInd() != CUSTOM_DISABLED) totalCustPrice += cowprice->GetCowPriceNetto();
		else woCustomPrice.Add(cowprice);
	}
	

	if(woCustomPrice.GetSize() == 0)
	{
		AfxMessageBox(IDS_ERRSETTOTALPRICE,MB_OK,0);
		PutTotalNetto();
		m_bSetTotalNetto = FALSE;
		UpdateData(FALSE);
		OnCheckTotalNetto();
		
		return;
	}
	if(totalnetto <= totalCustPrice) 
	{
		
		AfxMessageBox(IDS_TOTALTOOSMALL);
		m_bSetTotalNetto = FALSE;
		UpdateData(FALSE);
		OnCheckTotalNetto();
		return;
		
	}

	m_pInvoiceSession->SetAvgPrice((totalnetto-totalCustPrice)/woCustomPrice.GetSize());
	m_pInvoiceSession->EnableAvgPrice(true);
	
	UpdateTotal();
	
}

void CBaseInvoiceDlg::SetTotalBrutto(const Money& totalbrutto)
{

	CString s;
	CCowPriceDataArray woCustomPrice;
	Money totalCustPriceBrutto(ZEROMONEY),totalCustPrice;
	CowPriceData *cowprice;
	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
	{
		cowprice = m_pInvoiceSession->GetCowPrice(i);
		if(cowprice->GetPriceCustomInd() != CUSTOM_DISABLED) 
			totalCustPriceBrutto += cowprice->GetCowPriceBrutto();
		else woCustomPrice.Add(cowprice);
	}


	if(woCustomPrice.GetSize() == 0)
	{
		AfxMessageBox(IDS_ERRSETTOTALPRICE,MB_OK,0);
		PutTotalBrutto();
		m_bSetTotalBrutto = FALSE;
		UpdateData(FALSE);
		OnCheckTotalBrutto();
		
		
		return;
	}
	double vat_prim =(1+m_pInvoiceSession->GetVATRate().GetDouble());
	Money totalnetto(totalbrutto/vat_prim),vattotal(m_pInvoiceSession->GetVATRate()*totalnetto);

	
	if(totalbrutto <= totalCustPriceBrutto) 
	{
		
		AfxMessageBox(IDS_TOTALTOOSMALL);
		m_bSetTotalBrutto = FALSE;
		UpdateData(FALSE);
		OnCheckTotalBrutto();

		return;
		
	}

	totalCustPrice = totalCustPriceBrutto*vat_prim;

	m_pInvoiceSession->SetAvgPrice((totalnetto-totalCustPrice)/woCustomPrice.GetSize());
	m_pInvoiceSession->EnableAvgPrice(true);
	UpdateTotal();

}


void CBaseInvoiceDlg::PutTotalNetto()
{
	m_totalnetto.SetCommitedText(m_pInvoiceSession->GetTotalNetValue().ToString());
}

void CBaseInvoiceDlg::PutTotalBrutto()
{
	m_totalbrutto.SetCommitedText(m_pInvoiceSession->GetTotalGrossValue().ToString());
}

void CBaseInvoiceDlg::PutTotalVAT()
{
	SetDlgItemText(IDC_TOTALVAT,m_pInvoiceSession->GetTotalVATValue().ToString());
}

//void CBaseInvoiceDlg::OnRuleEdit()
//{
//	int nSel = m_RuleList.GetCurSel();
//	if(nSel >= 0)
//	{
//		Rule *pRule  = (Rule*)m_RuleList.GetItemData(nSel);
//		
//		CEditRuleDlg editRuleDlg(pRule,this);
//		if(editRuleDlg.DoModal() == IDOK)
//		{
//			if(!pRule->UpdateThisRule())
//			{
//				pRule->RollbackChanges();
//				SQLDB.SQLMessageBox(IDS_ERRUPDATINGRULE);
//			}
//			else
//			{
//				pRule->CommitChanges();
//				UpdateCowPriceList();
//				UpdateTotal();
//			}
//		}
//	}
//	
//}


void CBaseInvoiceDlg::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
		int iNewTab = m_naviTab.GetCurSel();

		TCITEM item;
		CWnd* pWnd;

		item.mask = TCIF_PARAM;
		

		
		//** hide the current tab ---------
		m_naviTab.GetItem(m_prevSelectedTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_HIDE);

		//** show the selected tab --------
		m_naviTab.GetItem(iNewTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_SHOW);

		OnTabChanged(m_prevSelectedTab,iNewTab);

	*pResult = 0;
}

void CBaseInvoiceDlg::OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_prevSelectedTab = m_naviTab.GetCurSel();

	*pResult = 0;
}
int CBaseInvoiceDlg::GetCurrentTabIndex()
{
	return m_naviTab.GetCurSel();
}

void CBaseInvoiceDlg::OnTabChanged(int oldTabIndex,int newTabIndex)
{
	TabChanged(newTabIndex);
}


//void CBaseInvoiceDlg::OnUseMyPerKgPrice()
//{
//	UpdateData();
//	m_pInvoiceSession->SetUseStockMyPerKgPrice(m_useMyPerKgPrice == TRUE);
//	UpdateCowPriceList();
//	UpdateTotal();	
//	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!m_useMyPerKgPrice);
//	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(!m_useMyPerKgPrice);
//}

void CBaseInvoiceDlg::OnAddItem()
{
	AddItem();
}

void CBaseInvoiceDlg::UpdateItemCount()
{
	SetDlgItemText(IDC_COUNT,(LPCSTR)CStringFormat((LPCSTR)CTempStringLoader(IDS_COWCOUNT),m_pInvoiceSession->GetCowCount()));
	m_pricelist.UpdateItemCount();
}

void CBaseInvoiceDlg::OnDelItem()
{	
	DelItem();
}
//
//void CBaseInvoiceDlg::OnSetTermWeight()
//{
//	int iSel = m_pricelist.GetCurSel();
//	if(iSel >= 0)
//	{
//		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
//		pCowPrice->SetCustomWeight(m_pInvoiceSession->GetInvoiceContext()->GetTermWeight(pCowPrice->GetCow()).ToInt());
//		UpdateCowPriceList();
//		UpdateTotal();
//	}
//}
//void CBaseInvoiceDlg::OnSetTermStock()
//{
//	int iSel = m_pricelist.GetCurSel();
//	if(iSel >= 0)
//	{
//		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
//		pCowPrice->SetCustomStock(m_pInvoiceSession->GetInvoiceContext()->GetTermStock(pCowPrice->GetCow()));
//		UpdateCowPriceList();
//		UpdateTotal();
//	}
//	
//}
//void CBaseInvoiceDlg::OnSetTermPrice()
//{
//
//	int iSel = m_pricelist.GetCurSel();
//	if(iSel >= 0)
//	{
//		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
//		pCowPrice->SetCustomPriceNetto(m_pInvoiceSession->GetInvoiceContext()->GetTermPrice(pCowPrice->GetCow()));
//		UpdateCowPriceList();
//		UpdateTotal();
//	}
//	
//}

void CBaseInvoiceDlg::OnDocListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if ((pNMListView->uChanged & LVIF_STATE) &&
		(pNMListView->uNewState & LVIS_SELECTED)) 
	{	
		m_pInvoiceSession->SetSelectedDocItem(pNMListView->iItem);
	}

	*pResult = (LRESULT)0;

}

void CBaseInvoiceDlg::OnCowPriceListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{

	CowPriceData *pCowPrice = NULL;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if ((pNMListView->uChanged & LVIF_STATE) &&
		(pNMListView->uNewState & LVIS_SELECTED)) 
	{
		pCowPrice = m_pInvoiceSession->GetCowPrice(pNMListView->iItem);
		


	//	Money termPrice = pInvoiceContext->GetTermPrice(pCowPrice->GetCow());
	//	NullInt termWeight = pInvoiceContext->GetTermWeight(pCowPrice->GetCow());
	//	Stock* pTermStock = pInvoiceContext->GetTermStock(pCowPrice->GetCow());

		/*if(!termPrice.IsNull())
			SetTermValue(IDC_TERMPRICE_VALUE,termPrice.ToString(),IDC_SETTERMPRICEBTN,TRUE);
		else
			SetTermValue(IDC_TERMPRICE_VALUE,szMissing,IDC_SETTERMPRICEBTN,FALSE);

		if(!termWeight.IsNull())
			SetTermValue(IDC_TERMWEIGHT_VALUE,termWeight.ToString(),IDC_SETTERMWEIGHTBTN,TRUE);
		else
			SetTermValue(IDC_TERMWEIGHT_VALUE,szMissing,IDC_SETTERMWEIGHTBTN,FALSE);


		if(pTermStock)
			SetTermValue(IDC_TERMSTOCK_VALUE,(LPCSTR)pTermStock->GetStockCode(),IDC_SETTERMSTOCKBTN,TRUE);
		else
			SetTermValue(IDC_TERMSTOCK_VALUE,szMissing,IDC_SETTERMSTOCKBTN,FALSE);*/

		
		m_pInvoiceSession->SetSelectedCowPriceItem(pNMListView->iItem);
		OnCowPriceSelected(pCowPrice);

	}

	*pResult = (LRESULT)0;
}

void CBaseInvoiceDlg::OnCowPriceSelected(CowPriceData* pCowPrice)
{

}

//
//void CBaseInvoiceDlg::SetDlgItemResText(UINT nID,UINT resID)
//{
//	CString text;
//	text.LoadString(resID);
//	SetDlgItemText(nID,text);
//}
//
//void CBaseInvoiceDlg::SetTermValue(UINT nValueLabelID,LPCSTR pszValueText,UINT nSetBtnID,BOOL bEnable)
//{
//	SetDlgItemText(nValueLabelID,pszValueText);
//	GetDlgItem(nSetBtnID)->EnableWindow(bEnable);
//}
//
//
//void CBaseInvoiceDlg::OnSetTermWeightAll()
//{
//	CowPriceData *pCowPrice = NULL;
//	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
//	{		
//		pCowPrice  =  m_pInvoiceSession->GetCowPrice(i);
//		pCowPrice->SetCustomWeight(m_pInvoiceSession->GetInvoiceContext()->GetTermWeight(pCowPrice->GetCow()).ToInt());
//	}
//	
//	UpdateCowPriceList();
//	UpdateTotal();
//
//}
//void CBaseInvoiceDlg::OnSetTermStockAll()
//{
//	CowPriceData *pCowPrice = NULL;
//	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
//	{
//		pCowPrice = m_pInvoiceSession->GetCowPrice(i);
//		pCowPrice->SetCustomStock(m_pInvoiceSession->GetInvoiceContext()->GetTermStock(pCowPrice->GetCow()));
//	}
//
//	UpdateCowPriceList();
//	UpdateTotal();
//}
//
//void CBaseInvoiceDlg::OnSetTermPriceAll()
//{
//	CowPriceData *pCowPrice = NULL;
//	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
//	{
//		pCowPrice  = m_pInvoiceSession->GetCowPrice(i);
//		pCowPrice->SetCustomPriceNetto(m_pInvoiceSession->GetInvoiceContext()->GetTermPrice(pCowPrice->GetCow()));
//	}
//
//	UpdateCowPriceList();
//	UpdateTotal();	
//}
//



void CBaseInvoiceDlg::OnCustomNumberFocus()
{
	CWnd* customNumberText = GetDlgItem(IDC_CUSTOMNUMBER);

	CRect rect;
	customNumberText->GetClientRect(&rect);
	customNumberText->ClientToScreen(&rect);
	m_lastInvTip.Show(m_pInvoiceSession->GetInvoiceContext()->GetInvoiceType(),CPoint(rect.left,rect.bottom));
}
void CBaseInvoiceDlg::OnCustomNumberLostFocus()
{
	m_lastInvTip.Hide();
}

int CBaseInvoiceDlg::GetSelectedPriceItem()
{
	return m_pricelist.GetCurSel();
}

void CBaseInvoiceDlg::SetInvoiceControls()
{
	m_vat.SetVATRate(m_pInvoiceSession->GetVATRate());		
	m_payway.ReloadCombo(m_pInvoiceSession->GetPayway());	
	m_extras.SetWindowText(m_pInvoiceSession->GetExtras());
	m_customNumber = m_pInvoiceSession->GetCustomNumber();
	m_invoicedate.SetDate(m_pInvoiceSession->GetInvoiceDate());
	m_spnPayDueDays.SetPos(m_pInvoiceSession->GetPayDueDays());
	m_TransactionDate.SetDate(m_pInvoiceSession->GetTransactionDate());
	m_sTransactionPlace = m_pInvoiceSession->GetTransactionPlace();
}


 void CBaseInvoiceDlg::AddItem()
 {
	 m_pInvoiceSession->AddItem();
	 m_docList.UpdateItemCount();
	 UpdateItemCount();
	 UpdateTotal();
 }

 void CBaseInvoiceDlg::DelItem()
 {
	 m_pricelist.Lock();
	 m_docList.Lock();

	 m_pInvoiceSession->DelItem();
	 m_docList.UpdateItemCount();
	 UpdateItemCount();
	 
	 m_pricelist.Unlock();
	 m_docList.Unlock();

	 UpdateTotal();
 }

 bool CBaseInvoiceDlg::CommitInvoice()
 {
	CString sExtras;
	m_extras.GetWindowText(sExtras);
	m_pInvoiceSession->SetExtras(sExtras);

	CDateTime dtInvoiceDate;
	m_invoicedate.GetDate(dtInvoiceDate);	
	m_pInvoiceSession->SetInvoiceDate(dtInvoiceDate);
	
	m_pInvoiceSession->SetPayway(m_payway.GetPayway());	

	m_pInvoiceSession->SetCustomNumber(m_customNumber);

	m_pInvoiceSession->SetPayDueDays(m_spnPayDueDays.GetPos());

	CDateTime transactionDt;
	m_TransactionDate.GetDate(transactionDt);

	m_pInvoiceSession->SetTransactionDate(transactionDt);
	m_pInvoiceSession->SetTransactionPlace(m_sTransactionPlace);


	return m_pInvoiceSession->CommitInvoice();
 }

 CString CBaseInvoiceDlg::GetInvDlgCaption()
 {
	 return m_pInvoiceSession->GetSessionTitle();
 }

 void CBaseInvoiceDlg::TabChanged(int newTabIndex)
 {
	 //m_pDriver->TabChanged(newTabIndex);
 }

 void CBaseInvoiceDlg::OnSetTransactionPlace()
 {
	 m_sTransactionPlace = m_pInvoiceSession->GetInvoiceHent()->GetCity();
	 UpdateData(FALSE);
 }
