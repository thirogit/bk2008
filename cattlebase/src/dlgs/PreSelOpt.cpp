#include "stdafx.h"
#include "PreSelOpt.h"
#include "MainCfg.h"
#include "../global_arrays.h"

// CPreSelOpt dialog

IMPLEMENT_DYNAMIC(CPreSelOpt, CPropertyPage)
CPreSelOpt::CPreSelOpt()
	: CPropertyPage(CPreSelOpt::IDD),m_inreasons(Reason::REASON_IN),m_outreasons(Reason::REASON_OUT)
{
	m_inreasonid = AppCfg().GetNum(CFG_PRESELINREASON);
	m_outreasonid = AppCfg().GetNum(CFG_PRESELOUTREASON);
	m_henttypeid = AppCfg().GetNum(CFG_PRESELHENTTYPE);
	m_cowsex = (COWSEX)AppCfg().GetNum(CFG_PRESELSEX);
	m_stock = ary_stocks.GetStockDataByStockId(AppCfg().GetNum(CFG_PRESELSTOCK));

	m_setinreason = m_inreasonid != 0;
	m_setoutreason = m_outreasonid != 0;
//	m_setsexes;
	m_sethenttypes = m_henttypeid != 0;
	m_setstocks = m_stock != NULL;

	m_VATRateVAT = AppCfg().GetFloat(CFG_PRESELVATRATEVATINV);
	m_paywayVAT = (Invoice::PAYWAYS)AppCfg().GetNum(CFG_PRESELPAYWAYVATINV);
	m_setpaywayVAT = m_paywayVAT != 0;

	m_VATRateRR = AppCfg().GetFloat(CFG_PRESELVATRATERRINV);
	m_paywayRR = (Invoice::PAYWAYS)AppCfg().GetNum(CFG_PRESELPAYWAYRRINV);
	m_setpaywayRR = m_paywayRR != 0;

	m_payduedaysVAT = AppCfg().GetNum(CFG_PRESELPAYDUEDAYSVAT);
	m_payduedaysRR = AppCfg().GetNum(CFG_PRESELPAYDUEDAYSRR);
}

CPreSelOpt::~CPreSelOpt()
{
}

void CPreSelOpt::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SELINREASON,m_inreasons);
	DDX_Control(pDX,IDC_SELOUTREASON,m_outreasons);
	DDX_Control(pDX,IDC_SELSEX,m_sexes);
	DDX_Control(pDX,IDC_SELHTYPE,m_henttypes);
	DDX_Control(pDX,IDC_SELSTOCK,m_stocks);

	DDX_Check(pDX,IDC_CHECKSELINREASON,m_setinreason);
	DDX_Check(pDX,IDC_CHECKSELOUTREASON,m_setoutreason);
	
	DDX_Check(pDX,IDC_CHECKSELHTYPE,m_sethenttypes);
	DDX_Check(pDX,IDC_CHECKSELSTOCK,m_setstocks);

	DDX_Control(pDX,IDC_PRESELVATRATEVAT,m_vatVAT);
	DDX_Control(pDX,IDC_PRESELPAYWAYVAT,m_paywayVATcombo);
	DDX_Check(pDX,IDC_CHECKSELPAYWAYVAT,m_setpaywayVAT);


	DDX_Control(pDX,IDC_PRESELVATRATERR,m_vatRR);
	DDX_Control(pDX,IDC_PRESELPAYWAYRR,m_paywayRRcombo);
	DDX_Check(pDX,IDC_CHECKSELPAYWAYRR,m_setpaywayRR);

	DDX_Control(pDX,IDC_PAYDUEDAYSVATSPIN,m_spnPayDueDaysVAT);
	
	DDX_Control(pDX,IDC_PAYDUEDAYSRRAPIN,m_spnPayDueDaysRR);
}


BEGIN_MESSAGE_MAP(CPreSelOpt, CPropertyPage)

	//ON_NOTIFY(EN_EDITCOMPLETE,IDC_PRESELVATRATE,OnVATComplete)
	//ON_CBN_SELCHANGE(IDC_PRESELVATRATE,OnVATChange)

	ON_BN_CLICKED(IDC_CHECKSELINREASON,UpdateControls)
	ON_BN_CLICKED(IDC_CHECKSELOUTREASON,UpdateControls)
	ON_BN_CLICKED(IDC_CHECKSELHTYPE,UpdateControls)
	ON_BN_CLICKED(IDC_CHECKSELSTOCK,UpdateControls)
	ON_BN_CLICKED(IDC_CHECKSELPAYWAYVAT,UpdateControls)
	ON_BN_CLICKED(IDC_CHECKSELPAYWAYRR,UpdateControls)
	ON_CBN_SELCHANGE(IDC_SELSTOCK, OnStockSelChange)
END_MESSAGE_MAP()


