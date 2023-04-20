#ifndef __BARCODESCANNERDRIVER__
#define __BARCODESCANNERDRIVER__

#include "../PtrFlushArray.h"
#include <memory>
#include "scanner.h"

interface IBarcodeReceiver
{
	virtual void OnBarcode(const CString& sBarcode) = 0;
};
class CBarcodeScannerSink : public CWnd
{
public:
	CBarcodeScannerSink();
	bool Create(IBarcodeReceiver *pReceiver);
	bool IsOnline();
	bool Connect();
	bool Disconnect();
private:
	bool RegisterWndClass();
	void OnUSBScannerPluggedIn();
	void OnUSBScannerPluggedOut();
	bool IsUSBScannerPluggedIn();
	bool CompareGUID(const GUID *pGUID1,const GUID *pGUID2);
	bool m_bScannerPluggedIn;

	CScanner m_scanner;
	bool m_bOnline;
	IBarcodeReceiver *m_pReceiver;
	HDEVNOTIFY m_hDevNotify;
protected:
	afx_msg void OnDataEventScanner(long lStatus);
	afx_msg LRESULT OnUSBDeviceChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
	
};


class CBarcodeScannerDriver : public IBarcodeReceiver
{
public:
	static bool IsScannerOnline();
	static bool ConnectToScanner();
	static void DisconnectScanner();
	static void AddReceiver(IBarcodeReceiver *pReceiver);
	static void RemoveReceiver(IBarcodeReceiver *pReceiver);
	~CBarcodeScannerDriver();
protected:
	CBarcodeScannerDriver();
	virtual void OnBarcode(const CString& sBarcode);
	
	static void Create();

	static CBarcodeScannerDriver* m_pInstance;
	static std::auto_ptr<CBarcodeScannerDriver> m_autoPtrInstance;
	static PtrFlushArray<IBarcodeReceiver> m_receivers;
	static CBarcodeScannerSink m_sink;
};


static const GUID GUID_DEVINTERFACE_USB_DEVICE = { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
static const GUID GUID_DEVINTERFACE_HID = 	{ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } };
static const GUID GUID_SYMBOL_BARCODE_SCANNER = {0x745A17A0,0x74D3,0x11D0,{0xB6, 0xFE, 0x00,0xA0,0xC9,0x0F,0x57,0xDA} };
#endif