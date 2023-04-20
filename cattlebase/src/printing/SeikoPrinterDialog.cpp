#include "stdafx.h"
#include "SeikoPrinterDialog.h"
#include "PrinterFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSeikoPrinterDialog::CSeikoPrinterDialog(UINT nNumOfCopies,CWnd* pParent /*=NULL*/) : CBasePrinterDialog(nNumOfCopies,pParent)
{
}

bool CSeikoPrinterDialog::GoodPrinter(const CPrinter* pPrinter)
{
	return CPrinterFactory::IsSeikoPrinter(pPrinter->GetDriverName()) || CPrinterFactory::IsSeikoBluetoothPrinter(pPrinter->GetPrinterName());
}