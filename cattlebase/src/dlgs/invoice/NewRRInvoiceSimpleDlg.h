#if 0
#ifndef __NEWRRINVOICEDLG_H__
#define __NEWRRINVOICEDLG_H__

#include "BaseNewInvoiceDlg.h"

class CNewRRInvoiceDlg : public CBaseNewInvoiceDlg
{
public:
	CNewRRInvoiceDlg(CCowArray &cowArray,InvoiceContext* pInvoiceContext,CWnd* pParent = NULL);   

protected:

	CCowArray m_baseCows;
	Doc* m_RRInDoc;

protected:
	virtual BOOL OnInitDialog();
	virtual CString GetInvDlgCaption();
	virtual void TabChanged(int newTabIndex);
	
	virtual void AddItem();
	virtual void DelItem();
	
	virtual bool CommitInvoice();
	
	virtual bool UpdateCowsWithInvoice(CCowInvoiceEntryArray &cowInvEntries);

	
	
	virtual double GetPreselectedVATRate();
	virtual InvoiceData::PAYWAYS GetPreselectedPayWay();
	virtual int GetPreSelectedPayDueDays();

	


};

#endif
#endif