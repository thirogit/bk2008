#ifndef __BLUESOLEILINTERFACE_H__
#define __BLUESOLEILINTERFACE_H__

#include "BtInterface.h"
#include <Btsdk_Macro.h>
#include <Btsdk_Stru.h>
#include <Btsdk_API.h>


class CBlueSoleil
{
public:
	CBlueSoleil();
	~CBlueSoleil();
	bool LoadBlueSoleil();

	BTBOOL Btsdk_IsServerConnected();
	BTINT32 Btsdk_Init(void);
	BTBOOL Btsdk_IsBluetoothHardwareExisted();
	BTBOOL Btsdk_IsBluetoothReady(void);
	BTINT32 Btsdk_StartBluetooth(void);
	BTINT32 Btsdk_SetLocalDeviceClass(BTUINT32 device_class);
	BTSDKHANDLE Btsdk_StartEnumRemoteDevice(BTUINT32 flag, BTUINT32 dev_class);
	BTDEVHDL Btsdk_EnumRemoteDevice(BTSDKHANDLE enum_handle, PBtSdkRemoteDevicePropertyStru rmt_dev_prop);
	BTINT32 Btsdk_GetRemoteDeviceClass(BTDEVHDL dev_hdl, BTUINT32 *pdevice_class);
	BTINT32 Btsdk_GetRemoteDeviceName(BTDEVHDL dev_hdl, BTUINT8 *name, BTUINT16 *plen);
	BTINT32 Btsdk_UpdateRemoteDeviceName(BTDEVHDL dev_hdl, BTUINT8 *name, BTUINT16 *plen);
	BTINT32 Btsdk_GetRemoteDeviceAddress(BTDEVHDL dev_hdl, BTUINT8 *bd_addr);
	BTINT32 Btsdk_EndEnumRemoteDevice(BTSDKHANDLE enum_handle);
	BTDEVHDL Btsdk_GetRemoteDeviceHandle(BTUINT8 *bd_addr);
	BTUINT32 Btsdk_GetASerialNum();
	BOOL Btsdk_PlugInVComm(UINT serialNum, ULONG *comportNumber, UINT usageType, ULONG flag, DWORD dwTimeout);
	BTINT32 Btsdk_InitCommObj(BTUINT8 com_idx, BTUINT16 svc_class);
	BTINT32 Btsdk_ConnectEx(BTDEVHDL dev_hdl, BTUINT16 service_class, BTUINT32 lParam, BTCONNHDL *conn_hdl);
	BTINT32 Btsdk_Disconnect(BTCONNHDL handle);
	BTINT32 Btsdk_DeinitCommObj(BTUINT8 com_idx);
	void Btsdk_PlugOutVComm(UINT serialNum, ULONG flag);
	BTINT32 Btsdk_Done(void);
private:
	BTBOOL (*fnBtsdk_IsServerConnected)();
	BTINT32 (*fnBtsdk_Init)(void);
	BTBOOL (*fnBtsdk_IsBluetoothHardwareExisted)();
	BTBOOL (*fnBtsdk_IsBluetoothReady)(void);
	BTINT32 (*fnBtsdk_StartBluetooth)(void);
	BTINT32 (*fnBtsdk_SetLocalDeviceClass)(BTUINT32 device_class);
	BTSDKHANDLE (*fnBtsdk_StartEnumRemoteDevice)(BTUINT32 flag, BTUINT32 dev_class);
	BTDEVHDL (*fnBtsdk_EnumRemoteDevice)(BTSDKHANDLE enum_handle, PBtSdkRemoteDevicePropertyStru rmt_dev_prop);
	BTINT32 (*fnBtsdk_GetRemoteDeviceClass)(BTDEVHDL dev_hdl, BTUINT32 *pdevice_class);
	BTINT32 (*fnBtsdk_GetRemoteDeviceName)(BTDEVHDL dev_hdl, BTUINT8 *name, BTUINT16 *plen);
	BTINT32 (*fnBtsdk_UpdateRemoteDeviceName)(BTDEVHDL dev_hdl, BTUINT8 *name, BTUINT16 *plen);
	BTINT32 (*fnBtsdk_GetRemoteDeviceAddress)(BTDEVHDL dev_hdl, BTUINT8 *bd_addr);
	BTINT32 (*fnBtsdk_EndEnumRemoteDevice)(BTSDKHANDLE enum_handle);
	BTDEVHDL (*fnBtsdk_GetRemoteDeviceHandle)(BTUINT8 *bd_addr);
	BTUINT32 (*fnBtsdk_GetASerialNum)();
	BOOL (*fnBtsdk_PlugInVComm)(UINT serialNum, ULONG *comportNumber, UINT usageType, ULONG flag, DWORD dwTimeout);
	BTINT32 (*fnBtsdk_InitCommObj)(BTUINT8 com_idx, BTUINT16 svc_class);
	BTINT32 (*fnBtsdk_ConnectEx)(BTDEVHDL dev_hdl, BTUINT16 service_class, BTUINT32 lParam, BTCONNHDL *conn_hdl);
	BTINT32 (*fnBtsdk_Disconnect)(BTCONNHDL handle);
	BTINT32 (*fnBtsdk_DeinitCommObj)(BTUINT8 com_idx);
	void (*fnBtsdk_PlugOutVComm)(UINT serialNum, ULONG flag);
	BTINT32 (*fnBtsdk_Done)(void);

	HMODULE m_hBlueSoleilDLL; 
};

class CBlueSoleilDeviceIterator : public BtDeviceIteratorInterface
{
public:
	CBlueSoleilDeviceIterator(CBlueSoleil* pBlueSoleil);
	~CBlueSoleilDeviceIterator();
	virtual BtDevice GetBtDevice(); 
	virtual bool Next();
private:
	BTSDKHANDLE m_hEnumHdl;
	BTDEVHDL m_dev_hdl;
	CBlueSoleil* m_pBlueSoleil;

};


class CBlueSoleilSerialSocket : public BtSerialSocketInterface
{
public:
	CBlueSoleilSerialSocket(BTDEVHDL RmtSppDevHdl,CBlueSoleil* pBlueSoleil);
	~CBlueSoleilSerialSocket();
	virtual bool Connect();
	virtual void Disconnect();
	virtual int Write(BYTE* pBuffer,DWORD nBytesToWrite);
	virtual int Read(BYTE* pBuffer,DWORD nBytesToRead);
private:
	BOOL OpenCOMPort(const CString& portName);
	BOOL CloseCOMPort();
private:
	BTDEVHDL m_RmtSppDevHdl;
	ULONG m_OSComPort;
	UINT m_ComSerialNum;
	BTCONNHDL m_SPPConnHdl;
	HANDLE m_SPPSvcComPortHndl;
	CBlueSoleil* m_pBlueSoleil;

};


class CBlueSoleilInterface : public BtInterface
{	
	DECLARE_DYNCREATE(CBlueSoleilInterface);
public:
	CBlueSoleilInterface();
	virtual bool InitBluetooth();
	virtual BtDeviceIterator EnumBtPrinters();
	virtual BtSerialSocket GetSerialSocket(const BtDevice& btPrinter);
	virtual ~CBlueSoleilInterface();
protected:
	bool LoadBlueSoleilLibrary();
	BOOL BtAddrStrToBtAddr(const CString& sBtAddrStr,BTUINT8* btAddr);
private:
	CBlueSoleil* m_pBlueSoleil;
	

};






#endif
