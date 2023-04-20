#ifndef __ZBIOROWKAFVDOC_H__
#define __ZBIOROWKAFVDOC_H__

#include "../printing/CowDrwDoc.h"
#include "global_arrays.h"

#define ZBIOROWKAFVPAGESIZE 12

class CZbiorowkaFVDoc : public CCowDrwDoc
{
protected:
	CZbiorowkaFVDoc(BuyOrSell buy_sell);
public:
	int GetPrintPages() const;
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
	void PrintPage(int page);
	
private:
	CInvoiceDataArray m_invs2print;
	CInvoiceDataArray invs;
	CDateTime date_from;
	CDateTime date_to;
	BuyOrSell buyOrsell;
	Money totalNetto;
	Money totalVAT;	
	int	  totalCows;
	int starti;
	int endi;
	int m_nPages;

};

class CZbiorowkaFVBuyDoc : public CZbiorowkaFVDoc
{
	DECLARE_DYNCREATE(CZbiorowkaFVBuyDoc)
public:
	CZbiorowkaFVBuyDoc();
};

class CZbiorowkaFVSellDoc : public CZbiorowkaFVDoc
{
	DECLARE_DYNCREATE(CZbiorowkaFVSellDoc)
public:
	CZbiorowkaFVSellDoc();
};

#endif
