#include "stdafx.h"
#include "WSABtInterface.h"
#include <initguid.h>

DEFINE_GUID(SPP_GUIDServiceClass, 0x00001101, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb);


WSABtDeviceIterator::WSABtDeviceIterator(HANDLE hBtDeviceLookup) : m_hBtDeviceLookup(hBtDeviceLookup)
{
	m_pWSAQuerySet = NULL;		
}

BtDevice WSABtDeviceIterator::GetBtDevice()
{
	BTH_ADDR bthRemoteBtAddr;
	 ;
				
    CopyMemory(&bthRemoteBtAddr,
              &((PSOCKADDR_BTH) m_pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr,
              sizeof(bthRemoteBtAddr));


	CString sBthAddr;
	for(int i = 5;i >= 0;i--)
	{
		sBthAddr.AppendFormat("%02X",((BYTE*)&bthRemoteBtAddr)[i]);
		if(i != 0)
			sBthAddr.AppendChar(':');
	}

	return BtDevice(m_pWSAQuerySet->lpszServiceInstanceName,sBthAddr);

}

bool WSABtDeviceIterator::Next()
{
	
	ULONG	ulFlags = LUP_CONTAINERS | LUP_RETURN_NAME | LUP_RETURN_ADDR, 
	ulPQSSize = sizeof(WSAQUERYSET);

	m_pWSAQuerySet = (PWSAQUERYSET) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulPQSSize);

	ZeroMemory(m_pWSAQuerySet, ulPQSSize);
	m_pWSAQuerySet->dwNameSpace = NS_BTH;
    m_pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);

	while(true)
	{
		if ( NO_ERROR == WSALookupServiceNext(m_hBtDeviceLookup,
											  ulFlags,
											  &ulPQSSize,
											  m_pWSAQuerySet) ) 
		{
			return true;
		} 
		else 
		{                
			INT iResult = 0;

			if ( WSA_E_NO_MORE == ( iResult = WSAGetLastError() ) ) 
			{ 
				TRACE("No more devices found.\n");
			}
			else if ( WSAEFAULT == iResult ) 
			{
				//
				// The buffer for QUERYSET was insufficient.
				// In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
				// the required size.  So we can use this parameter to reallocate memory for QUERYSET.
				//
				HeapFree(GetProcessHeap(), 0, m_pWSAQuerySet);
				m_pWSAQuerySet = (PWSAQUERYSET) HeapAlloc(GetProcessHeap(),
																HEAP_ZERO_MEMORY,
																ulPQSSize);   
				
				TRACE(" The buffer for QUERYSET was insufficient.\n");
				continue;
			} 
			else 
			{
				TRACE("=CRITICAL= | WSALookupServiceNext() failed with error code %d\n", iResult);	            
			}
		}
		break;

	} 

	return false;
    
}
WSABtDeviceIterator::~WSABtDeviceIterator()
{
	if(m_pWSAQuerySet != NULL)
	{
		HeapFree(GetProcessHeap(), 0, m_pWSAQuerySet);
		m_pWSAQuerySet = NULL;
	}	
    WSALookupServiceEnd(m_hBtDeviceLookup);
}

/////////////////////////////////////////////////////////////////////////////////////////

WSABtSerialSocket::WSABtSerialSocket(SOCKET btSocket,BTH_ADDR bthRemoteAddr) : m_btSocket(btSocket),m_bthRemoteAddr(bthRemoteAddr)
{
}

bool WSABtSerialSocket::Connect()
{
	
	SOCKADDR_BTH	SockAddrBthServer = {0};
      
    //
    // Setting address family to AF_BTH indicates winsock2 to use Bluetooth sockets
    // Port should be set to 0 if ServiceClassId is spesified.
    //
    SockAddrBthServer.addressFamily = AF_BTH;
    SockAddrBthServer.btAddr =  m_bthRemoteAddr;
    SockAddrBthServer.serviceClassId = SPP_GUIDServiceClass;
    SockAddrBthServer.port = 0;

	//
	// Connect the socket (pSocket) to a given remote socket represented by address (pServerAddr)
	//
    if ( SOCKET_ERROR == connect(m_btSocket,
                                 (struct sockaddr *) &SockAddrBthServer,
                                 sizeof(SOCKADDR_BTH)) ) 
	{
		return false;
    }
	return true;
}

void WSABtSerialSocket::Disconnect()
{
	shutdown(m_btSocket,SD_BOTH);
}

int WSABtSerialSocket::Write(BYTE* pBuffer,DWORD nBytesToWrite)
{ 
    return send(m_btSocket,(const char*) pBuffer, nBytesToWrite, 0) ;
}

