#include "stdafx.h"
#include "findinvdlg.h"
#include "gridtabs.h"
#include "../global_arrays.h"
#include "../classes/match/InvMatcher.h"
#include "../classes/forallactions/FindAction.h"

BEGIN_MESSAGE_MAP(CFindInvDlg, CFindBaseDlg)
	ON_BN_CLICKED(IDC_BYINVDATE,UpdateDateSpans)
	ON_BN_CLICKED(IDC_BYTRANSDATE,UpdateDateSpans)
	ON_CBN_SELCHANGE(IDC_BYPAID,OnPaidChange)
END_MESSAGE_MAP()


//  DDX_MyRadio(), which is a modified DDX_Radio().
// 
void AFXAPI DDX_MyRadio(CDataExchange* pDX, int nIDC, int& value)
    // must be first in a group of auto radio buttons
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT(::GetWindowLong(hWndCtrl, GWL_STYLE) & WS_GROUP);
    ASSERT(::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON);
    if( pDX->m_bSaveAndValidate )
        value = -1;     // value if none found
    // walk all children in group
    int iButton = 0;
    do
    {
        if( ::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON)
        {
        // control in group is a radio button
        if( pDX->m_bSaveAndValidate )
        {
            if( ::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L) != 0 )
            {
                ASSERT(value == -1);    // only set once
                value = iButton;
            }
        }
        else
        {
        // select button
            ::SendMessage( hWndCtrl, BM_SETCHECK, (iButton == value), 0L
);
        }
        iButton++;
        }
        else
        {
			TRACE( "Warning: skipping non-radio button in group.\n" );
        }
        hWndCtrl = ::GetWindow( hWndCtrl, GW_HWNDNEXT );
    } while(hWndCtrl!=NULL && !(GetWindowLong(hWndCtrl,GWL_STYLE)&WS_GROUP));
}
			

CFindInvDlg::CFindInvDlg() : CFindBaseDlg(CFindInvDlg::IDD)
{ 
m_invTypeRadio = 0;
m_bByInvDate = FALSE;
m_bSearchForCorrect = FALSE;
}

void CFindInvDlg::DoDataExchange(CDataExchange* pDX)
{
	CFindBaseDlg::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_BYINVNOFROM,m_invnofrom);
	DDX_Text(pDX,IDC_BYINVNOTO,m_invnoto);

	DDX_Check(pDX,IDC_BYINVDATE,m_bByInvDate);	
	DDX_Control(pDX,IDC_INVDATESPAN,m_invdatespan);

	DDX_Control(pDX,IDC_BYVATRATE,m_vatrate);
	DDX_Control(pDX,IDC_BYPAYWAY,m_payway);

	DDX_Control(pDX,IDC_BYPAID,m_paid);
	DDX_Control(pDX,IDC_PAIDDATE,m_paiddate);

	DDX_Control(pDX,IDC_BYTOTAL,m_total);
	
	DDX_Radio(pDX,IDC_SELLINVRADIO,m_invTypeRadio);
	
	DDX_Check(pDX,IDC_CORRECT,m_bSearchForCorrect);
}

void CFindInvDlg::UpdateDateSpans()
{
	UpdateData();
	m_invdatespan.EnableWindow(m_bByInvDate);
}

void CFindInvDlg::OnPaidChange()
{
	m_paiddate.EnableWindow((PAID_SEARCH_TYPE)m_paid.GetItemData(m_paid.GetCurSel()) == PAID_ONDATE);
}

BOOL CFindInvDlg::OnInitDialog()
{
	CFindBaseDlg::OnInitDialog();

	struct tagYesNoComboItem
	{
		char szItemText[5];
		PAID_SEARCH_TYPE  itemData;
	} YesNoItems[] =
	{
		{"TAK",PAID_YES},
		{"NIE",PAID_NO},
		{"*",PAID_NOSEARCH},
		{"Dnia",PAID_ONDATE}
	};
	int item,neutralItem = -1;
	m_paid.ModifyStyle(CBS_SORT,0);
	for(int i = 0;i < (sizeof(YesNoItems)/sizeof(tagYesNoComboItem));i++)
	{
		item = m_paid.AddString(YesNoItems[i].szItemText);
		m_paid.SetItemData(item,YesNoItems[i].itemData);
		if(YesNoItems[i].itemData == PAID_NOSEARCH) neutralItem = item;
	}
	m_paid.SetCurSel(neutralItem);


	m_payway.InitFindCombo();
	UpdateDateSpans();
	OnPaidChange();

	


	return TRUE;
}


