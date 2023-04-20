#if !defined(__HTTPVERSIONINFO_H__)
#define __HTTPVERSIONINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h>
#include "../taskprogress/ProgressIFace.h"

class CHttpVersionInfo
{
private:
	CHttpVersionInfo();
	static CHttpVersionInfo m_instance;

	static void CALLBACK _OnStatusCallBack(HINTERNET hInternet, DWORD dwContext, DWORD dwInternetStatus, 
                                        LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	void OnStatusCallBack(HINTERNET hInternet, DWORD dwInternetStatus, 
                        LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
 

public:
	static CHttpVersionInfo* GetInstance();	
	 bool CheckForNewerVersion(ITaskProgres *pProgress);
};


#endif 
