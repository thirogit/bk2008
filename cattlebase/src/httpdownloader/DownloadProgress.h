#ifndef __DOWNLOADPROGRESS_H__
#define  __DOWNLOADPROGRESS_H__

#include "HttpFileGetter.h"
#include "../taskprogress/ProgressIFace.h"

class DownloadProgress : public CHttpFileGetterNotification
{
public:
	DownloadProgress(const CString& URL,ITaskProgres* pProgress);
	
	virtual void OnError(LPCSTR szErrorMessage,DWORD dwLastError);
	virtual void OnStatus(HINTERNET hInternet, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	virtual void OnDownloadProgress(UINT bytesTotal,UINT bytesReceived);
private:
	ITaskProgres* m_pProgress;
};

#endif