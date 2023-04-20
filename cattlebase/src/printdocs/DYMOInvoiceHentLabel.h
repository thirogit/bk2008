#ifndef __DYMOINVOICEHENTLABEL_H__
#define __DYMOINVOICEHENTLABEL_H__

#include "DYMOBaseHentLabel.h"

class CDYMOInvoiceHentLabel : public CDYMOBaseHentLabel
{
	DECLARE_DYNCREATE(CDYMOInvoiceHentLabel)
public:
	CDYMOInvoiceHentLabel();
	virtual int LoadInputData(CPtrArray &docInputData);
	

};

#endif