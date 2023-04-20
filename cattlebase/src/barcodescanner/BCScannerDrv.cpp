#include "stdafx.h"
#include "BCScannerDrv.h"
#include <dbt.h>
#include <setupapi.h>
#include "../log.h"

#define ID_SCANNER 1001
#define CLASSNAME "Barcode_Sink"

BEGIN_EVENTSINK_MAP(CBarcodeScannerSink, CWnd)
  
	ON_EVENT(CBarcodeScannerSink, ID_SCANNER, 1 /* DataEvent */, OnDataEventScanner, VTS_I4)
	
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(CBarcodeScannerSink, CWnd)
	ON_MESSAGE(WM_DEVICECHANGE, OnUSBDeviceChange)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



CBarcodeScannerSink::CBarcodeScannerSink() : m_bOnline(false),m_pReceiver(NULL)
{
	m_bScannerPluggedIn = IsUSBScannerPluggedIn();
}

bool CBarcodeScannerSink::RegisterWndClass()
{
	
	WNDCLASS wc;
	wc.lpszClassName = CLASSNAME; 
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = NULL;
	wc.hbrBackground = (HBRUSH) ::GetStockObject(COLOR_WINDOW);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_SAVEBITS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	return (::RegisterClass(&wc) != 0);
}

bool CBarcodeScannerSink::Create(IBarcodeReceiver *pReceiver)
{

	RegisterWndClass();
	if(!CWnd::Create(CLASSNAME,"",WS_OVERLAPPED,CRect(),AfxGetMainWnd(),0,NULL))
	{
		LOG("FAILD TO CREATE BARCODE SINK WINDOW");
		return FALSE;
	}

	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

	NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
	m_hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(),&NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if( !m_hDevNotify ) 
	{
		LOG("FAILD TO REGISTER DEVICE NOTIFICATION.");
		DestroyWindow();
		return FALSE;
	}

	

	m_pReceiver = pReceiver;
	if(m_scanner.Create("","SCANNER",0,CRect(),this,ID_SCANNER) &&
		m_scanner.Open("STI_USBSCANNER") == 0)
	{
		return true;
	}
	LOG("UNABLE TO SETUP OLE SCANNER.");
	DestroyWindow();
	return false;
}

void CBarcodeScannerSink::OnDestroy()
{
	if(m_scanner.GetControlUnknown() != NULL)
	{
		m_scanner.Close();
	}
	if(m_hDevNotify) UnregisterDeviceNotification(m_hDevNotify);
}

bool CBarcodeScannerSink::IsOnline()
{
	return m_bOnline;
}
bool CBarcodeScannerSink::Connect()
{
	if(m_scanner.GetControlUnknown() != NULL)
	{
		m_bOnline = m_scanner.Claim(0) == 0;
		if(m_bOnline)
		{
			m_scanner.SetDeviceEnabled(TRUE);
			m_scanner.SetDataEventEnabled(TRUE);
			m_scanner.SetFreezeEvents(FALSE);
		}
		return m_bOnline; 
	}
	return false;
}
bool CBarcodeScannerSink::Disconnect()
{
	if(m_bOnline)
	{
		m_bOnline = false;
		return m_scanner.Release() == 0;
	}
	return false;
}

void CBarcodeScannerSink::OnDataEventScanner(long lStatus)
{
	if(m_pReceiver)
		m_pReceiver->OnBarcode(m_scanner.GetScanData());

	m_scanner.SetDataEventEnabled(TRUE);
}

void CBarcodeScannerSink::OnUSBScannerPluggedIn()
{
	Connect();
}
void CBarcodeScannerSink::OnUSBScannerPluggedOut()
{
	Disconnect();
}

