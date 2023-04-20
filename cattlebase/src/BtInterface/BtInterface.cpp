#include "stdafx.h"
#include "BtInterface.h"



/////////////////////////////////////////////////////////////////////////////////////////
BtDevice::BtDevice(const CString& sDeviceName,const CString& sBtAddress) : m_sDeviceName(sDeviceName),m_sDeviceBtAddress(sBtAddress)
{

}
BtDevice::BtDevice(const BtDevice& copy)
{
	CopyFrom(copy);
}

BtDevice& BtDevice::operator=(const BtDevice& copy)
{
	CopyFrom(copy);
	return *this;
}

void BtDevice::CopyFrom(const BtDevice& copy)
{
	m_sDeviceName = copy.m_sDeviceName;
	m_sDeviceBtAddress = copy.m_sDeviceBtAddress;

}

CString BtDevice::GetDeviceName() const
{
	return m_sDeviceName;
}

CString BtDevice::GetDeviceBtAddress() const
{
	return m_sDeviceBtAddress;
}

void BtDevice::SetDeviceName(const CString& sDeviceName)
{
	m_sDeviceName = sDeviceName;
}

void BtDevice::SetDeviceBtAddress(const CString& sBtAddress)
{
	m_sDeviceBtAddress = sBtAddress;
}
/////////////////////////////////////////////////////////////////////////////////////////
BtDevice BtNullIterator::GetBtDevice()
{
	return BtDevice("","");
}
bool BtNullIterator::Next()
{
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////
BtDeviceIterator::BtDeviceIterator(BtDeviceIteratorInterface* pDeviceIterator)
{
	m_ptrDeviceIterator = boost::shared_ptr<BtDeviceIteratorInterface>(pDeviceIterator);
}
BtDeviceIterator::BtDeviceIterator(const BtDeviceIterator& copy)
{
	CopyFrom(copy);
}

BtDeviceIterator& BtDeviceIterator::operator=(const BtDeviceIterator& copy)
{
	CopyFrom(copy);
	return *this;
}

void BtDeviceIterator::CopyFrom(const BtDeviceIterator& copy)
{
	m_ptrDeviceIterator = copy.m_ptrDeviceIterator;
}

BtDevice BtDeviceIterator::operator*()
{
	return m_ptrDeviceIterator->GetBtDevice();
}

bool BtDeviceIterator::Next()
{
	return m_ptrDeviceIterator->Next();
}

BtDeviceIterator::~BtDeviceIterator()
{
}

BtDeviceIterator BtDeviceIterator::nullIterator()
{
	return BtDeviceIterator(new BtNullIterator());
}
/////////////////////////////////////////////////////////////////////////////////////////
bool BtInvalidSerialSocket::Connect()
{
	return false;
}

void BtInvalidSerialSocket::Disconnect()
{
	return;
}
int BtInvalidSerialSocket::Write(BYTE* pBuffer,DWORD nBytesToWrite)
{
	return -1;
}
int BtInvalidSerialSocket::Read(BYTE* pBuffer,DWORD nBytesToRead)
{
	return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////
BtSerialSocket::BtSerialSocket(BtSerialSocketInterface* pBtSocketInterface)
{
	m_ptrBtSocketInterface = boost::shared_ptr<BtSerialSocketInterface>(pBtSocketInterface);;
}

BtSerialSocket::BtSerialSocket(const BtSerialSocket& copy)
{
	CopyFrom(copy);
}
BtSerialSocket& BtSerialSocket::operator=(const BtSerialSocket& copy)
{
	CopyFrom(copy);
	return *this;
}

void BtSerialSocket::CopyFrom(const BtSerialSocket& copy)
{
	m_ptrBtSocketInterface = copy.m_ptrBtSocketInterface;
}

BtSerialSocket::~BtSerialSocket()
{
}

bool BtSerialSocket::Connect()
{
	return m_ptrBtSocketInterface->Connect();
}

void BtSerialSocket::Disconnect()
{
	return m_ptrBtSocketInterface->Disconnect();
}

int BtSerialSocket::Write(BYTE* pBuffer,DWORD nBytesToWrite)
{
	return m_ptrBtSocketInterface->Write(pBuffer,nBytesToWrite);
}

int BtSerialSocket::Read(BYTE* pBuffer,DWORD nBytesToRead)
{
	return m_ptrBtSocketInterface->Read(pBuffer,nBytesToRead);
}

	


