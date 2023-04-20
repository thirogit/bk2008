#ifndef __BTINTERFACE_H__
#define __BTINTERFACE_H__

#include <boost/shared_ptr.hpp>

class BtDevice
{
public:
	BtDevice(const CString& sDeviceName,const CString& sBtAddress);
	BtDevice(const BtDevice& copy);
	BtDevice& operator=(const BtDevice& copy);
	void CopyFrom(const BtDevice& copy);

	CString GetDeviceName() const;
	CString GetDeviceBtAddress() const;
	void SetDeviceName(const CString& sDeviceName);
	void SetDeviceBtAddress(const CString& sBtAddress);
protected:
	CString m_sDeviceName;
	CString m_sDeviceBtAddress;
};

class BtDeviceIteratorInterface
{
public:
	virtual BtDevice GetBtDevice() = 0; 
	virtual bool Next() = 0;    
};

class BtNullIterator : public BtDeviceIteratorInterface
{
public:
	virtual BtDevice GetBtDevice();
	virtual bool Next();
};


class BtDeviceIterator
{
public:
	BtDeviceIterator(BtDeviceIteratorInterface* pDeviceIterator);
	BtDeviceIterator(const BtDeviceIterator& copy);
	BtDeviceIterator& operator=(const BtDeviceIterator& copy);
	void CopyFrom(const BtDeviceIterator& copy);
	BtDevice operator*();
	bool Next();
	~BtDeviceIterator();

	static BtDeviceIterator nullIterator();
private:
	boost::shared_ptr<BtDeviceIteratorInterface> m_ptrDeviceIterator;
};


class BtSerialSocketInterface
{
public:
	virtual bool Connect() = 0;
	virtual void Disconnect() = 0;
	virtual int Write(BYTE* pBuffer,DWORD nBytesToWrite) = 0;
	virtual int Read(BYTE* pBuffer,DWORD nBytesToRead) = 0;

};

class BtInvalidSerialSocket : public BtSerialSocketInterface
{
public:
	virtual bool Connect();
	virtual void Disconnect();
	virtual int Write(BYTE* pBuffer,DWORD nBytesToWrite);
	virtual int Read(BYTE* pBuffer,DWORD nBytesToRead);
};


class BtSerialSocket
{
public:
	BtSerialSocket(BtSerialSocketInterface* pBtSocketInterface);
	BtSerialSocket(const BtSerialSocket& copy);
	BtSerialSocket& operator=(const BtSerialSocket& copy);
	void CopyFrom(const BtSerialSocket& copy);
	~BtSerialSocket();
	bool Connect();
	void Disconnect();
	int Write(BYTE* pBuffer,DWORD nBytesToWrite);
	int Read(BYTE* pBuffer,DWORD nBytesToRead);
private:
	boost::shared_ptr<BtSerialSocketInterface> m_ptrBtSocketInterface;

};




class BtInterface : public CObject
{	
public:
	virtual bool InitBluetooth() = 0;
	virtual BtDeviceIterator EnumBtPrinters() = 0;
	virtual BtSerialSocket GetSerialSocket(const BtDevice& btPrinter) = 0;
	virtual ~BtInterface() {};
};






#endif
