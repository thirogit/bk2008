#ifndef __SEIKOBLUETOOTHPRINTER_H__
#define __SEIKOBLUETOOTHPRINTER_H__

#include "SeikoPrinter.h"
#include "../../BtInterface/Bluetooth.h"

class CSeikoBluetoothPrinter : public CSeikoPrinter
{
public:
	CSeikoBluetoothPrinter(const CString& sPrinterName,const CString& sPrinterAddress,const CString& sDriverName);
	virtual ~CSeikoBluetoothPrinter();
protected:
	virtual DWORD WritePrinter(BYTE *pBuffer,DWORD bytesToWrite);
	virtual void DoPrint(CBaseDocView* pDocView,UINT nNumOfCopies);
	//BOOL CloseCOMPort();
	//BOOL OpenCOMPort(const CString& portName);
	//BOOL BtAddrStrToBtAddr(const CString& sBtAddrStr,BTUINT8* btAddr);
protected:
	BtSerialSocket m_SPPSocket;
};

#endif