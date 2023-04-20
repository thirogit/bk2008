#include "stdafx.h"
#include "SeikoBluetoothPrinter.h"
#include "../../log.h"
//
//#include <Btsdk_Stru.h>
//#include <Btsdk_API.h>


CSeikoBluetoothPrinter::CSeikoBluetoothPrinter(const CString& sPrinterName,const CString& sPrinterAddress,const CString& sDriverName) :
	CSeikoPrinter(sPrinterName,sPrinterAddress,PRINTER_BLUETOOTH,sDriverName),m_SPPSocket(new BtInvalidSerialSocket())
		//,m_SPPSvcComPortHndl(INVALID_HANDLE_VALUE)
{
}

CSeikoBluetoothPrinter::~CSeikoBluetoothPrinter()
{
}
//
//BOOL CSeikoBluetoothPrinter::BtAddrStrToBtAddr(const CString& sBtAddrStr,BTUINT8* btAddr)
//{
//	const int btAddrOctCount = 6;
//	UINT btOctets[btAddrOctCount];
//
//	if(!sscanf_s(sBtAddrStr,"%02x:%02x:%02x:%02x:%02x:%02x",&btOctets[5],&btOctets[4],&btOctets[3],&btOctets[2],&btOctets[1],&btOctets[0]))
//			return FALSE;
//	
//	for(int i = 0;i < btAddrOctCount;i++)
//		btAddr[i] = (BTUINT8)btOctets[i];	
//
//	return TRUE;
//}

