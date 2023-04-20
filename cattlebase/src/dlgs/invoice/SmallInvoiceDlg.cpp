#include "stdafx.h"
#include "SmallInvoiceDlg.h"
#include "../../ctrls/CowPriceList/CowPriceList.h"
#include "MainCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const GRIDHEADERS SmallInvoiceDlg::szCowPriceListCols[] =
{	
	{IDS_HEADEREAN,			CCowPriceList::COL_EAN},
	{IDS_HEADERSTOCK,		CCowPriceList::COL_STOCK},
	{IDS_HEADERWEIGHT,		CCowPriceList::COL_WEIGHT},	
	{IDS_HEADERRULE,		CCowPriceList::COL_RULE},
	{IDS_HEADERPRICEPERKG,	CCowPriceList::COL_PRICEPERKG},
	{IDS_HEADERNETTO,		CCowPriceList::COL_NETTO},
	{IDS_HEADERVATVALUE,	CCowPriceList::COL_VAT},
	{IDS_HEADERBRUTTO,		CCowPriceList::COL_BRUTTO},
	{0,0}
};


SmallInvoiceDlg::SmallInvoiceDlg(CowPriceInvoiceSession* m_pInvoiceSession,CWnd* pParent/* = NULL*/) : 
	CBaseInvoiceDlg(SmallInvoiceDlg::IDD,
					m_pInvoiceSession,
					new InvoiceUISettings(CFGID_SMALLINVCOWSLIST,szCowPriceListCols),
					pParent)	
{
}

SmallInvoiceDlg::~SmallInvoiceDlg()
{
	delete m_pUISettings;
}
