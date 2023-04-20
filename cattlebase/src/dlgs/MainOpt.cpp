#include "stdafx.h"
#include "MainOpt.h"
#include "../ctrls/EANEDit.h"
#include "LoginDlg.h"
#include "../ctrls/FarmNoEdit.h"
#include "MainCfg.h"
#include "../missing/util.h"
#include "../global_arrays.h"
#include "../consts.h"
// CMainOpt dialog

BEGIN_MESSAGE_MAP(CMainOpt, CPropertyPage)
	ON_BN_CLICKED(IDC_DOUBLECHECK, UpdateControls)
	ON_BN_CLICKED(IDC_ASKATEXIT, UpdateControls)
	//ON_BN_CLICKED(IDC_SHOWWELCOME, UpdateControls)
	
	ON_BN_CLICKED(IDC_CHECKEANS,UpdateControls)
	ON_BN_CLICKED(IDC_AUTOLOGIN,UpdateControls)
	
	ON_BN_CLICKED(IDC_MASKMYPRICE,AuthorizeUser)

	ON_BN_CLICKED(IDC_MARKCOWAGE,UpdateControls)
	ON_BN_CLICKED(IDC_WARNBEFOREDATE,UpdateControls)
	ON_BN_CLICKED(IDC_MARKBUYSELLLOSS,UpdateControls)

	ON_BN_CLICKED(IDC_WARNTOSMALLPRICECHECK,UpdateControls)
	ON_BN_CLICKED(IDC_WARNTOBIGPROFIT,UpdateControls)


	ON_CBN_SELENDOK(IDC_GRPCLRCOMBO, OnColorSelEnd)

	ON_BN_CLICKED(IDC_MARKMYPRICELOSSBUY,UpdateControls)
	ON_BN_CLICKED(IDC_MARKMYPRICELOSSSELL,UpdateControls)

END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(CMainOpt, CPropertyPage)
CMainOpt::CMainOpt()
: CPropertyPage(CMainOpt::IDD),m_REunsigned(g_szRegExUnsigned),
		m_MinPriceEdit(&m_REunsigned),m_MaxProfitEdit(&m_REunsigned)
		//,m_FPMWRateEdit("[0-9]{1,2}([,.][0-9]{0,2})?")
{

	m_bDoubleAskDel = AppCfg().GetBool(CFG_DOUBLEASKDEL);
	//m_bWelcomeScreen = AppCfg().GetBool(CFG_WELCOMESCREEN);
	m_bAskAtExit = AppCfg().GetBool(CFG_ASKBEFOREEXIT);

	
	m_autologin = AppCfg().GetBool(CFG_AUTOLOGIN);
	m_autopass = AppCfg().GetString(CFG_AUTOPASS);
	m_autouserid = AppCfg().GetNum(CFG_AUTOUSERID);	
	
	m_bCheckEANs = AppCfg().GetBool(CFG_CHECKEANS);
	m_bMaskMyPrice = AppCfg().GetBool(CFG_MASKMYPRICE);

	m_bMarkCowAge = AppCfg().GetBool(CFG_MARKCOWAGE);
	m_CowAgeDays = AppCfg().GetNum(CFG_MARKCOWAGEDAYS);

	m_bWarnCowAge = AppCfg().GetBool(CFG_WARNCOWAGE);
	m_MarkBuySellLoss = AppCfg().GetBool(CFG_MARKBUYSELLLOSS);
		
	m_WarnCowAgeDays = AppCfg().GetNum(CFG_WARNCOWAGEDAYS);
	

	m_MarkAgeColor = AppCfg().GetRGB(CFG_MARKCOWAGECLR);
	m_WarnAgeColor = AppCfg().GetRGB(CFG_WARNCOWAGECLR);
	m_MarkLossColor = AppCfg().GetRGB(CFG_MARKBUYSELLLOSSCLR);
	m_bWarnEmptyPlateNo = AppCfg().GetBool(CFG_WARNEMPTYPLATENO);

	m_bWarnToSmallPrice = AppCfg().GetBool(CFG_WARNTOSMALLPRICE);
	m_WarnMinPrice = AppCfg().GetFloat(CFG_WARNMINPRICE);
	m_bWarnToBigProfit = AppCfg().GetBool(CFG_WARNTOBIGPROFIT);
	m_MaxProfit = AppCfg().GetFloat(CFG_WARNMAXPROFIT);

	m_WarnToBigProfitClr = AppCfg().GetRGB(CFG_WARNTOBIGPROFITCLR);
	m_bCheckFarmNo = AppCfg().GetBool(CFG_CHECKFARMNO);

	
	m_nMarkMyPriceGtLtBuy = AppCfg().GetNum(CFG_MARKMYPRICELOSSBUY);
	m_nMarkMyPriceGtLtSell = AppCfg().GetNum(CFG_MARKMYPRICELOSSSELL);

	m_bMarkMyPriceGtLtBuy = m_nMarkMyPriceGtLtBuy != GTLT_NONE;
	m_bMarkMyPriceGtLtSell = m_nMarkMyPriceGtLtSell != GTLT_NONE;
	//m_dFPMWRate = AppCfg().GetFloat(CFGID_FPMWRATE);


	
}

