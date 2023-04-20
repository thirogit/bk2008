#ifndef __COWPRICELIST_H__
#define  __COWPRICELIST_H__

#include "../BaseListCtrl.h"
#include "../../datatypes/CowPriceData.h"
#include "../EditCellTemplate.h"
#include "../../datatypes/CowPriceDataArray.h"
#include "../../combo/StockComboCell.h"
#include "MarkWarnShowOptions.h"
#include "../../datatypes/CowPriceDataComparator.h"
#include "../../datatypes/invoice/CowPriceInvoiceSession.h"


#define DISABLED_CLR RGB(192,192,192)
#define CUSTOM_PRICE_CLR RGB(239,134,134)
#define AVERAGE_PRICE_WEIGHT_CLR RGB(0,128,255)
#define ERROR_CLR RGB(255,0,0)
#define CUSTOM_SET_CLR RGB(255,255,0)
#define CUSTOM_PRICE_PER_KG_CLR (RGB(163,73,164))
#define STOCK_PER_KG_PRICE_CLR (RGB(130,231,112))

class CCowPriceList : public CBaseListCtrl
{
public:
	CCowPriceList(const GRIDHEADERS *ppHeaders,UINT pConfigItemId,CowPriceInvoiceSession *pInvoiceSession);
	void UpdateItemCount();
	enum COWPRICELISTCOLS
	{
		COL_HASTERM = COWPRICEDATA_HASTERM,
		COL_EAN = COWPRICEDATA_EAN,
		COL_STOCK = COWPRICEDATA_STOCK,
		COL_WEIGHT = COWPRICEDATA_WEIGHT,
		COL_MYPRICE = COWPRICEDATA_MYPRICE,
		COL_HENT = COWPRICEDATA_HENT,
		COL_OTHERPRICE = COWPRICEDATA_OTHERPRICE,
		COL_RULE = COWPRICEDATA_RULE,
		COL_PRICEPERKG = COWPRICEDATA_PRICEPERKG,
		COL_NETTO = COWPRICEDATA_NETTO,
		COL_VAT = COWPRICEDATA_VAT,
		COL_BRUTTO = COWPRICEDATA_BRUTTO
	};
private:
	//static const GRIDHEADERS szCowPriceListCols[]; 
	CImageList m_starImageList;
	CowPriceInvoiceSession *m_pInvoiceSession;

protected:
	CMarkWarnShowOptions *m_mws_opts;

	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	CComboBox *RuleComboSubItem(int Item, int Column);
	CFloatEditCell* FloatEditSubItem (int Item, int Column,double initValue,int prec = 4);
	CIntEditCell*  IntegerEditSubItem (int Item, int Column,int initValue);
	CComboBox *StockComboSubItem(int Item, int Column);
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	virtual void SortByCol(int Column,bool bAsc);
	virtual void SetListCols();
	int GetItemImage(int iItem,int iSubItem);
	virtual CString GetColText(UINT nColID);
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);

	
	DECLARE_MESSAGE_MAP();
};

#endif