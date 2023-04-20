#if !defined(SYNCSOCKET_H__INCLUDED_)
#define SYNCSOCKET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxsock.h>
#include <afxmt.h>

#define DEFAULT_PORT 998
#define MAX_CMD_LEN 256

class CSyncSocket;

class CSyncTransceiver
{
public:
	CSyncTransceiver(CSyncSocket *pSock);
    void ReceiveData();
	bool GetCmd(char *pCmd);
	bool SendCmd(const char * pCmd,int nCmdLen = -1);
private:
	CSyncSocket * m_pSock;
	char m_recvBuffer[MAX_CMD_LEN+1];
	int m_recvBufferPos;
	CCriticalSection m_cs;
	CList<CString,CString&> m_cmdList;

};


class CSyncSocket : public CAsyncSocket
{
// Attributes
public:


	interface ISocketCallback
	{
		virtual void OnAccept() = 0;
		virtual void OnDisconnect() = 0;
		virtual void OnConnect() = 0;
		virtual void OnOutOfBandData() = 0;
		virtual void OnReceive() = 0;
		virtual void OnSend() = 0;
	};

// Operations
public:
	CSyncSocket();
	virtual ~CSyncSocket();

	BOOL CreateServerSocket(UINT nSockPort,ISocketCallback *pISockCallback);
	BOOL CreateClientSocket(ISocketCallback *pISockCallback);
	
protected:
	
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	
protected:
	bool m_bCreated;
	ISocketCallback *m_pISocketCallback;

};


#endif 
