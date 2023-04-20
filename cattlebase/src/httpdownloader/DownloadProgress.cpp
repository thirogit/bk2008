#include "stdafx.h"
#include "DownloadProgress.h"


DownloadProgress::DownloadProgress(const CString& URL,ITaskProgres* pProgress) : m_pProgress(pProgress)
{
	CString taskTitle;
	taskTitle.Format(IDS_DOWNLOADINGURL,(LPCSTR)URL);
	if(m_pProgress)
		m_pProgress->SetTask((UINT)(LPCSTR)taskTitle,100);	
}

void DownloadProgress::OnError(LPCSTR szErrorMessage,DWORD dwLastError)
{
	CString errorStr;
	if(dwLastError)
		errorStr.Format("%s Error Code %d",szErrorMessage,dwLastError);
	else
		errorStr = szErrorMessage;
	if(m_pProgress)
		m_pProgress->OnTaskError(errorStr);
}
void DownloadProgress::OnStatus(HINTERNET hInternet, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
}
void DownloadProgress::OnDownloadProgress(UINT bytesTotal,UINT bytesReceived)
{
	if(m_pProgress)
		m_pProgress->OnStep((bytesReceived*100)/bytesTotal);
}