bool CBarcodeScannerSink::IsUSBScannerPluggedIn()
{
	DWORD dwFlag = (DIGCF_PRESENT);
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_SYMBOL_BARCODE_SCANNER,"USB",NULL,dwFlag);
	SP_DEVINFO_DATA spDevInfoData;
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for(int i=0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++) 
	{
		DWORD nSize=0 ;
		TCHAR buf[MAX_PATH];

		if (SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, buf, sizeof(buf), &nSize) ) 
		{
			if(CompareGUID(&spDevInfoData.ClassGuid,&GUID_SYMBOL_BARCODE_SCANNER))
			{
				return true;
			}
		} 
	}
	return false;

}


bool CBarcodeScannerSink::CompareGUID(const GUID *pGUID1,const GUID *pGUID2)
{
	if(pGUID1->Data1 == pGUID2->Data1 &&
		pGUID1->Data2 == pGUID2->Data2 &&
		pGUID1->Data3 == pGUID2->Data3)
	{
		for(int i = 0;i < sizeof(pGUID1->Data4);i++)
		{
			if(pGUID1->Data4[i] != pGUID2->Data4[i])
				return false;
		}
		return true;
	}
	return false;
}


LRESULT CBarcodeScannerSink::OnUSBDeviceChange(WPARAM wParam, LPARAM lParam)
{
    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;

	switch(wParam)
	{
		case DBT_DEVICEARRIVAL:
			if(pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				if(IsUSBScannerPluggedIn() && !m_bScannerPluggedIn)
				{
					OnUSBScannerPluggedIn();
					m_bScannerPluggedIn = true;
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			if(pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				if(!IsUSBScannerPluggedIn() && m_bScannerPluggedIn)
				{
					OnUSBScannerPluggedOut();	
					m_bScannerPluggedIn = false;
				}
			}
			break;
	}
	
    return (LRESULT)TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////
CBarcodeScannerDriver* CBarcodeScannerDriver::m_pInstance;
std::auto_ptr<CBarcodeScannerDriver> CBarcodeScannerDriver::m_autoPtrInstance;
PtrFlushArray<IBarcodeReceiver> CBarcodeScannerDriver::m_receivers;
CBarcodeScannerSink  CBarcodeScannerDriver::m_sink;

bool CBarcodeScannerDriver::IsScannerOnline()
{
	CBarcodeScannerDriver::Create();
	return CBarcodeScannerDriver::m_sink.IsOnline();

}
bool CBarcodeScannerDriver::ConnectToScanner()
{
	Create();
	return CBarcodeScannerDriver::m_sink.Connect();
}
void CBarcodeScannerDriver::DisconnectScanner()
{
	Create();
	CBarcodeScannerDriver::m_sink.Disconnect();
}
void CBarcodeScannerDriver::AddReceiver(IBarcodeReceiver *pReceiver)
{
	CBarcodeScannerDriver::m_receivers.Add(pReceiver);
}
void CBarcodeScannerDriver::RemoveReceiver(IBarcodeReceiver *pReceiver)
{
	CBarcodeScannerDriver::m_receivers.RemovePtr(pReceiver);
}

CBarcodeScannerDriver::~CBarcodeScannerDriver()
{
	CBarcodeScannerDriver::DisconnectScanner();
}

CBarcodeScannerDriver::CBarcodeScannerDriver()
{
}

void CBarcodeScannerDriver::Create()
{
	if(CBarcodeScannerDriver::m_pInstance == NULL)
	{
		CBarcodeScannerDriver::m_pInstance = new CBarcodeScannerDriver();
		CBarcodeScannerDriver::m_autoPtrInstance.reset(m_pInstance);
		m_sink.Create(CBarcodeScannerDriver::m_pInstance);
	}
}

void CBarcodeScannerDriver::OnBarcode(const CString& sBarcode)
{
	PtrFlushArray<IBarcodeReceiver>::ArrayIterator it = CBarcodeScannerDriver::m_receivers.iterator();
	while(it.hasNext())
	{
		(*(it++))->OnBarcode(sBarcode);
	}
	
}