BOOL CPreSelOpt::OnInitDialog()
{
	if(!CPropertyPage::OnInitDialog()) return FALSE;

	m_inreasons.ReloadCombo(m_inreasonid);

	m_outreasons.ReloadCombo(m_outreasonid);

	m_sexes.ReloadCombo(m_cowsex);

	m_henttypes.ReloadCombo(ary_htypes.GetHentTypeByHentTypeId(m_henttypeid));

	m_stocks.ReloadCombo(m_stock);

	UpdateControls();

	m_vatVAT.SetVATRate(m_VATRateVAT);
	m_paywayVATcombo.ReloadCombo(m_paywayVAT);

	m_vatRR.SetVATRate(m_VATRateRR);
	m_paywayRRcombo.ReloadCombo(m_paywayRR);


	m_spnPayDueDaysVAT.SetRange(0,MAX_PAYDUEDAYS);
	m_spnPayDueDaysVAT.SetPos(m_payduedaysVAT);

	m_spnPayDueDaysRR.SetRange(0,MAX_PAYDUEDAYS);
	m_spnPayDueDaysRR.SetPos(m_payduedaysRR);

	return TRUE;
}


void CPreSelOpt::UpdateControls()
{
	UpdateData();
	m_inreasons.EnableWindow(m_setinreason);
	m_outreasons.EnableWindow(m_setoutreason);
	m_henttypes.EnableWindow(m_sethenttypes);
	m_stocks.EnableWindow(m_setstocks);
	m_paywayVATcombo.EnableWindow(m_setpaywayVAT);
	m_paywayRRcombo.EnableWindow(m_setpaywayRR);	
}


void CPreSelOpt::OnVATChange()
{
//	m_VATRate = m_vat.GetCurVATRate();
}

void CPreSelOpt::OnVATComplete(NMHDR* pNMHDR, LRESULT* pResult)
{
//	m_VATRate = m_vat.GetCurVATRate();
	*pResult = 0;
}

BOOL CPreSelOpt::OnApply()
{

	AppCfg().SetNum(CFG_PRESELSEX,(int)m_sexes.GetSelSex());

	int id;

	Stock *stock;
	if(m_setstocks && (stock = m_stocks.GetSelItemData()))
		 id = stock->GetId();
	else id = 0;
	
	AppCfg().SetNum(CFG_PRESELSTOCK,id);


	HentType *htype;
	if(m_sethenttypes &&(htype = m_henttypes.GetSelItemData()))
		id = htype->henttypeid;
	else id = 0;
		AppCfg().SetNum(CFG_PRESELHENTTYPE,id);

	Reason *reason;
	if(m_setinreason &&(reason = m_inreasons.GetSelReason()))
		id = reason->GetId();
	else id = 0;
    	AppCfg().SetNum(CFG_PRESELINREASON,id);


	if(m_setoutreason && (reason = m_outreasons.GetSelReason()))
		id = reason->GetId();
	else id = 0;

	AppCfg().SetNum(CFG_PRESELOUTREASON,id);

	
	m_VATRateVAT = m_vatVAT.GetVATRate().GetDouble();
	m_VATRateRR = m_vatRR.GetVATRate().GetDouble();

	AppCfg().SetFloat(CFG_PRESELVATRATEVATINV,m_VATRateVAT);
	AppCfg().SetFloat(CFG_PRESELVATRATERRINV,m_VATRateRR);

	
	if(m_setpaywayVAT) 
		SetPayWayCfgItem(m_paywayVATcombo,CFG_PRESELPAYWAYVATINV);
	
	if(m_setpaywayRR) 
		SetPayWayCfgItem(m_paywayRRcombo,CFG_PRESELPAYWAYRRINV);
	
	m_payduedaysVAT = m_spnPayDueDaysVAT.GetPos();	
	m_payduedaysRR = m_spnPayDueDaysRR.GetPos();
	
	AppCfg().SetNum(CFG_PRESELPAYDUEDAYSVAT,m_payduedaysVAT);
	AppCfg().SetNum(CFG_PRESELPAYDUEDAYSRR,m_payduedaysRR);

	return TRUE;
}

void CPreSelOpt::SetPayWayCfgItem(CPaywayCombo& payWayCombo,UINT cfgItemId)
{
	int curPayway = payWayCombo.GetCurSel();	
	int payway = 0;
	if(curPayway >= 0)
		payway = payWayCombo.GetItemData(curPayway);
	AppCfg().SetNum(cfgItemId,payway);
}

void CPreSelOpt::OnStockSelChange()
{
	int curSel = m_stocks.GetCurSel();
	if(curSel >= 0)
	{
		Stock *pStock = (Stock*)m_stocks.GetItemData(curSel);
		if(!pStock->GetPredefSex().IsUNK())
		{
			AfxMessageBox(IDS_PREDEFSEXWARNING,MB_OK,0);
		}
	}
}