void CSeikoBluetoothPrinter::DoPrint(CBaseDocView* pDocView,UINT nNumOfCopies)
{
	CBluetooth bt;

	if(bt.InitBluetooth())
	{
		m_SPPSocket = bt.GetSerialSocket(BtDevice(GetPrinterName(),GetPrinterAddress()));
		if(m_SPPSocket.Connect())
		{
			CSeikoPrinter::DoPrint(pDocView,nNumOfCopies);
			m_SPPSocket.Disconnect();
		}
	}
	/*BTCONNHDL SPPConnHdl = BTSDK_INVALID_HANDLE;
	BTDEVHDL RmtSppDevHdl = BTSDK_INVALID_HANDLE;
	BTSHCHDL SppSvcHdl = BTSDK_INVALID_HANDLE;
	UINT ComSerialNum = 0;

	BTINT32 ulRet = BTSDK_FALSE;
	ULONG OSComPort;
	BTUINT8 btAddr[BTSDK_BDADDR_LEN] = {0};

	char szComPortName[30];

	if(!BtAddrStrToBtAddr(GetPrinterAddress(),btAddr))
	{
		return;
	}
	

	RmtSppDevHdl = Btsdk_GetRemoteDeviceHandle(btAddr);
	if(RmtSppDevHdl == BTSDK_INVALID_HANDLE)
	{
		return;
	}

	ComSerialNum = Btsdk_GetASerialNum();
	Btsdk_PlugInVComm(ComSerialNum , &OSComPort ,1 ,COMM_SET_RECORD|COMM_SET_USAGETYPE, 2200);
	if( Btsdk_InitCommObj((BTUINT8)OSComPort, BTSDK_CLS_SERIAL_PORT) == BTSDK_OK)		
	{	
		BtSdkSPPConnParamStru sppStru = {0};
		sppStru.size = sizeof(BtSdkSPPConnParamStru);
		sppStru.com_index = (BTUINT8)OSComPort ;
		if (Btsdk_ConnectEx(RmtSppDevHdl, BTSDK_CLS_SERIAL_PORT,(BTINT32)&sppStru, &SPPConnHdl) == BTSDK_OK)
		{
			sprintf(szComPortName,"\\\\.\\COM%d", OSComPort);
			if(OpenCOMPort(szComPortName))
			{
				CSeikoPrinter::DoPrint(pDocView,nNumOfCopies);
				CloseCOMPort();
			}

			if (SPPConnHdl)
			{
				Btsdk_Disconnect(SPPConnHdl);
				SPPConnHdl = BTSDK_INVALID_HANDLE;
				Btsdk_DeinitCommObj((UINT8)OSComPort);
				Btsdk_PlugOutVComm(ComSerialNum, COMM_SET_RECORD);
			}

		}
	}	*/

}
//
//BOOL CSeikoBluetoothPrinter::OpenCOMPort(const CString& portName)
//{
//	DCB portConfig;
//	
//  	  m_SPPSvcComPortHndl = CreateFile(portName,  // Specify port device: default "COM1"
//     GENERIC_WRITE | GENERIC_READ,       // Specify mode that open device.
//    0,                                  // the devide isn't shared.
//    NULL,                               // the object gets a default security.
//    OPEN_EXISTING,                      // Specify which action to take on file. 
//    0,                                  // default.
//    NULL);                              // default.
//
//	if(m_SPPSvcComPortHndl == INVALID_HANDLE_VALUE)
//	{
//		//DWORD error = ::GetLastError();
//		return FALSE;
//	}
//
//    // Get current configuration of serial communication port.
//    if (GetCommState(m_SPPSvcComPortHndl,&portConfig) == 0)
//    {
//        return FALSE;
//    }
//    // Assign user parameter.
//    portConfig.BaudRate = CBR_9600;//CBR_115200;//CBR_256000;    // Specify buad rate of communicaiton.
//    portConfig.StopBits = ONESTOPBIT;    // Specify stopbit of communication.
//    portConfig.Parity = NOPARITY;        // Specify parity of communication.
//    portConfig.ByteSize = 8;    // Specify  byte of size of communication.
//
//    // Set current configuration of serial communication port.
//    if (SetCommState(m_SPPSvcComPortHndl,&portConfig) == 0)
//    {
//       return FALSE;
//    }
//  
//    //// instance an object of COMMTIMEOUTS.
//    //COMMTIMEOUTS comTimeOut;                   
//    //// Specify time-out between charactor for receiving.
//    //comTimeOut.ReadIntervalTimeout = 3;
//    //// Specify value that is multiplied 
//    //// by the requested number of bytes to be read. 
//    //comTimeOut.ReadTotalTimeoutMultiplier = 3;
//    //// Specify value is added to the product of the 
//    //// ReadTotalTimeoutMultiplier member
//    //comTimeOut.ReadTotalTimeoutConstant = 2;
//    //// Specify value that is multiplied 
//    //// by the requested number of bytes to be sent. 
//    //comTimeOut.WriteTotalTimeoutMultiplier = 3;
//    //// Specify value is added to the product of the 
//    //// WriteTotalTimeoutMultiplier member
//    //comTimeOut.WriteTotalTimeoutConstant = 2;
//    //// set the time-out parameter into device control.
//    //SetCommTimeouts(m_SPPSvcComPortHndl,&comTimeOut);
//    //// Updatae port's status.
//	return TRUE;
//}
//
//
//BOOL CSeikoBluetoothPrinter::CloseCOMPort()
//{
//	if(m_SPPSvcComPortHndl != INVALID_HANDLE_VALUE)
//		return CloseHandle(m_SPPSvcComPortHndl); 
//
//	return TRUE;
//}


DWORD CSeikoBluetoothPrinter::WritePrinter(BYTE *pBuffer,DWORD bytesToWrite)
{
	
	return (DWORD)m_SPPSocket.Write(pBuffer,bytesToWrite);
	/*if(m_SPPSvcComPortHndl != INVALID_HANDLE_VALUE)
	{
		DWORD dwBytesWritten = 0;
		if(WriteFile(m_SPPSvcComPortHndl, pBuffer,   bytesToWrite,&dwBytesWritten,NULL))
			return dwBytesWritten;
	}*/
	//return (DWORD)(-1);
}

