#include "stdafx.h"
#include "BlueSoleilInterface.h"

CBlueSoleil::CBlueSoleil()
{	
	fnBtsdk_IsServerConnected = NULL;
	fnBtsdk_Init = NULL;
	fnBtsdk_IsBluetoothHardwareExisted = NULL;
	fnBtsdk_IsBluetoothReady = NULL;
	fnBtsdk_StartBluetooth = NULL;
	fnBtsdk_SetLocalDeviceClass = NULL;

	fnBtsdk_StartEnumRemoteDevice = NULL;
	fnBtsdk_EnumRemoteDevice = NULL;
	fnBtsdk_GetRemoteDeviceClass = NULL;
	fnBtsdk_GetRemoteDeviceName = NULL;
	fnBtsdk_UpdateRemoteDeviceName = NULL;
	fnBtsdk_GetRemoteDeviceAddress = NULL;
	fnBtsdk_EndEnumRemoteDevice = NULL;
	m_hBlueSoleilDLL = NULL;
}

bool CBlueSoleil::LoadBlueSoleil()
{
	
	struct BLUESOLEILEXPORT
	{
		FARPROC* fnExport;
		const char* szExportFnName;
	} _bluesoleilExports[] =
	{
		{(FARPROC*)&fnBtsdk_IsServerConnected, "Btsdk_IsServerConnected"},
		{(FARPROC*)&fnBtsdk_Init, "Btsdk_Init"},
		{(FARPROC*)&fnBtsdk_IsBluetoothHardwareExisted, "Btsdk_IsBluetoothHardwareExisted"},
		{(FARPROC*)&fnBtsdk_IsBluetoothReady, "Btsdk_IsBluetoothReady"},
		{(FARPROC*)&fnBtsdk_StartBluetooth, "Btsdk_StartBluetooth"},
		{(FARPROC*)&fnBtsdk_SetLocalDeviceClass, "Btsdk_SetLocalDeviceClass"},
		{(FARPROC*)&fnBtsdk_StartEnumRemoteDevice, "Btsdk_StartEnumRemoteDevice"},
		{(FARPROC*)&fnBtsdk_EnumRemoteDevice, "Btsdk_EnumRemoteDevice"},
		{(FARPROC*)&fnBtsdk_GetRemoteDeviceClass, "Btsdk_GetRemoteDeviceClass"},
		{(FARPROC*)&fnBtsdk_GetRemoteDeviceName, "Btsdk_GetRemoteDeviceName"},
		{(FARPROC*)&fnBtsdk_UpdateRemoteDeviceName, "Btsdk_UpdateRemoteDeviceName"},
		{(FARPROC*)&fnBtsdk_GetRemoteDeviceAddress, "Btsdk_GetRemoteDeviceAddress"},
		{(FARPROC*)&fnBtsdk_EndEnumRemoteDevice, "Btsdk_EndEnumRemoteDevice"},
		{(FARPROC*)&fnBtsdk_GetRemoteDeviceHandle, "Btsdk_GetRemoteDeviceHandle"},
		{(FARPROC*)&fnBtsdk_GetASerialNum, "Btsdk_GetASerialNum"},
		{(FARPROC*)&fnBtsdk_PlugInVComm, "Btsdk_PlugInVComm"},
		{(FARPROC*)&fnBtsdk_InitCommObj, "Btsdk_InitCommObj"},
		{(FARPROC*)&fnBtsdk_ConnectEx, "Btsdk_ConnectEx"},
		{(FARPROC*)&fnBtsdk_Disconnect, "Btsdk_Disconnect"},
		{(FARPROC*)&fnBtsdk_DeinitCommObj, "Btsdk_DeinitCommObj"},
		{(FARPROC*)&fnBtsdk_PlugOutVComm, "Btsdk_PlugOutVComm"},
		{(FARPROC*)&fnBtsdk_Done, "Btsdk_Done"},
		{NULL, NULL},
	};


	m_hBlueSoleilDLL = LoadLibrary("BSSDK.dll");
	if(m_hBlueSoleilDLL != NULL)
	{
		BLUESOLEILEXPORT* pBSExport = _bluesoleilExports;
		while(pBSExport->fnExport != NULL)
		{
			(*pBSExport->fnExport) = GetProcAddress(m_hBlueSoleilDLL,pBSExport->szExportFnName);
			if((*pBSExport->fnExport) == NULL)
			{
				FreeLibrary(m_hBlueSoleilDLL);
				return false;
			}
			pBSExport++;
		}
		return true;
	}	
	return false;
}
CBlueSoleil::~CBlueSoleil()
{
	if(m_hBlueSoleilDLL)
		FreeLibrary(m_hBlueSoleilDLL);
}

