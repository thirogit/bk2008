#include "stdafx.h"
#include "Printer.h"

CPrinter::CPrinter(const CString& sPrinterName,const CString& sPrinterAddress,PrinterConnectionType connectionType,const CString& sDriverName) : 
		m_sPrinterName(sPrinterName),m_sPrinterAddress(sPrinterAddress),m_ConnectionType(connectionType),m_sDriverName(sDriverName)
{

}
CPrinter::~CPrinter()
{
}

CString CPrinter::GetPrinterName() const
{
	return m_sPrinterName;
}

CString CPrinter::GetPrinterAddress() const
{
	return m_sPrinterAddress;
}

void CPrinter::Print(CBaseDocView* pDocView,UINT nNumOfCopies)
{
	DoPrint(pDocView,nNumOfCopies);
}

CPrinter::PrinterConnectionType CPrinter::GetConnectionType() const
{
	return m_ConnectionType;
}

CString CPrinter::GetDriverName() const
{
	return m_sDriverName;
}