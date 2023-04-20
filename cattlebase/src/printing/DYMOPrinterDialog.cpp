#include "stdafx.h"
#include "DYMOPrinterDialog.h"
#include "PrinterFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDYMOPrinterDialog::CDYMOPrinterDialog(UINT nNumOfCopies,CWnd* pParent /*=NULL*/) : CBasePrinterDialog(nNumOfCopies,pParent)
{
}

bool CDYMOPrinterDialog::GoodPrinter(const CPrinter* pPrinter)
{
	return CPrinterFactory::IsDYMOPrinter(pPrinter->GetDriverName());
}

