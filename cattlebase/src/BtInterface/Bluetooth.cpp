#include "stdafx.h"
#include "Bluetooth.h"
#include "BlueSoleilInterface.h"
#include "WSABtInterface.h"

boost::shared_ptr<BtInterface> CBluetooth::m_btrBluetooth;

bool CBluetooth::InitBluetooth()
{
	CRuntimeClass* BtInterfaces[] = 
	{
		RUNTIME_CLASS(CBlueSoleilInterface),
		RUNTIME_CLASS(WSABtInterface),
		NULL
	};


	if(m_btrBluetooth.get() == NULL)
	{
		CRuntimeClass** pBtInterfaceClass = BtInterfaces;
		while(*pBtInterfaceClass)
		{
			BtInterface* pBtInterface = (BtInterface*)((*pBtInterfaceClass)->CreateObject());

			if(pBtInterface->InitBluetooth())
			{
				m_btrBluetooth = boost::shared_ptr<BtInterface>(pBtInterface);
				return true;
			}
			else
			{
				delete pBtInterface;
				pBtInterfaceClass++;				
			}
		}
		return false;
	}
	return true;
}

BtDeviceIterator CBluetooth::EnumBtPrinters()
{
	if(m_btrBluetooth.get() != NULL)
	{
		return m_btrBluetooth->EnumBtPrinters();
	}
	else
		return BtDeviceIterator::nullIterator();
}

BtSerialSocket CBluetooth::GetSerialSocket(const BtDevice& btPrinter)
{
	if(m_btrBluetooth.get() != NULL)
	{
		return m_btrBluetooth->GetSerialSocket(btPrinter);
	}
	else
		return BtSerialSocket(new BtInvalidSerialSocket());
}

