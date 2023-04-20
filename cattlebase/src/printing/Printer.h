#ifndef __PRINTER_H__
#define __PRINTER_H__


class CPrinter 
{
public:
	CPrinter(const CString& sPrinterName,const CString& sPort);
	CString GetPrinterName() const;
	CString GetPrinterPort() const;

	virtual ~CPrinter() = 0;
	virtual void Print(CCowBasePrintDoc* pDoc,UINT nNumOfCopeis);
protected:
	virtual void DoPrint(CCowBasePrintDoc* pDoc,UINT nNumOfCopeis) = 0;
private:
	CString m_sPrinterName;
	CString m_sPrinterPort;
};

#endif