CMainOpt::~CMainOpt()
{
}

void CMainOpt::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX,IDC_DOUBLECHECK,m_bDoubleAskDel);
	DDX_Check(pDX,IDC_ASKATEXIT,m_bAskAtExit);
	//DDX_Check(pDX,IDC_SHOWWELCOME,m_bWelcomeScreen);
	
	DDX_Control(pDX,IDC_AUTOUSER,m_autousers);
	DDX_Text(pDX,IDC_AUTOPASS,m_autopass);
	DDX_Check(pDX,IDC_AUTOLOGIN,m_autologin);
	
	
	DDX_Check(pDX,IDC_CHECKEANS,m_bCheckEANs);
	DDX_Check(pDX,IDC_MASKMYPRICE,m_bMaskMyPrice);


	DDX_Check(pDX,IDC_MARKCOWAGE,m_bMarkCowAge);
	DDX_Check(pDX,IDC_WARNBEFOREDATE,m_bWarnCowAge);
	DDX_Check(pDX,IDC_MARKBUYSELLLOSS,m_MarkBuySellLoss);


	DDX_Text(pDX, IDC_MARKDAYAGE, m_CowAgeDays);
	DDV_MinMaxUInt(pDX, m_CowAgeDays, 1, 999);

	DDX_Text(pDX, IDC_WARNDAYS, m_WarnCowAgeDays);
	DDV_MinMaxUInt(pDX, m_WarnCowAgeDays, 1, 998);

	
	DDX_Control(pDX, IDC_MARKAGECOLOR,m_ctlMarkAgeColour);
	DDX_Control(pDX, IDC_AGEWARNCOLOR,m_ctlWarnAgeColour);
	DDX_Control(pDX, IDC_BUYSELLLOSSCOLOR,m_ctlWarnLossColour);

	DDX_ColorPicker(pDX, IDC_MARKAGECOLOR, m_MarkAgeColor);
	DDX_ColorPicker(pDX, IDC_AGEWARNCOLOR, m_WarnAgeColor);
	DDX_ColorPicker(pDX, IDC_BUYSELLLOSSCOLOR, m_MarkLossColor);
	DDX_Check(pDX,IDC_WARNEMPTYPLATENO,m_bWarnEmptyPlateNo);

	DDX_Check(pDX,IDC_WARNTOSMALLPRICECHECK,m_bWarnToSmallPrice);
	DDX_Check(pDX,IDC_WARNTOBIGPROFIT,m_bWarnToBigProfit);

	DDX_Control(pDX,IDC_WARNTOSMALLPRICE,m_MinPriceEdit);
	DDX_Control(pDX,IDC_MAXPROFITEDIT,m_MaxProfitEdit);

	DDX_Control(pDX, IDC_WARNTOBIGPRICECLR,m_ctlWarnToBigProfitClr);

	DDX_ColorPicker(pDX, IDC_WARNTOBIGPRICECLR, m_WarnToBigProfitClr);

	DDX_Control(pDX,IDC_GRPCLRCOMBO,m_grpClrCombo);
	DDX_Check(pDX,IDC_CHECKFARMNO,m_bCheckFarmNo);

	DDX_Control(pDX,IDC_BUY_GT_LT_COMBO,m_MyPriceGtLtBuy);
	DDX_Check(pDX,IDC_MARKMYPRICELOSSBUY,m_bMarkMyPriceGtLtBuy);

	DDX_Control(pDX,IDC_SELL_GT_LT_COMBO,m_MyPriceGtLtSell);
	DDX_Check(pDX,IDC_MARKMYPRICELOSSSELL,m_bMarkMyPriceGtLtSell);


	
	
}