BTBOOL CBlueSoleil::Btsdk_IsServerConnected()
{
	if(fnBtsdk_IsServerConnected)
		return fnBtsdk_IsServerConnected();
	return BTSDK_FALSE;
}
BTINT32 CBlueSoleil::Btsdk_Init(void)
{
	if(fnBtsdk_Init)
		return fnBtsdk_Init();

	return 0;

}
BTBOOL CBlueSoleil::Btsdk_IsBluetoothHardwareExisted()
{
	if(fnBtsdk_IsBluetoothHardwareExisted)
		return fnBtsdk_IsBluetoothHardwareExisted();
	return BTSDK_FALSE;
}

BTBOOL CBlueSoleil::Btsdk_IsBluetoothReady(void)
{
	if(fnBtsdk_IsBluetoothReady)
		return fnBtsdk_IsBluetoothReady();
	return BTSDK_FALSE;
}
BTINT32 CBlueSoleil::Btsdk_StartBluetooth(void)
{
	if(fnBtsdk_StartBluetooth)
		return fnBtsdk_StartBluetooth();

	return 0;
}
BTINT32 CBlueSoleil::Btsdk_SetLocalDeviceClass(BTUINT32 device_class)
{
	if(fnBtsdk_SetLocalDeviceClass)
		return fnBtsdk_SetLocalDeviceClass(device_class);

	return 0;
}
BTSDKHANDLE CBlueSoleil::Btsdk_StartEnumRemoteDevice(BTUINT32 flag, BTUINT32 dev_class)
{
	if(fnBtsdk_StartEnumRemoteDevice)
		return fnBtsdk_StartEnumRemoteDevice(flag,dev_class);

	return BTSDK_INVALID_HANDLE;
}
BTDEVHDL CBlueSoleil::Btsdk_EnumRemoteDevice(BTSDKHANDLE enum_handle, PBtSdkRemoteDevicePropertyStru rmt_dev_prop)
{
	if(fnBtsdk_EnumRemoteDevice)
		return fnBtsdk_EnumRemoteDevice(enum_handle,rmt_dev_prop);

	return BTSDK_INVALID_HANDLE;
}

BTINT32 CBlueSoleil::Btsdk_GetRemoteDeviceClass(BTDEVHDL dev_hdl, BTUINT32 *pdevice_class)
{
	if(fnBtsdk_GetRemoteDeviceClass)
		return fnBtsdk_GetRemoteDeviceClass(dev_hdl,pdevice_class);

	return 0;
}

BTINT32 CBlueSoleil::Btsdk_GetRemoteDeviceName(BTDEVHDL dev_hdl, BTUINT8 *name, BTUINT16 *plen)
{
	if(fnBtsdk_GetRemoteDeviceName)
		return fnBtsdk_GetRemoteDeviceName(dev_hdl,name,plen);

	return 0;
}

BTINT32 CBlueSoleil::Btsdk_UpdateRemoteDeviceName(BTDEVHDL dev_hdl, BTUINT8 *name, BTUINT16 *plen)
{
	if(fnBtsdk_UpdateRemoteDeviceName)
		return fnBtsdk_UpdateRemoteDeviceName(dev_hdl,name,plen);

	return 0;
}

