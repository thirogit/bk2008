#include "stdafx.h"
#include "findcowdlg.h"
#include "finddlg.h"
#include "../mainfrm_ctrl.h"
#include "../classes/match/CowMatcher.h"
#include "../classes/forallactions/FindAction.h"

BEGIN_MESSAGE_MAP(CFindCowDlg, CFindBaseDlg)
	//{{AFX_MSG_MAP(CFindCowDlg)
	ON_BN_CLICKED(IDC_PASSENABLE, OnSpanCheck)
	ON_BN_CLICKED(IDC_BIRTHENABLE, OnSpanCheck)
	ON_BN_CLICKED(IDOK,OnOK)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFindCowDlg::CFindCowDlg() : CFindBaseDlg(CFindCowDlg::IDD),
							m_EAN(2,12), m_motherEAN(2,12), m_PassNo(2,10),
							m_deliver(false),m_buyer(false),m_bInCow(false)
{
	
	m_bPassEnable = FALSE;
	m_bBirthEnable = FALSE;

}



void CFindCowDlg::DoDataExchange(CDataExchange* pDX)
{
	CFindBaseDlg::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CFindCowDlg)
		DDX_Control (pDX, IDC_PASSDATESPAN  , m_passdatespan ) ;
		DDX_Control (pDX, IDC_BIRTHDATESPAN ,m_birthdatespan  ) ;

		DDX_Control (pDX, IDC_BYSEX ,m_sex  ) ;
		//DDX_Text (pDX, IDC_BYWEIGHT ,m_weight  ) ;
		DDX_Control (pDX, IDC_BYWEIGHT ,m_weightrange  ) ;
		DDX_Control (pDX, IDC_BYSTOCK ,m_stock  ) ;
		DDX_Text (pDX, IDC_BYBIRTHPLACE ,m_birthplace  ) ;
		DDX_Control (pDX, IDC_BYDELIVER ,m_deliver  ) ;
		DDX_Control (pDX, IDC_BYBUYER,m_buyer  ) ;
		DDX_Control(pDX,IDC_BYFSTOWNR,m_fstownr);

		DDX_Check(pDX,IDC_PASSENABLE,m_bPassEnable);
		DDX_Check(pDX,IDC_BIRTHENABLE,m_bBirthEnable);
		DDX_Control(pDX,IDC_BYBUYPRICE,m_buypricerange);
		DDX_Control(pDX,IDC_BYSELLPRICE,m_sellpricerange);
		DDX_Control(pDX,IDC_BYEAN,m_EAN);
		DDX_Control(pDX,IDC_BYMOTHEREAN,m_motherEAN);
		DDX_Control(pDX,IDC_BYPASSNO,m_PassNo);
		DDX_Check(pDX,IDC_ININCOWS,m_bInCow);


		DDX_Control(pDX,IDC_BYBUYWEIGHT,m_buyweightrange);
		DDX_Control(pDX,IDC_BYSELLWEIGHT,m_sellweightrange);
		DDX_Control(pDX,IDC_BYBUYSTOCK,m_buystock);
		DDX_Control(pDX,IDC_BYSELLSTOCK,m_sellstock);
		
		
	//}}AFX_DATA_MAP
}

BOOL CFindCowDlg::OnInitDialog()
{
	CFindBaseDlg::OnInitDialog();
	/*m_EAN.SubclassDlgItem (IDC_BYEAN, this);
	m_motherEAN.SubclassDlgItem (IDC_BYMOTHEREAN, this);
	m_PassNo.SubclassDlgItem (IDC_BYPASSNO, this);*/


//	TRACE("style = %08X,exstyle = %08X\n",m_EAN.GetStyle(),m_EAN.GetExStyle());
	//SetIntSpanCombo(m_weightrange);

	m_stock.InitFindCombo();
	m_deliver.InitFindCombo();
	m_fstownr.InitFindCombo();
	m_buyer.InitFindCombo();

	m_buystock.InitFindCombo();
	m_sellstock.InitFindCombo();

	OnSpanCheck();
	
	return TRUE;
}


void CFindCowDlg::OnSpanCheck(void)
{
	UpdateData();

	m_passdatespan.EnableWindow(m_bPassEnable);
	m_birthdatespan.EnableWindow(m_bBirthEnable);


}

