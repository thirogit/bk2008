#ifndef __BUFFERTRANSFER_H__
#define __BUFFERTRANSFER_H__

#include "../ftp/FTPClient.h"
#include "../ftp/FTPDataTypes.h"

class CBufferTransfer : public nsFTP::ITransferNotification
{
public:
	CBufferTransfer(void *pBuffer,UINT uSize);
protected:
	virtual void OnBytesReceived(const nsFTP::TByteVector& vBuffer, long lReceivedBytes);
	virtual void OnPreBytesSend(nsFTP::TByteVector& vBuffer, size_t& bytesToSend);
private:
	void * m_pBufffer;
	UINT m_uSize;
	UINT m_bytesSent;
	UINT m_bytesRecvd;
};


#endif