BTINT32 CBlueSoleil::Btsdk_GetRemoteDeviceAddress(BTDEVHDL dev_hdl, BTUINT8 *bd_addr)
{
	if(fnBtsdk_GetRemoteDeviceAddress)
		return fnBtsdk_GetRemoteDeviceAddress(dev_hdl,bd_addr);

	return 0;
}

BTINT32 CBlueSoleil::Btsdk_EndEnumRemoteDevice(BTSDKHANDLE enum_handle)
{
	if(fnBtsdk_EndEnumRemoteDevice)
		return fnBtsdk_EndEnumRemoteDevice(enum_handle);

	return 0;
}

BTDEVHDL CBlueSoleil::Btsdk_GetRemoteDeviceHandle(BTUINT8 *bd_addr)
{
	if(fnBtsdk_GetRemoteDeviceHandle)
		return fnBtsdk_GetRemoteDeviceHandle(bd_addr);
	return BTSDK_INVALID_HANDLE;
}

BTUINT32 CBlueSoleil::Btsdk_GetASerialNum()
{
	if(fnBtsdk_GetASerialNum)
		return fnBtsdk_GetASerialNum();
	return 0;
}

BOOL CBlueSoleil::Btsdk_PlugInVComm(UINT serialNum, ULONG *comportNumber, UINT usageType, ULONG flag, DWORD dwTimeout)
{
	if(fnBtsdk_PlugInVComm)
		return fnBtsdk_PlugInVComm(serialNum, comportNumber, usageType, flag, dwTimeout);
	return BTSDK_FALSE;
}

BTINT32 CBlueSoleil::Btsdk_InitCommObj(BTUINT8 com_idx, BTUINT16 svc_class)
{
	if(fnBtsdk_InitCommObj)
		return fnBtsdk_InitCommObj(com_idx,svc_class);
	return 0;
}

BTINT32 CBlueSoleil::Btsdk_ConnectEx(BTDEVHDL dev_hdl, BTUINT16 service_class, BTUINT32 lParam, BTCONNHDL *conn_hdl)
{
	if(fnBtsdk_ConnectEx)
		return fnBtsdk_ConnectEx(dev_hdl, service_class, lParam, conn_hdl);
	return 0;
}

BTINT32 CBlueSoleil::Btsdk_Disconnect(BTCONNHDL handle)
{
	if(fnBtsdk_Disconnect)
		return fnBtsdk_Disconnect(handle);
	return 0;
}

BTINT32 CBlueSoleil::Btsdk_DeinitCommObj(BTUINT8 com_idx)
{
	if(fnBtsdk_DeinitCommObj)
		return fnBtsdk_DeinitCommObj(com_idx);
	return 0;
}

