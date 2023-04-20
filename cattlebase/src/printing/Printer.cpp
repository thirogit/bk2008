#include "stdafx.h"
#include "Printer.h"

CPrinter::CPrinter(const CString& sPrinterName,const CString& sPort) : m_sPrinterPort(sPort),m_sPrinterName(sPrinterName)
{
}

CString CPrinter::GetPrinterName() const
{
	return m_sPrinterName;
}

CString CPrinter::GetPrinterPort() const
{
	return m_sPrinterPort;
}

void CPrinter::Print(CCowBasePrintDoc* pDoc,UINT nNumOfCopeis)
{
	DoPrint(pDoc,nNumOfCopeis);
}
