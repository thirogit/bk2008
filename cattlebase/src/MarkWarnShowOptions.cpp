#include "stdafx.h"
#include "MarkWarnShowOptions.h"
#include "MainCfg.h"
#include "combo/GtLtCombo.h"

CMarkWarnShowOptions CMarkWarnShowOptions::m_instance;

CMarkWarnShowOptions::CMarkWarnShowOptions() 
{
	m_bShowPrice = true;
	m_clrMarkCowAge = RGB(255,255,255);
	m_clrWarnCowAge  = RGB(255,255,255);
	m_clrMarkLoss  = RGB(255,255,255);
	m_iMarkDays = 0;
	m_iWarnDays = 0;
	m_bMarkAge = false;
	m_bWarnAge = false;
	m_bMarkLoss = false;
	m_clrWarnToBigProfit = RGB(128,255,0);
	m_fMaxProfit = 50.0;
	m_bWarnToBigProfit = false;
	m_nMarkMyPriceGtLtBuy = GTLT_NONE;
	m_nMarkMyPriceGtLtSell = GTLT_NONE;
}

CMarkWarnShowOptions* CMarkWarnShowOptions::GetInstance()
{
	return &m_instance;
}

void CMarkWarnShowOptions::Load()
{
	m_bShowPrice = !AppCfg().GetBool(CFG_MASKMYPRICE) == TRUE;
	m_bMarkAge = AppCfg().GetBool(CFG_MARKCOWAGE) == TRUE;
	m_bWarnAge = AppCfg().GetBool(CFG_WARNCOWAGE) == TRUE;
	m_bMarkLoss = AppCfg().GetBool(CFG_MARKBUYSELLLOSS) == TRUE;
	m_bWarnToBigProfit = AppCfg().GetBool(CFG_WARNTOBIGPROFIT) == TRUE;

	m_clrMarkCowAge = AppCfg().GetRGB(CFG_MARKCOWAGECLR);
	m_clrWarnCowAge = AppCfg().GetRGB(CFG_WARNCOWAGECLR);
	m_clrMarkLoss =  AppCfg().GetRGB(CFG_MARKBUYSELLLOSSCLR);
	m_clrWarnToBigProfit = AppCfg().GetRGB(CFG_WARNTOBIGPROFITCLR);

	m_iMarkDays = AppCfg().GetNum(CFG_MARKCOWAGEDAYS);
	m_iWarnDays = AppCfg().GetNum(CFG_WARNCOWAGEDAYS);
	m_fMaxProfit = AppCfg().GetFloat(CFG_WARNMAXPROFIT);

	m_nMarkMyPriceGtLtBuy = AppCfg().GetNum(CFG_MARKMYPRICELOSSBUY);
	m_nMarkMyPriceGtLtSell = AppCfg().GetNum(CFG_MARKMYPRICELOSSSELL);


}
void CMarkWarnShowOptions::SetShowPrices(bool bShow)
{
	m_bShowPrice = bShow;
}
void CMarkWarnShowOptions::SetMarkAge(bool bMarkAge)
{
	m_bMarkAge = bMarkAge;
}
void CMarkWarnShowOptions::SetWarnAge(bool bWarnAge)
{
	m_bWarnAge = bWarnAge;
}
void CMarkWarnShowOptions::SetMarkLoss(bool bMarkLoss)
{
	m_bMarkLoss = bMarkLoss;
}
void CMarkWarnShowOptions::SetWarnToBigProfit(bool bWarnToBigProfit)
{
	m_bWarnToBigProfit = bWarnToBigProfit;
}

void CMarkWarnShowOptions::SetMarkCowAgeColor(COLORREF clrMarkCowAge)
{
	m_clrMarkCowAge = clrMarkCowAge;
}
void CMarkWarnShowOptions::SetWarnCowAgeColor(COLORREF clrWarnCowAge)
{
	m_clrWarnCowAge  = clrWarnCowAge;
}
void CMarkWarnShowOptions::SetMarkLossColor(COLORREF clrMarkLoss)
{
	m_clrMarkLoss = clrMarkLoss;
}
void CMarkWarnShowOptions::SetWarnToBigProfitColor(COLORREF clrToBigProfit)
{
	m_clrWarnToBigProfit = clrToBigProfit;
}

void CMarkWarnShowOptions::SetMarkDays(int iMarkDays)
{
	m_iMarkDays = iMarkDays;
	
}

void CMarkWarnShowOptions::SetWarnDays(int iWarnDays)
{
	m_iWarnDays = iWarnDays;
}

void CMarkWarnShowOptions::SetMaxProfit(Money &maxProfit)
{
	m_fMaxProfit = maxProfit;
}

void CMarkWarnShowOptions::SetMarkMyPriceGtLtBuy(UINT nMarkMyPriceGtLtBuy)
{
	m_nMarkMyPriceGtLtBuy = nMarkMyPriceGtLtBuy;
}
void CMarkWarnShowOptions::SetMarkMyPriceGtLtSell(UINT nMarkMyPriceGtLtSell)
{
	m_nMarkMyPriceGtLtSell = nMarkMyPriceGtLtSell;
}