void CBlueSoleil::Btsdk_PlugOutVComm(UINT serialNum, ULONG flag)
{
	if(fnBtsdk_PlugOutVComm)
		fnBtsdk_PlugOutVComm(serialNum,  flag);
}
BTINT32 CBlueSoleil::Btsdk_Done(void)
{
	if(fnBtsdk_Done)
		return fnBtsdk_Done();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////


CBlueSoleilDeviceIterator::CBlueSoleilDeviceIterator(CBlueSoleil* pBlueSoleil) : m_pBlueSoleil(pBlueSoleil)
{
	m_hEnumHdl = m_pBlueSoleil->Btsdk_StartEnumRemoteDevice(BTSDK_ERD_FLAG_NOLIMIT,
											//BTSDK_ERD_FLAG_PAIRED,
											0);
}

CBlueSoleilDeviceIterator::~CBlueSoleilDeviceIterator()
{
	if(m_hEnumHdl != BTSDK_INVALID_HANDLE)
		m_pBlueSoleil->Btsdk_EndEnumRemoteDevice(m_hEnumHdl);		
}

BtDevice CBlueSoleilDeviceIterator::GetBtDevice()
{
	BTUINT32 ulDevClass = 0;
	BTUINT16 usLen = 0;
	BTUINT8 btAddr[BTSDK_BDADDR_LEN] = {0};
	BTUINT8 szDevName[BTSDK_DEVNAME_LEN] = { 0 };
	char szBtAddr[3*BTSDK_BDADDR_LEN+1];
	

	if(m_dev_hdl  != BTSDK_INVALID_HANDLE)
	{
		m_pBlueSoleil->Btsdk_GetRemoteDeviceClass(m_dev_hdl, &ulDevClass);
			
		if( BTSDK_IMAGE_PRINTER == (ulDevClass & DEVICE_CLASS_MASK))
		{
		
			usLen = 32;
			if (m_pBlueSoleil->Btsdk_GetRemoteDeviceName(m_dev_hdl, szDevName, &usLen) != BTSDK_OK)
			{
				if (m_pBlueSoleil->Btsdk_UpdateRemoteDeviceName(m_dev_hdl, szDevName, &usLen) != BTSDK_OK)
				{
					strcpy((char*)szDevName, "Unknown");
				}
			}
			m_pBlueSoleil->Btsdk_GetRemoteDeviceAddress(m_dev_hdl, btAddr);

			sprintf_s(szBtAddr,3*6,"%02X:%02X:%02X:%02X:%02X:%02X",btAddr[5],btAddr[4],btAddr[3],btAddr[2],btAddr[1],btAddr[0]);
			
			return BtDevice(szDevName,szBtAddr);
		}	
	}

	return BtDevice("","");
}


bool CBlueSoleilDeviceIterator::Next()
{
	if(m_hEnumHdl != BTSDK_INVALID_HANDLE)
	{
		m_dev_hdl = m_pBlueSoleil->Btsdk_EnumRemoteDevice(m_hEnumHdl, NULL);
		return m_dev_hdl != BTSDK_INVALID_HANDLE;
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////
CBlueSoleilSerialSocket::CBlueSoleilSerialSocket(BTDEVHDL RmtSppDevHdl,CBlueSoleil* pBlueSoleil) : m_RmtSppDevHdl(RmtSppDevHdl),
																		  m_OSComPort(0),
																		  m_ComSerialNum(0),
																		  m_SPPConnHdl(BTSDK_INVALID_HANDLE),
																		  m_SPPSvcComPortHndl(INVALID_HANDLE_VALUE),
																		  m_pBlueSoleil(pBlueSoleil)
{
}

CBlueSoleilSerialSocket::~CBlueSoleilSerialSocket()
{
	Disconnect();
}

bool CBlueSoleilSerialSocket::Connect()
{	
	
	char szComPortName[30];

	if(m_RmtSppDevHdl == BTSDK_INVALID_HANDLE)
	{
		return false;
	}

	m_ComSerialNum = m_pBlueSoleil->Btsdk_GetASerialNum();
	m_pBlueSoleil->Btsdk_PlugInVComm(m_ComSerialNum , &m_OSComPort ,1 ,COMM_SET_RECORD|COMM_SET_USAGETYPE, 2200);
	if( m_pBlueSoleil->Btsdk_InitCommObj((BTUINT8)m_OSComPort, BTSDK_CLS_SERIAL_PORT) == BTSDK_OK)		
	{	
		BtSdkSPPConnParamStru sppStru = {0};
		sppStru.size = sizeof(BtSdkSPPConnParamStru);
		sppStru.com_index = (BTUINT8)m_OSComPort ;
		if (m_pBlueSoleil->Btsdk_ConnectEx(m_RmtSppDevHdl, BTSDK_CLS_SERIAL_PORT,(BTINT32)&sppStru, &m_SPPConnHdl) == BTSDK_OK)
		{
			sprintf(szComPortName,"\\\\.\\COM%d", m_OSComPort);
			if(OpenCOMPort(szComPortName))
			{
				return true;
			}			
			Disconnect();
		}
		else
		{
			m_pBlueSoleil->Btsdk_DeinitCommObj((UINT8)m_OSComPort);
			m_pBlueSoleil->Btsdk_PlugOutVComm(m_ComSerialNum, COMM_SET_RECORD);
		}
	}
	else
	{
		m_pBlueSoleil->Btsdk_PlugOutVComm(m_ComSerialNum, COMM_SET_RECORD);
	}
	return false;

}

void CBlueSoleilSerialSocket::Disconnect()
{
	CloseCOMPort();

	if (m_SPPConnHdl != BTSDK_INVALID_HANDLE)
	{
		m_pBlueSoleil->Btsdk_Disconnect(m_SPPConnHdl);
		m_SPPConnHdl = BTSDK_INVALID_HANDLE;
	}
	if(m_OSComPort != 0)
	{
		m_pBlueSoleil->Btsdk_DeinitCommObj((UINT8)m_OSComPort);
		m_OSComPort = 0;
	}
	if(m_ComSerialNum != 0)
	{
		m_pBlueSoleil->Btsdk_PlugOutVComm(m_ComSerialNum, COMM_SET_RECORD);
		m_ComSerialNum = 0;
	}
}

int CBlueSoleilSerialSocket::Write(BYTE* pBuffer,DWORD nBytesToWrite)
{
	if(m_SPPSvcComPortHndl != INVALID_HANDLE_VALUE)
	{
		DWORD dwBytesWritten = 0;
		if(WriteFile(m_SPPSvcComPortHndl, pBuffer,   nBytesToWrite,&dwBytesWritten,NULL))
			return (int)dwBytesWritten;
	}
	return -1;
}

int CBlueSoleilSerialSocket::Read(BYTE* pBuffer,DWORD nBytesToRead)
{
	if(m_SPPSvcComPortHndl != INVALID_HANDLE_VALUE)
	{
		DWORD dwBytesRead = 0;
		if(ReadFile(m_SPPSvcComPortHndl, pBuffer,   nBytesToRead,&dwBytesRead,NULL))
			return (int)dwBytesRead;
	}
	return -1;
	
}

BOOL CBlueSoleilSerialSocket::OpenCOMPort(const CString& portName)
{
	DCB portConfig;
	
  	  m_SPPSvcComPortHndl = CreateFile(portName,  // Specify port device: default "COM1"
     GENERIC_WRITE | GENERIC_READ,       // Specify mode that open device.
    0,                                  // the devide isn't shared.
    NULL,                               // the object gets a default security.
    OPEN_EXISTING,                      // Specify which action to take on file. 
    0,                                  // default.
    NULL);                              // default.

	if(m_SPPSvcComPortHndl == INVALID_HANDLE_VALUE)
	{
		//DWORD error = ::GetLastError();
		return FALSE;
	}

    // Get current configuration of serial communication port.
    if (GetCommState(m_SPPSvcComPortHndl,&portConfig) == 0)
    {
        return FALSE;
    }
    // Assign user parameter.
    portConfig.BaudRate = CBR_9600;//CBR_115200;//CBR_256000;    // Specify buad rate of communicaiton.
    portConfig.StopBits = ONESTOPBIT;    // Specify stopbit of communication.
    portConfig.Parity = NOPARITY;        // Specify parity of communication.
    portConfig.ByteSize = 8;    // Specify  byte of size of communication.

    // Set current configuration of serial communication port.
    if (SetCommState(m_SPPSvcComPortHndl,&portConfig) == 0)
    {
       return FALSE;
    }
  
    //// instance an object of COMMTIMEOUTS.
    //COMMTIMEOUTS comTimeOut;                   
    //// Specify time-out between charactor for receiving.
    //comTimeOut.ReadIntervalTimeout = 3;
    //// Specify value that is multiplied 
    //// by the requested number of bytes to be read. 
    //comTimeOut.ReadTotalTimeoutMultiplier = 3;
    //// Specify value is added to the product of the 
    //// ReadTotalTimeoutMultiplier member
    //comTimeOut.ReadTotalTimeoutConstant = 2;
    //// Specify value that is multiplied 
    //// by the requested number of bytes to be sent. 
    //comTimeOut.WriteTotalTimeoutMultiplier = 3;
    //// Specify value is added to the product of the 
    //// WriteTotalTimeoutMultiplier member
    //comTimeOut.WriteTotalTimeoutConstant = 2;
    //// set the time-out parameter into device control.
    //SetCommTimeouts(m_SPPSvcComPortHndl,&comTimeOut);
    //// Updatae port's status.
	return TRUE;
}


BOOL CBlueSoleilSerialSocket::CloseCOMPort()
{
	BOOL bCloseResult = TRUE;
	if(m_SPPSvcComPortHndl != INVALID_HANDLE_VALUE)
	{
		bCloseResult = CloseHandle(m_SPPSvcComPortHndl);
		m_SPPSvcComPortHndl = INVALID_HANDLE_VALUE;
	}

	return bCloseResult;
}
/////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CBlueSoleilInterface, BtInterface)

CBlueSoleilInterface::CBlueSoleilInterface() : m_pBlueSoleil(NULL)
{

}

CBlueSoleilInterface::~CBlueSoleilInterface()
{
	if(m_pBlueSoleil)
	{
		m_pBlueSoleil->Btsdk_Done();
		delete m_pBlueSoleil;
	}
}

bool CBlueSoleilInterface::LoadBlueSoleilLibrary()
{
	m_pBlueSoleil = new CBlueSoleil();
	if(m_pBlueSoleil->LoadBlueSoleil())
		return true;
	else
	{
		delete m_pBlueSoleil;
		m_pBlueSoleil = NULL;
	}
	return false;
}

bool CBlueSoleilInterface::InitBluetooth()
{
	if(m_pBlueSoleil) return true;

	if(!LoadBlueSoleilLibrary())
		return false;

	if (BTSDK_TRUE != m_pBlueSoleil->Btsdk_IsServerConnected()) /* not connected with BlueSoleil */
	{
		if (BTSDK_OK == m_pBlueSoleil->Btsdk_Init())
		{
			if (BTSDK_TRUE == m_pBlueSoleil->Btsdk_IsBluetoothHardwareExisted())
			{				
				if (BTSDK_FALSE == m_pBlueSoleil->Btsdk_IsBluetoothReady())
				{
					m_pBlueSoleil->Btsdk_StartBluetooth();					
				}
				if (BTSDK_TRUE == m_pBlueSoleil->Btsdk_IsBluetoothReady())
				{
					/*we default expect this application runs on desktop platform. 
					of course, you can set another device class according to your need. */
					m_pBlueSoleil->Btsdk_SetLocalDeviceClass(BTSDK_COMPCLS_DESKTOP);
					return true;
				}
			}
		}
	}
	return false;
}


BtDeviceIterator CBlueSoleilInterface::EnumBtPrinters()
{
	return BtDeviceIterator(new CBlueSoleilDeviceIterator(m_pBlueSoleil));
}

BtSerialSocket CBlueSoleilInterface::GetSerialSocket(const BtDevice& btPrinter)
{
	BTDEVHDL RmtSppDevHdl = BTSDK_INVALID_HANDLE;
	BTUINT8 btAddr[BTSDK_BDADDR_LEN] = {0};

	if(!BtAddrStrToBtAddr(btPrinter.GetDeviceBtAddress(),btAddr))
	{
		return BtSerialSocket(new CBlueSoleilSerialSocket(BTSDK_INVALID_HANDLE,NULL));;
	}
	RmtSppDevHdl = m_pBlueSoleil->Btsdk_GetRemoteDeviceHandle(btAddr);
	
	return BtSerialSocket(new CBlueSoleilSerialSocket(RmtSppDevHdl,m_pBlueSoleil));

}

BOOL CBlueSoleilInterface::BtAddrStrToBtAddr(const CString& sBtAddrStr,BTUINT8* btAddr)
{
	const int btAddrOctCount = 6;
	UINT btOctets[btAddrOctCount];

	if(!sscanf_s(sBtAddrStr,"%02x:%02x:%02x:%02x:%02x:%02x",&btOctets[5],&btOctets[4],&btOctets[3],&btOctets[2],&btOctets[1],&btOctets[0]))
			return FALSE;
	
	for(int i = 0;i < btAddrOctCount;i++)
		btAddr[i] = (BTUINT8)btOctets[i];	

	return TRUE;
}
