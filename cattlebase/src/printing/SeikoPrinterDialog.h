#ifndef __SEIKOPRINTERDIALOG_H__
#define __SEIKOPRINTERDIALOG_H__

#include "BasePrinterDialog.h"

class CSeikoPrinterDialog : public CBasePrinterDialog
{
public:
	CSeikoPrinterDialog(UINT nNumOfCopies,CWnd* pParent = NULL);	
protected:
	virtual bool GoodPrinter(const CPrinter* pPrinter); 
};

#endif 
