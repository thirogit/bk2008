#include "stdafx.h"
#include "BufferTransfer.h"

CBufferTransfer::CBufferTransfer(void *pBuffer,UINT uSize)
{
	ASSERT(pBuffer);
	ASSERT(uSize > 0);
	m_pBufffer = pBuffer;
	m_uSize = uSize;
	m_bytesSent = 0;
	m_bytesRecvd = 0;
}
void CBufferTransfer::OnBytesReceived(const nsFTP::TByteVector& vBuffer, long lReceivedBytes)
{
	UINT bytesToWrite = lReceivedBytes;
	if(m_bytesRecvd+bytesToWrite >= m_uSize)
	{
		bytesToWrite = m_uSize-m_bytesRecvd;
	}
	memcpy(((UCHAR*)m_pBufffer)+m_bytesRecvd,&(*vBuffer.begin()), bytesToWrite);
	m_bytesRecvd+=bytesToWrite;
}
void CBufferTransfer::OnPreBytesSend(nsFTP::TByteVector& vBuffer, size_t& bytesToSend)
{
	if(m_bytesSent < m_uSize)
	{
		int bytesLeftToSend = (m_uSize-m_bytesSent);
		if(bytesLeftToSend < 0) bytesLeftToSend = 0;
		UINT bufferSize = vBuffer.size();
		bytesToSend = bytesLeftToSend > bufferSize ? bufferSize : bytesLeftToSend;
		memcpy(&(*vBuffer.begin()),((UCHAR*)m_pBufffer) + m_bytesSent, bytesToSend);
		m_bytesSent += bytesToSend;
	}
	else
	{
		bytesToSend = 0;
	}
}