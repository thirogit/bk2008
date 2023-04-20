#include "stdafx.h"
#include "DYMOPrinter.h"
#include "../../printdocs/DYMOBaseDoc.h"

CDYMOPrinter::CDYMOPrinter(const CString& sPrinterName,const CString& sPrinterAddress,const CString& sDriverName) :
CPrinter(sPrinterName,sPrinterAddress,PRINTER_USB,sDriverName)
{
}

CDYMOPrinter::~CDYMOPrinter()
{

}

void CDYMOPrinter::DoPrint(CBaseDocView* pDocView,UINT nNumOfCopies)
{
	CDYMOBaseDoc* pDYMODoc = (CDYMOBaseDoc*)pDocView->GetDocument();
	pDYMODoc->Print(GetPrinterName(),GetPrinterAddress(),nNumOfCopies);
	
}