int WSABtSerialSocket::Read(BYTE* pBuffer,DWORD nBytesToRead)
{
	return recv(m_btSocket,(char*)pBuffer,nBytesToRead,0);
}

WSABtSerialSocket::~WSABtSerialSocket()
{
	 closesocket(m_btSocket);
}

/////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(WSABtInterface, BtInterface)

bool WSABtInterface::InitBluetooth()
{
	ZeroMemory(&m_WSAData, sizeof(m_WSAData));
	
	ULONG 		ulRetCode = WSAStartup(MAKEWORD(2, 2), &m_WSAData);
    return ( 0 == ulRetCode );
}

BtDeviceIterator WSABtInterface::EnumBtPrinters()
{
	INT				iResult = 0;
    ULONG			ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
    HANDLE			hLookup = 0;
    WSAQUERYSET		WSAQuerySet;
   
   
    //
    // WSALookupService is used for both service search and device inquiry
    // LUP_CONTAINERS is the flag which signals that we're doing a device inquiry.
    //
    ulFlags = LUP_CONTAINERS;

    //
    // Friendly device name (if available) will be returned in lpszServiceInstanceName
    //
    ulFlags |= LUP_RETURN_NAME;

    //
    // BTH_ADDR will be returned in lpcsaBuffer member of WSAQUERYSET
    //
    ulFlags |= LUP_RETURN_ADDR;

    
    //
    // Flush the device cache for all inquiries, except for the first inquiry
    //
    // By setting LUP_FLUSHCACHE flag, we're asking the lookup service to do
    // a fresh lookup instead of pulling the information from device cache.
    //
    //ulFlags |= LUP_FLUSHCACHE;

        
    iResult = 0;
    hLookup = 0;
    
    ZeroMemory(&WSAQuerySet, ulPQSSize);
    WSAQuerySet.dwNameSpace = NS_BTH;
    WSAQuerySet.dwSize = sizeof(WSAQUERYSET);
    iResult = WSALookupServiceBegin(&WSAQuerySet, ulFlags, &hLookup);

    if ( (NO_ERROR == iResult) && (NULL != hLookup) ) 
	{
		return BtDeviceIterator(new WSABtDeviceIterator(hLookup));
    } 
	else 
	{
		int iWSALastError = WSAGetLastError();
		return BtDeviceIterator(new BtNullIterator());
    }
}

BtSerialSocket WSABtInterface::GetSerialSocket(const BtDevice& btPrinter)
{
    SOCKET			bthSocket = INVALID_SOCKET;
    

      //
    // Open a bluetooth socket using RFCOMM protocol
    //
    bthSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if ( INVALID_SOCKET == bthSocket ) 
	{
		return BtSerialSocket(new BtInvalidSerialSocket());
    }

	BTH_ADDR bthRemoteAddr;
	
	if(GetWSABthAddr(btPrinter.GetDeviceBtAddress(),&bthRemoteAddr))
	{
		return BtSerialSocket(new WSABtSerialSocket(bthSocket,bthRemoteAddr));
	}
	
	return BtSerialSocket(new BtInvalidSerialSocket());


}

bool WSABtInterface::GetWSABthAddr(const CString& sBthAddr,BTH_ADDR *pRemoteBtAddr)
{
	ULONG		ulAddrData[6];
    BTH_ADDR	BtAddrTemp = 0;

	if ( ( 17 != sBthAddr.GetLength() ) || ( NULL == pRemoteBtAddr ) ) 
	{
        return false;
    }

    *pRemoteBtAddr = 0;

    //
    // Populate a 6 membered array of unsigned long integers
    // by parsing the given address in string format
    //
    sscanf_s((LPCSTR)sBthAddr,
           "%02x:%02x:%02x:%02x:%02x:%02x",
           &ulAddrData[0],&ulAddrData[1],&ulAddrData[2],&ulAddrData[3],&ulAddrData[4],&ulAddrData[5]);

    //
    // Construct a BTH_ADDR from the 6 integers stored in the array
    //
    for ( int i=0; i<6; i++ ) 
	{
        //
        // Extract data from the first 8 lower bits.
        //
        BtAddrTemp = (BTH_ADDR)( ulAddrData[i] & 0xFF );

        //
        // Push 8 bits to the left
        //
        *pRemoteBtAddr = ( (*pRemoteBtAddr) << 8 ) + BtAddrTemp;
    }

   return true;
}

WSABtInterface::~WSABtInterface()
{
}