BOOL CMainOpt::OnInitDialog()
{
	if(!CPropertyPage::OnInitDialog()) return FALSE;

	m_autousers.ReloadCombo(ary_users.GetPtrByUserId(m_autouserid));
	

	m_MinPriceEdit.SetWindowText(m_WarnMinPrice.ToString());
	m_MaxProfitEdit.SetWindowText(m_MaxProfit.ToString());


	COLORREF CustColors[CUSTOM_COLORS_COUNT];
	for(int i = 0;i < CUSTOM_COLORS_COUNT;i++)
	{
		CustColors[i] = AppCfg().GetRGB(CFG_CUSTOMCOLORS_ITEM,i);
	}
	m_ctlWarnToBigProfitClr.SetCustomColors(CustColors);


	int iItem;
	CString grpNo;
	for(int i = 0;i <= MAX_INOUT_GRP;i++)
	{
		grpNo.Format("-%d-",i);
		iItem = m_grpClrCombo.AddString(grpNo);
		m_grpClrCombo.SetItemData(iItem,AppCfg().GetRGB(CFG_GROUPCOLORS_ITEM,i));
	}

	GetDlgItem(IDC_MASKMYPRICE)->EnableWindow(HAS_PRIVLEDGE(INVOICE));

	m_MyPriceGtLtBuy.InitCombo(m_nMarkMyPriceGtLtBuy);
	m_MyPriceGtLtSell.InitCombo(m_nMarkMyPriceGtLtSell);

	UpdateData(FALSE);
	UpdateControls();

	/*CString FPMWRateStr;
	FPMWRateStr.Format("%4.2f",m_dFPMWRate*100);
	m_FPMWRateEdit.SetWindowText(FPMWRateStr);*/
	
	return TRUE;
}

void CMainOpt::AuthorizeUser()
{
	if(m_bMaskMyPrice)
	{
		CLoginDlg loginDlg(UserData::GetCurrentUser(),false);
		UpdateData(loginDlg.DoModal() == IDOK);
	}
	else
		UpdateData();
}


void CMainOpt::UpdateControls()
{
	UpdateData();
	m_autousers.EnableWindow(m_autologin);
	GetDlgItem(IDC_AUTOPASS)->EnableWindow(m_autologin);
	
	GetDlgItem(IDC_MARKDAYAGE)->EnableWindow(m_bMarkCowAge);
	m_ctlMarkAgeColour.EnableWindow(m_bMarkCowAge);


	GetDlgItem(IDC_WARNDAYS)->EnableWindow(m_bWarnCowAge);
	m_ctlWarnAgeColour.EnableWindow(m_bWarnCowAge);

	m_ctlWarnLossColour.EnableWindow(m_MarkBuySellLoss);
	
	m_MinPriceEdit.EnableWindow(m_bWarnToSmallPrice);

	m_MaxProfitEdit.EnableWindow(m_bWarnToBigProfit);
	
	m_ctlWarnToBigProfitClr.EnableWindow(m_bWarnToBigProfit);

	m_MyPriceGtLtBuy.EnableWindow(m_bMarkMyPriceGtLtBuy);
	m_MyPriceGtLtSell.EnableWindow(m_bMarkMyPriceGtLtSell);
}

