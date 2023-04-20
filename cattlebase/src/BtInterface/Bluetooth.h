#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "BtInterface.h"

class CBluetooth : public BtInterface
{
public:
	virtual bool InitBluetooth();
	virtual BtDeviceIterator EnumBtPrinters();
	virtual BtSerialSocket GetSerialSocket(const BtDevice& btPrinter);
protected:

private:
	static boost::shared_ptr<BtInterface> m_btrBluetooth;
};




#endif
