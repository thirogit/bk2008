#ifndef __DYMOPRINTER_H__
#define __DYMOPRINTER_H__

#include "../CowBasePrintDoc.h"
#include "Printer.h"

class CDYMOPrinter : public CPrinter
{
public:
	CDYMOPrinter(const CString& sPrinterName,const CString& sPrinterAddress,const CString& sDriverName);
	virtual ~CDYMOPrinter();
protected:
	virtual void DoPrint(CBaseDocView* pDocView,UINT nNumOfCopies);
};

#endif