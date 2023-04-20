// CSyncSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SyncSocket.h"
#include "../log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSyncTransceiver

CSyncTransceiver::CSyncTransceiver(CSyncSocket *pSock) : m_pSock(pSock),m_recvBufferPos(0)
{
	ASSERT(m_pSock);
}
void CSyncTransceiver::ReceiveData()
{
	char partialBuffer[MAX_CMD_LEN];
	int byteRead = m_pSock->Receive(partialBuffer,MAX_CMD_LEN);
	if(byteRead == SOCKET_ERROR) return;


	CString test(partialBuffer,byteRead);
	LOG("ReceiveData() partialBuffer: %s",(LPCSTR)test);
    
	CSingleLock lock(&m_cs);
	int partPos = 0,lastLF  = 0,i;
	bool bAdd = false;
	if (byteRead > 0)
	{
		while(partPos < byteRead)
		{
			if(partialBuffer[partPos] != '\n') 
			{
				partPos++;
				continue;
			}
			bAdd = true;
			
			for(i = lastLF;i < partPos;i++)
			{
				m_recvBuffer[m_recvBufferPos] = partialBuffer[i];
				m_recvBufferPos++;
				if(m_recvBufferPos >= MAX_CMD_LEN)
				{
					m_recvBufferPos = 0;
					bAdd = false;
					break; //drop this command
				}
			}
			if(bAdd)
			{
				lock.Lock();
				
				CString test2(m_recvBuffer,m_recvBufferPos);
				LOG("ReceiveData() add: %s",(LPCSTR)test2);

				m_cmdList.AddHead(test2);
				lock.Unlock();
				m_recvBufferPos = 0;
			}
			lastLF = partPos;
			break;
		}

		for(i = lastLF;i < partPos;i++)
		{
			m_recvBuffer[m_recvBufferPos++] = partialBuffer[i];					
		}
	}
		
}
bool CSyncTransceiver::GetCmd(char *pCmd)
{
	bool bRet = false;

	CSingleLock lock(&m_cs);
	// Lock
	lock.Lock();
	if(m_cmdList.GetCount() > 0)
	{
		strncpy(pCmd,(LPCSTR)m_cmdList.RemoveTail(),MAX_CMD_LEN);
		bRet = true;
		LOG("GetCmd(): %s",pCmd);
	}
	
	// Unlock
	lock.Unlock();
	return bRet;
}
bool CSyncTransceiver::SendCmd(const char * pCmd,int nCmdLen/* = -1*/)
{
	char szCmdLF[MAX_CMD_LEN+1];
	int nnCmdLen = nCmdLen;
	if(nnCmdLen < 0)
		nnCmdLen = strlen(pCmd);
	sprintf(szCmdLF,"%s\n",pCmd);
	if(nnCmdLen >= MAX_CMD_LEN) return false;
	nnCmdLen++;

	LOG("SendCmd(): %s",pCmd);
	return m_pSock->Send(szCmdLF,nnCmdLen) == nnCmdLen;
}
	





/////////////////////////////////////////////////////////////////////////////
// CSyncSocket

CSyncSocket::CSyncSocket() : m_pISocketCallback(NULL),m_bCreated(false)
{
}

CSyncSocket::~CSyncSocket()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSyncSocket member functions

void CSyncSocket::OnAccept(int nErrorCode) 
{
	if(nErrorCode == 0 && m_pISocketCallback)
	{
		m_pISocketCallback->OnAccept();
	}
	CAsyncSocket::OnAccept(nErrorCode);
}

void CSyncSocket::OnClose(int nErrorCode) 
{
	if(m_pISocketCallback)
	{
		m_pISocketCallback->OnDisconnect(); 
	}	
	CAsyncSocket::OnClose(nErrorCode);
}

void CSyncSocket::OnConnect(int nErrorCode) 
{
	if(nErrorCode == 0 && m_pISocketCallback)
	{
		m_pISocketCallback->OnConnect();
	}
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CSyncSocket::OnOutOfBandData(int nErrorCode) 
{
	if(nErrorCode == 0 && m_pISocketCallback)
	{
		m_pISocketCallback->OnOutOfBandData();
	}
	
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CSyncSocket::OnReceive(int nErrorCode) 
{
	if(nErrorCode == 0 && m_pISocketCallback)
	{
		m_pISocketCallback->OnReceive();
	}
}

void CSyncSocket::OnSend(int nErrorCode) 
{
	if(nErrorCode == 0 && m_pISocketCallback)
	{
		m_pISocketCallback->OnSend();
	}
}



BOOL CSyncSocket::CreateServerSocket(UINT nSockPort,ISocketCallback *pISockCallback)
{
	if(m_bCreated) return FALSE;

	m_pISocketCallback = pISockCallback;
	m_bCreated = true;
	return CAsyncSocket::Create(nSockPort);
}

BOOL CSyncSocket::CreateClientSocket(ISocketCallback *pISockCallback)
{
	if(m_bCreated) return FALSE;
	m_pISocketCallback = pISockCallback;
	m_bCreated = true;
	return TRUE;
}