int CFindCowDlg::GoSearch(CFindSetup& setup)
{
	
	UpdateData();

	CCowDataMatcher cowMatcher;

	if(m_EAN.GetWindowTextLength() > 0)		
		cowMatcher.FindEAN(m_EAN.GetWindowText());

	if(m_motherEAN.GetWindowTextLength() > 0)
		cowMatcher.FindMotherEAN(m_motherEAN.GetWindowText());
		
	if(m_PassNo.GetWindowTextLength() > 0)
		cowMatcher.FindPassNo(m_PassNo.GetWindowText());

	if(m_birthplace.GetLength() > 0)
		cowMatcher.FindBirthPlace(m_birthplace);

	if(setup.ByExtras())
		cowMatcher.FindExtras(setup.GetExtras());

	if(!m_stock.IsNeutral())
		cowMatcher.FindStock(m_stock.GetSelItemData());
	
	if(m_bPassEnable)
	{
		CDateTime pass_t1,pass_t2;
		TIMESPAN pass_span_type = m_passdatespan.GetSpan(pass_t1,pass_t2);
		cowMatcher.FindPassDate(pass_t1,pass_t2,pass_span_type);
	}

	if(m_bBirthEnable)
	{
		CDateTime birth_t1,birth_t2;
		TIMESPAN birth_span_type = m_birthdatespan.GetSpan(birth_t1,birth_t2);
		cowMatcher.FindBirthDate(birth_t1,birth_t2,birth_span_type);
	}

	if(!m_sex.IsNeutral())
		cowMatcher.FindSex(CowSex(m_sex.GetSelSex()));
	

	if(!m_fstownr.IsNeutral())
		cowMatcher.FindFirstOwner(m_fstownr.GetSelItemData());
	

	if(!m_deliver.IsNeutral())
		cowMatcher.FindProvider(m_deliver.GetSelItemData());
	

	if(!m_buyer.IsNeutral())
		cowMatcher.FindBuyer(m_buyer.GetSelItemData());
	

	if(!m_sellstock.IsNeutral())
		cowMatcher.FindSellStock(m_sellstock.GetSelItemData());

	if(!m_buystock.IsNeutral())
		cowMatcher.FindBuyStock(m_buystock.GetSelItemData());
	
	if(m_weightrange.HasValue())
		cowMatcher.FindWeight(m_weightrange.GetValue(),m_weightrange.GetRangeType());

	if(m_sellweightrange.HasValue())
		cowMatcher.FindSellWeight(m_sellweightrange.GetValue(),m_sellweightrange.GetRangeType());

	if(m_sellweightrange.HasValue())
		cowMatcher.FindBuyWeight(m_sellweightrange.GetValue(),m_sellweightrange.GetRangeType());
	
	if(m_buypricerange.HasValue())
		cowMatcher.FindBuyPrice(m_buypricerange.GetValue(),m_buypricerange.GetRangeType());

	if(m_sellpricerange.GetValue())
		cowMatcher.FindSellPrice(m_sellpricerange.GetValue(),m_sellpricerange.GetRangeType());
	
	if(m_bInCow)
		cowMatcher.FindNotSold();
	
	if(setup.ByCreationTime())
	{
		CDateTime time1,time2;
		TIMESPAN creationSearchSpan = setup.CreationTimeSpan(time1,time2);
		cowMatcher.FindCreationTime(time1,time2,creationSearchSpan);
	}
	
	if(setup.ByLastModificationTime())
	{
		CDateTime time1,time2;
		TIMESPAN lastModificationSearchSpan = setup.LastModificationTimeSpan(time1,time2);
		cowMatcher.FindLastModificationTime(time1,time2,lastModificationSearchSpan);
	}

	UserData *pCreator = setup.SelectedCreator();
	if(pCreator)
		cowMatcher.FindCreator(pCreator);
		
	UserData* pLastModifier = setup.SelectedLastModifier();
	if(pLastModifier)
		cowMatcher.FindLastModifier(pLastModifier);
	

	if(setup.GetInResult())
		cowMatcher.FindMarked();

	CFindAction<CCowArray::ArrayAction,CCowDataMatcher,Cow> findCow(setup.GetAddToResult(),&cowMatcher);
		
	ary_cows.ForAllAction(findCow);

	setup.ChangeMainGrid(m_bInCow ? INCOWSGRID : ALLCOWSGRID );

	return findCow.GetFoundCount();
}






