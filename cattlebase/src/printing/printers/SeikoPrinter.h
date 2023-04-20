#ifndef __SEIKOPRINTER_H__
#define __SEIKOPRINTER_H__

#include "Printer.h"
#include <FreeImagePlus.h>


class CSeikoPrinter : public CPrinter
{
public:
	CSeikoPrinter(const CString& sPrinterName,const CString& sPrinterPort,PrinterConnectionType connectionType,const CString& sDriverName);
	virtual ~CSeikoPrinter();
	UINT GetPageWidth() const;
protected:
	virtual void DoPrint(CBaseDocView* pDocView,UINT nNumOfCopeis);
	virtual DWORD WritePrinter(BYTE *pBuffer,DWORD bytesToWrite) = 0;
	bool WriteImage(fipImage& image);
	void RotateBestFit(fipImage& image);
	void FeedPaper(short px);
};

#endif