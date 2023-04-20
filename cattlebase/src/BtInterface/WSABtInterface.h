#ifndef __WSABTINTERFACE_H__
#define __WSABTINTERFACE_H__

#include "BtInterface.h"
#include <winsock2.h>
#include <ws2bth.h>

class WSABtDeviceIterator : public BtDeviceIteratorInterface
{
public:
	WSABtDeviceIterator(HANDLE hBtDeviceLookup);
	virtual BtDevice GetBtDevice();
	virtual bool Next();
	~WSABtDeviceIterator();
private:
	HANDLE	m_hBtDeviceLookup;
	PWSAQUERYSET m_pWSAQuerySet;
};

class WSABtSerialSocket : public BtSerialSocketInterface
{
public:
	WSABtSerialSocket(SOCKET btSocket,BTH_ADDR bthRemoteAddr);
	virtual bool Connect();
	virtual void Disconnect();
	virtual int Write(BYTE* pBuffer,DWORD nBytesToWrite);
	virtual int Read(BYTE* pBuffer,DWORD nBytesToRead);
	~WSABtSerialSocket();
private:
	SOCKET m_btSocket;
	BTH_ADDR m_bthRemoteAddr;

};

class WSABtInterface : public BtInterface
{
	DECLARE_DYNCREATE(WSABtInterface);
public:
	virtual bool InitBluetooth();
	virtual BtDeviceIterator EnumBtPrinters();
	virtual BtSerialSocket GetSerialSocket(const BtDevice& btPrinter);
	virtual ~WSABtInterface();
protected:
	bool GetWSABthAddr(const CString& sBthAddr,BTH_ADDR *pRemoteBtAddr);
private:
	 WSADATA m_WSAData;
};





#endif
