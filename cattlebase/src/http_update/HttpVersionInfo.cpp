#include "StdAfx.h"
#include "HttpVersionInfo.h"
#include "../log.h"
#include "HttpVersionCfg.h"
#include "../httpdownloader/HttpFileGetter.h"
#include "../httpdownloader/TransferBuffer.h"
#include <memory>
#include "../../../tools/UpdateInfoEdit/src/VersionInfoStructFileTypes.h"
#include "../VersionNo.h"
#include "httpdownloader/DownloadProgress.h"
#include "../classes/TempStringLoader.h"

CHttpVersionInfo CHttpVersionInfo::m_instance;

CHttpVersionInfo::CHttpVersionInfo()
{
}

CHttpVersionInfo* CHttpVersionInfo::GetInstance()
{
	return &m_instance;
}

bool CHttpVersionInfo::CheckForNewerVersion(ITaskProgres *pProgress)
{
	LOG("CHECKING FOR NEWER CATTLEBASE VERSION, CURRENT %d",VERSION_EXTEND);
	CHttpVersionCfg *cfg = CHttpVersionCfg::GetInstance();
	if(!cfg)
	{
		LOG("FAILD TO GET VERSION INFO CONFIGURATION!");
		return false;
	}

	DownloadProgress downloadProgress(CTempStringLoader(IDS_CHECKINGUPDATE),pProgress);
	CHttpFileGetter verGetter(cfg->GetHttpVersionInfoURL(),&downloadProgress);
	long lContentLength = verGetter.GetContentLength();
	if(lContentLength > 0)
	{
		UINT uContentLength = (UINT)lContentLength;
		BYTE *pBuffer = new BYTE[uContentLength];
		std::auto_ptr<BYTE> autoBuffer(pBuffer);
		CTransferBuffer transferBuffer(pBuffer,uContentLength);
		if(verGetter.Download(transferBuffer))
		{
			VERSION_INFO_HEADER* pVIHeader = (VERSION_INFO_HEADER*)pBuffer;
			if(!strcmp(VERSION_INFO_HEADER_ID,pVIHeader->id))
			{
				if(pVIHeader->size == uContentLength)
				{
					return VERSION_EXTEND < pVIHeader->max_version_no;
				}	
				else
				{
					LOG("VERSION INFO FILE IS CORUPTED.");
				}
			}
		}
		else
		{
			LOG("FAILD TO GET VERSION INFO CONFIGURATION!");
		}
	}
	else
	{
		LOG("FAILD TO GET VERSION INFO FILE SIZE.");
	}

	return false;
}

