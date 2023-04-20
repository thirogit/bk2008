#ifndef __SMALLINVOICEDLG_H__
#define __SMALLINVOICEDLG_H__


#include "../../datatypes/invoice/CowPriceInvoiceSession.h"
#include "BaseInvoiceDlg.h"
#include "InvoiceUISettings.h"

class SmallInvoiceDlg : public CBaseInvoiceDlg
{
public:
	SmallInvoiceDlg(CowPriceInvoiceSession* m_pInvoiceSession,CWnd* pParent = NULL);   
	virtual ~SmallInvoiceDlg();
	enum { IDD = IDD_SMALLINVOICE };
protected:
	static const GRIDHEADERS szCowPriceListCols[];

	

};

#endif