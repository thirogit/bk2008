#ifndef __DYMOPRINTERDIALOG_H__
#define __DYMOPRINTERDIALOG_H__

#include "BasePrinterDialog.h"

class CDYMOPrinterDialog : public CBasePrinterDialog
{
public:
	CDYMOPrinterDialog(UINT nNumOfCopies,CWnd* pParent = NULL);	
protected:
	virtual bool GoodPrinter(const CPrinter* pPrinter);
};

#endif 