int CFindInvDlg::GoSearch(CFindSetup& setup)
{
	CInvoiceDataArray *invsArray = NULL;
	UpdateData();

	CInvDataMatcher invMatcher;


	int InvNoFrom = 0,InvNoTo = INT_MAX;
	bool bByInvNo = false;
	if(m_invnofrom.GetLength() > 0)
	{
		InvNoFrom = atoi(m_invnofrom);
		bByInvNo = true;
	}
	if(m_invnoto.GetLength() > 0)
	{
		InvNoTo = atoi(m_invnoto);
		bByInvNo = true;
	}
	if(bByInvNo)
		invMatcher.FindInvNo(InvNoFrom,InvNoTo);

	if(setup.ByExtras())
		invMatcher.FindExtras(setup.GetExtras());

	if(m_bByInvDate) 
	{
		CDateTime inv_t1,inv_t2;
		TIMESPAN inv_span_type = m_invdatespan.GetSpan(inv_t1,inv_t2);
		invMatcher.FindInvDate(inv_t1,inv_t2,inv_span_type);		
	}
	

	if(setup.ByCreationTime())
	{
		CDateTime time1,time2;
		TIMESPAN creationSearchSpan = setup.CreationTimeSpan(time1,time2);
		invMatcher.FindCreationTime(time1,time2,creationSearchSpan);
	}
	
	if(setup.ByLastModificationTime())
	{
		CDateTime time1,time2;
		TIMESPAN lastModificationSearchSpan = setup.LastModificationTimeSpan(time1,time2);
		invMatcher.FindLastModificationTime(time1,time2,lastModificationSearchSpan);
	}

	UserData *pCreator = setup.SelectedCreator();
	if(pCreator)
		invMatcher.FindCreator(pCreator);
		
	UserData* pLastModifier = setup.SelectedLastModifier();
	if(pLastModifier)
		invMatcher.FindLastModifier(pLastModifier);
	
	if(!m_payway.IsNeutral())
		invMatcher.FindPayway((Invoice::PAYWAYS)m_payway.GetItemData(m_payway.GetCurSel()));

	if(m_vatrate.HasValue())
		invMatcher.FindVATRate(m_vatrate.GetValue()/100.0,m_vatrate.GetRangeType());
	
	if(m_total.HasValue())
		invMatcher.FindTotal(m_total.GetValue(),m_total.GetRangeType());

	if(setup.GetInResult())
		invMatcher.FindMarked();

	PAID_SEARCH_TYPE paidcmpaction = (PAID_SEARCH_TYPE)m_paid.GetItemData(m_paid.GetCurSel());	
	if(paidcmpaction != PAID_NOSEARCH)
	{
		TIMESPAN paid_span_type;
		CDateTime paid_t1,paid_t2;

		switch(paidcmpaction)
		{
			case PAID_NO:			
				invMatcher.FindPaidBool(false) ;
			break;
			case PAID_YES:
				invMatcher.FindPaidBool(true) ;
			break;
			case PAID_ONDATE:
				paid_span_type = m_paiddate.GetSpan(paid_t1,paid_t2);
				invMatcher.FindPaidDate(paid_t1,paid_t2,paid_span_type);
			break;
		}

	}

	if(m_bSearchForCorrect)
		invMatcher.FindOnlyCorrects();

	CFindAction<CInvoiceDataArray::ArrayAction,CInvDataMatcher,Invoice> findInvoice(setup.GetAddToResult(),&invMatcher);
	

	switch(m_invTypeRadio)
	{
		case FIND_VAT_OUTINVOICE:
			ary_sell_invs.ForAllAction(findInvoice);
			setup.ChangeMainGrid(SELLINVSGRID);
		break;
		case FIND_VAT_ININVOICE:
			ary_buy_invs.ForAllAction(findInvoice);
			setup.ChangeMainGrid(BUYINVSGRID);
		break;
		case FIND_RR_ININVOICE:
			ary_rr_invs.ForAllAction(findInvoice);
			setup.ChangeMainGrid(RRINVSGRID);
		break;
		default:
			ASSERT(FALSE);
	}
	
	
	return findInvoice.GetFoundCount();
		
}
