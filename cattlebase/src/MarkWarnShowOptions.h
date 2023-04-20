#ifndef __MARKWARNSHOWOPTIONS_H__
#define __MARKWARNSHOWOPTIONS_H__

#include "classes/types/Money.h"
#include "DataTypes.h"

class CMarkWarnShowOptions
{
private:
	bool m_bShowPrice;
	COLORREF m_clrMarkCowAge;
	COLORREF m_clrWarnCowAge;
	COLORREF m_clrMarkLoss;
	COLORREF m_clrWarnToBigProfit;
	
	int      m_iWarnDays;
	bool     m_bMarkAge;
	bool     m_bWarnAge;
	int      m_iMarkDays;
	bool     m_bMarkLoss;
	Money	 m_fMaxProfit;
	bool     m_bWarnToBigProfit;
	UINT	 m_nMarkMyPriceGtLtBuy;
	UINT	 m_nMarkMyPriceGtLtSell;

	CMarkWarnShowOptions();
	static CMarkWarnShowOptions m_instance; 

public:

	static CMarkWarnShowOptions* GetInstance();

	void Load();
	inline bool GetShowPrices()
	{
		return m_bShowPrice && HAS_PRIVLEDGE(INVOICE);
	};

	void SetShowPrices(bool bShow);
	void SetMarkAge(bool bMarkAge);
	void SetWarnAge(bool bWarnAge);
	void SetMarkLoss(bool bMarkLoss);
	void SetWarnToBigProfit(bool bWarnToBigProfit);
	void SetMarkCowAgeColor(COLORREF clrMarkCowAge);
	void SetWarnCowAgeColor(COLORREF clrWarnCowAge);
	void SetMarkLossColor(COLORREF clrMarkLoss);
	void SetWarnToBigProfitColor(COLORREF clrToBigProfit);
	void SetMarkDays(int iMarkDays);
	void SetWarnDays(int iWarnDays);
	void SetMaxProfit(Money &maxProfit);
	void SetMarkMyPriceGtLtBuy(UINT nMarkMyPriceGtLtBuy);
	void SetMarkMyPriceGtLtSell(UINT nMarkMyPriceGtLtSell);

	inline bool GetMarkAge()
	{
		return m_bMarkAge;
	}

	inline UINT GetMarkMyPriceGtLtBuy()
	{
		return m_nMarkMyPriceGtLtBuy;	
	}
	inline UINT GetMarkMyPriceGtLtSell()
	{
		return m_nMarkMyPriceGtLtSell;
	}
	

	inline bool GetWarnAge()
	{
		return m_bWarnAge;
	}

	inline bool GetMarkLoss()
	{
		return m_bMarkLoss;
	}

	inline bool GetWarnToBigProfit()
	{
		return m_bWarnToBigProfit;
	}
	
	inline COLORREF GetMarkCowAgeColor()
	{
		return m_clrMarkCowAge;
	}

	inline COLORREF GetWarnCowAgeColor()
	{
		return m_clrWarnCowAge;
	}
	inline COLORREF GetMarkLossColor()
	{
		return m_clrMarkLoss;
	}
	inline COLORREF  GetWarnToBigProfitColor()
	{
		return m_clrWarnToBigProfit;
	}
	
	inline int GetMarkDays()
	{
		return m_iMarkDays;
	}

	inline int GetWarnDays()
	{
		return m_iWarnDays;
	}

	inline Money& GetMaxProfit()
	{
		return m_fMaxProfit;
	}

	

};

#endif