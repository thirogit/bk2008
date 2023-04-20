#ifndef __PRINTER_H__
#define __PRINTER_H__

#include "../BaseDocView.h"



class CPrinter 
{
public:
	enum PrinterConnectionType
	{
		PRINTER_BLUETOOTH = 'B',
		PRINTER_USB = 'U'
	};

public:
	CPrinter(const CString& sPrinterName,const CString& sPrinterAddress,
			PrinterConnectionType connectionType,const CString& sDriverName);
	virtual ~CPrinter();
	CString GetPrinterName() const;
	CString GetPrinterAddress() const;
	PrinterConnectionType GetConnectionType() const;
	CString GetDriverName() const;
	virtual void Print(CBaseDocView* pDocView,UINT nNumOfCopies);
	
protected:
	virtual void DoPrint(CBaseDocView* pDocView,UINT nNumOfCopies) = 0;
private:
	CString m_sPrinterName;
	CString m_sPrinterAddress;
	PrinterConnectionType m_ConnectionType;
	CString m_sDriverName;
};

#endif