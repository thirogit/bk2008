#ifndef __COWINVOICEENTRY_H__
#define __COWINVOICEENTRY_H__

#include "Cow.h"

class Invoice;

class CowInvoiceEntry
{
public:
	CowInvoiceEntry(Cow* pCow,Invoice* pInvoice);
	Stock*			GetStock() const;
	Weight			GetWeight() const;
	Money			GetPrice() const;
	Cow*			GetCow() const;
	Invoice*		GetInvoice() const;

	void			SetStock(Stock* invStock);
	void			SetWeight(const Weight& weight);
	void			SetPrice(double dPrice);
	
	bool			InsertThis(bool bCommit = true);
	bool			DeleteThis(bool bCommit = true);

protected:
	Invoice*	m_pInvoice;
	Cow*		m_pCow;
	Stock*		m_pInvStock;
	Weight		m_InvWeight;
	Money		m_InvPrice;
};

#endif