BOOL CMainOpt::OnApply()
{

//	m_WarnDateEdit.GetDate(m_WarnDate);

/*
	if(!m_WarnDate.IsNull())
	{
		if(m_WarnDate <= time(NULL))
		{
			AfxMessageBox(IDS_PASTDATE,MB_OK,0);
			m_WarnDateEdit.SetFocus();
			return FALSE;
		}

	}
	
*/

	CString minPriceFloatStr,maxProfitFloatStr;

	m_MinPriceEdit.GetWindowText(minPriceFloatStr);
	m_MaxProfitEdit.GetWindowText(maxProfitFloatStr);

	m_WarnMinPrice = CUtil::myatof((LPCSTR)minPriceFloatStr);
	m_MaxProfit = CUtil::myatof((LPCSTR)maxProfitFloatStr);


	if(m_WarnCowAgeDays >= m_CowAgeDays)
	{
		AfxMessageBox(IDS_ERRCOWAGEERROR,MB_OK,0);
		GetDlgItem(IDC_WARNDAYS)->SetFocus();
		return FALSE;
	}

	m_nMarkMyPriceGtLtBuy = m_MyPriceGtLtBuy.GetSelGtLt();

	if(m_bMarkMyPriceGtLtBuy && m_nMarkMyPriceGtLtBuy == GTLT_NONE)
	{
		AfxMessageBox(IDS_ERRNOGTLTBUYCHOSEN);
		m_MyPriceGtLtBuy.SetFocus();
		return FALSE;
	}

	m_nMarkMyPriceGtLtSell = m_MyPriceGtLtSell.GetSelGtLt();
	if(m_bMarkMyPriceGtLtSell && m_nMarkMyPriceGtLtSell == GTLT_NONE)
	{
		AfxMessageBox(IDS_ERRNOGTLTSELLCHOSEN);
		m_MyPriceGtLtSell.SetFocus();
		return FALSE;
	}




	AppCfg().SetBool(CFG_DOUBLEASKDEL,m_bDoubleAskDel);
	AppCfg().SetBool(CFG_ASKBEFOREEXIT,m_bAskAtExit);

	UserData *pUser;

	AppCfg().SetBool(CFG_AUTOLOGIN,m_autologin);
	if(m_autologin)
	{
		pUser = m_autousers.GetSelItemData();
		if(pUser) 
		{
			AppCfg().SetString(CFG_AUTOPASS,m_autopass);
			AppCfg().SetNum(CFG_AUTOUSERID,pUser->userid);
		}
		else
		{
			AfxMessageBox(IDS_ERRSELUSER,MB_OK,0);
			return FALSE;
		}
	}
	else
	{
		AppCfg().SetString(CFG_AUTOPASS,"");
		AppCfg().SetNum(CFG_AUTOUSERID,0);
	}

	
	AppCfg().SetBool(CFG_CHECKEANS,m_bCheckEANs);
	

	AppCfg().SetBool(CFG_MASKMYPRICE,m_bMaskMyPrice);


	AppCfg().SetBool(CFG_MARKCOWAGE,m_bMarkCowAge);
	AppCfg().SetNum(CFG_MARKCOWAGEDAYS,m_CowAgeDays);

	AppCfg().SetBool(CFG_WARNCOWAGE,m_bWarnCowAge);
	AppCfg().SetBool(CFG_MARKBUYSELLLOSS,m_MarkBuySellLoss);
		
	AppCfg().SetNum(CFG_WARNCOWAGEDAYS,m_WarnCowAgeDays);
	
	
	AppCfg().SetRGB(CFG_MARKCOWAGECLR,m_MarkAgeColor);
	AppCfg().SetRGB(CFG_WARNCOWAGECLR,m_WarnAgeColor);
	AppCfg().SetRGB(CFG_MARKBUYSELLLOSSCLR,m_MarkLossColor);

	AppCfg().SetBool(CFG_WARNEMPTYPLATENO,m_bWarnEmptyPlateNo);

	AppCfg().SetFloat(CFG_WARNMINPRICE,m_WarnMinPrice.GetDouble());
	AppCfg().SetBool(CFG_WARNTOSMALLPRICE,m_bWarnToSmallPrice);

	
	AppCfg().SetBool(CFG_WARNTOBIGPROFIT,m_bWarnToBigProfit);
	AppCfg().SetFloat(CFG_WARNMAXPROFIT,m_MaxProfit.GetDouble());

	AppCfg().SetRGB(CFG_WARNTOBIGPROFITCLR,m_WarnToBigProfitClr);

	
	for(int i = 0;i <= MAX_INOUT_GRP;i++)
	{
		AppCfg().SetRGB(CFG_GROUPCOLORS_ITEM,i,m_grpClrCombo.GetItemData(i));
	}

	AppCfg().SetBool(CFG_CHECKFARMNO,m_bCheckFarmNo);
	
	AppCfg().SetNum(CFG_MARKMYPRICELOSSBUY,m_bMarkMyPriceGtLtBuy ? m_nMarkMyPriceGtLtBuy : GTLT_NONE);
	AppCfg().SetNum(CFG_MARKMYPRICELOSSSELL,m_bMarkMyPriceGtLtSell ? m_nMarkMyPriceGtLtSell : GTLT_NONE);


	//CString FPMWRateStr;
	//m_FPMWRateEdit.GetWindowText(FPMWRateStr);
	//if(FPMWRateStr.IsEmpty())
	//{
	//	AfxMessageBox(IDS_NOFPMWRATE);
	//	return FALSE;
	//}

	//m_dFPMWRate = CUtil::myatof(FPMWRateStr)/100;
	//AppCfg().SetFloat(CFGID_FPMWRATE,m_dFPMWRate);

	
	return TRUE;
}

void CMainOpt::OnColorSelEnd() 
{
	CRect rect;
	int i,nItem = m_grpClrCombo.GetCurSel();
	if(nItem < 0)   return;

	COLORREF clr = (COLORREF)m_grpClrCombo.GetItemData(nItem);

	COLORREF custRGB[CUSTOM_COLORS_COUNT];

	for(i = 0;i < CUSTOM_COLORS_COUNT;i++)
	{
		custRGB[i] = AppCfg().GetRGB(CFG_CUSTOMCOLORS_ITEM,i);
	}

	CColorDialog clrdlg(clr, CC_FULLOPEN | CC_ANYCOLOR, this);
	clrdlg.m_cc.lStructSize  = sizeof(CHOOSECOLOR);
	clrdlg.m_cc.hwndOwner    = m_hWnd;
	clrdlg.m_cc.lpCustColors = custRGB;
	clrdlg.m_cc.Flags        |= CC_FULLOPEN | CC_ANYCOLOR;

	if (clrdlg.DoModal() == IDOK)
	{
		clr = clrdlg.GetColor();
		m_grpClrCombo.SetItemData(nItem,clr);		
		m_grpClrCombo.Invalidate();
	}
		
	for(i = 0;i < CUSTOM_COLORS_COUNT;i++)
	{
		AppCfg().SetRGB(CFG_CUSTOMCOLORS_ITEM,i,custRGB[i]);
	}

	


}





