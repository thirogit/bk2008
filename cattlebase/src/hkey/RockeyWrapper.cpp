#include "stdafx.h"
#include "RockeyWrapper.h"
#include <Ry4S.h>

RockeyWrapper::RockeyWrapper() : m_wHandle(INVALID_RY_HANDLE),m_dwKeySerialNo(0)
{
}

DWORD RockeyWrapper::GetKeySerialNo() const
{
	return m_dwKeySerialNo;
}

bool RockeyWrapper::IsOpen() const
{
	return m_wHandle != INVALID_RY_HANDLE;
}

bool RockeyWrapper::Open(WORD wBasicPwd1,WORD wBasicPwd2)
{
	Close();
	
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = wBasicPwd1;
	WORD  wP2   = wBasicPwd2;
	WORD  wP3   = 0;
	WORD  wP4   = 0;
	DWORD dwKeySerialNo = 0;
	WORD  wHandle = INVALID_RY_HANDLE;

	DWORD dwRet = 0;
	
	dwRet = Rockey(RY_FIND,&wHandle,&dwLP1,&dwLP2,&wP1,&wP2,&wP3,&wP4,NULL);
	if (dwRet != ERR_SUCCESS)
	{
		Rockey(RY_CLOSE,&wHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return false;

	}
	
	dwRet = Rockey(RY_OPEN,&wHandle,&dwLP1,&dwLP2,&wP1,&wP2,&wP3,&wP4,NULL);
	if (dwRet != ERR_SUCCESS)
	{
		Rockey(RY_CLOSE,&wHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return false;
	}
	
	dwKeySerialNo = dwLP1;

	dwRet = Rockey(RY_VERSION,&wHandle,&dwLP1,&dwLP2,&wP1,&wP2,&wP3,&wP4,NULL);
	if (dwRet)
	{
		Rockey(RY_CLOSE,&wHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return false;
	}
	if (dwLP2 < 259)
	{
		Rockey(RY_CLOSE,&wHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return false;
	}

	m_dwKeySerialNo = dwKeySerialNo;
	m_wHandle = wHandle;
	return true;
}

bool RockeyWrapper::Close()
{
	DWORD dwRet = ERR_UNKNOWN;
	if(IsOpen())
	{
		dwRet = Rockey(RY_CLOSE,&m_wHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		m_dwKeySerialNo = 0;
		m_wHandle = INVALID_RY_HANDLE;
	}
	return dwRet == ERR_SUCCESS;
}

void RockeyWrapper::ConvertTime(SYSTEMTIME* pOutSystemTime, const struct tm* t)
{
	pOutSystemTime->wSecond = t->tm_sec;
	pOutSystemTime->wMinute = t->tm_min;
	pOutSystemTime->wHour = t->tm_hour;
	pOutSystemTime->wDay = t->tm_mday;
	pOutSystemTime->wMonth = t->tm_mon+1;
	pOutSystemTime->wYear = t->tm_year+1900;
}

bool RockeyWrapper::AdjustClock(time_t t)
{
	const int BUFFER_SIZE = 500;
	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = 0;
	WORD  wP2   = 0;
	WORD  wP3   = 0;
	WORD  wP4   = 0;
	time_t currentTime = t;

	if(!IsOpen())
		return false;
		
	
	BYTE bBuffer[BUFFER_SIZE];
	memset(bBuffer,0,BUFFER_SIZE);

	SYSTEMTIME* pRyTimeClock = (SYSTEMTIME*)bBuffer;
	struct tm clock;
	localtime_s(&clock,&currentTime);
	ConvertTime(pRyTimeClock,&clock);
		 	
	
	dwRet = Rockey(RY_ADJUST_TIMER_EX,&m_wHandle,&dwLP1,&dwLP2,&wP1,&wP2,&wP3,&wP4,(BYTE*)pRyTimeClock);
	return (dwRet == ERR_SUCCESS);
}

bool RockeyWrapper::Read(BYTE* pOutBuffer,UINT uMaxSize,UINT uOffset)
{

	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = (WORD)uOffset;
	WORD  wP2   = (WORD)uMaxSize;
	WORD  wP3   = 0;
	WORD  wP4   = 0;

	if(!IsOpen())
		return false;
		
	dwRet = Rockey(RY_READ_EX, &m_wHandle, &dwLP1, &dwLP2, &wP1, &wP2, &wP3, &wP4, pOutBuffer);
	return (dwRet == ERR_SUCCESS);
	
}

bool RockeyWrapper::Write(const BYTE* pBuffer,UINT uSize,UINT uOffset)
{
	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = (WORD)uOffset;
	WORD  wP2   = (WORD)uSize;
	WORD  wP3   = 0;
	WORD  wP4   = 0;

	if(!IsOpen())
		return false;

	BYTE* pBufferForWrite = const_cast<BYTE*>(pBuffer);

	dwRet = Rockey(RY_WRITE, &m_wHandle,&dwLP1,&dwLP2,&wP1,&wP2,&wP3,&wP4, pBufferForWrite);
	return (dwRet == ERR_SUCCESS);
}


#if 0



time_t getClock(WORD wHandle)
{
	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = 0;
	WORD  wP2   = 0;
	WORD  wP3   = 0;
	WORD  wP4   = 0;

	DWORD dwClock = 0;

	wP1 = ROCKEY_CLOCK_BASE_ADDRESS;
	wP2 = 4;

	dwRet = Rockey(RY_READ, &wHandle, &dwLP1, &dwLP2, &wP1, &wP2, &wP3, &wP4, (BYTE*)&dwClock);
	if (dwRet != ERR_SUCCESS)
	{
		return 0;
	}

	struct tm ryBaseTm;
	memset(&ryBaseTm,0,sizeof(struct tm));

	
	ryBaseTm.tm_min = 0;
	ryBaseTm.tm_sec = 0;
	ryBaseTm.tm_hour = 0;
	ryBaseTm.tm_year = 2006-1900;
	ryBaseTm.tm_mday = 1;
	ryBaseTm.tm_mon = 0;

	time_t ryBaseTime = mktime(&ryBaseTm);

	time_t clockTm = dwClock*60 + ryBaseTime;

	return clockTm;
}

time_t getExpiryDt(WORD wHandle)
{
	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = 0;
	WORD  wP2   = 0;
	WORD  wP3   = 0;
	WORD  wP4   = 0;

	wP1 = 0;//Time unit number based on 0
	wP3 = 1;//Time flag. 1 means date,2 means hour
	wP2 = 0;
	wP4 = 0;
	dwLP1 = 0;
	dwLP2 = 0;

	SYSTEMTIME expiryDt;
	memset(&expiryDt,0,sizeof(SYSTEMTIME));

	dwRet = Rockey(RY_GET_TIMER_EX,&wHandle,&dwLP1,&dwLP2,&wP1,&wP2,&wP3,&wP4,(BYTE*)(&expiryDt));

	if (dwRet != ERR_SUCCESS || wP3 != 1)
	{
		return 0;
	}

	struct tm tmExpiryDt;
	memset(&tmExpiryDt,0,sizeof(struct tm));

	tmExpiryDt.tm_sec = 59;
	tmExpiryDt.tm_min = 59; 
	tmExpiryDt.tm_hour = 23; 

	tmExpiryDt.tm_mday = expiryDt.wDay; 
	tmExpiryDt.tm_mon = expiryDt.wMonth-1;
	tmExpiryDt.tm_year = expiryDt.wYear-1900;

	return mktime(&tmExpiryDt);

}




bool adjustClockWithTime(WORD wHandle,time_t t)
{
	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = 0;
	WORD  wP2   = 0;
	WORD  wP3   = 0;
	WORD  wP4   = 0;
	
	time_t currentTime = t;
		
	const int BUFFER_SIZE = 500;
	BYTE bBuffer[BUFFER_SIZE];
	memset(bBuffer,0,BUFFER_SIZE);

	SYSTEMTIME* pRyTimeClock = (SYSTEMTIME*)bBuffer;
	struct tm clock;
	localtime_s(&clock,&currentTime);
	convertTime(pRyTimeClock,&clock);
		 	
	wP1 = 0;
	wP2 = 0;
	wP3 = 0;
	wP4 = 0;
	dwLP1 = 0;
	dwLP2 = 0;

	
	dwRet = Rockey(RY_ADJUST_TIMER_EX,&wHandle,&dwLP1,&dwLP2,&wP1,&wP2,&wP3,&wP4,(BYTE*)pRyTimeClock);
	if (dwRet != ERR_SUCCESS)
	{
		return false;
	}


	DWORD dwClock = 0;
	dwLP2 = clock.tm_year+1900;
	wP1 = clock.tm_mon+1;
	wP2 = clock.tm_mday;
	wP3 = clock.tm_hour;
	wP4 = clock.tm_min;
	
	dwRet = Rockey(RY_GET_TIME_DWORD, &wHandle, (DWORD *)&dwClock, &dwLP2, &wP1, &wP2, &wP3, &wP4, NULL);
	if (dwRet != ERR_SUCCESS)
	{
		return false;
	}

	wP1 = ROCKEY_CLOCK_BASE_ADDRESS;
	wP2 = 4;
	wP3 = 0;
	wP4 = 0;
	dwLP1 = 0;
	dwLP2 = 0;
	

	dwRet = Rockey(RY_WRITE, &wHandle,&dwLP1,&dwLP2,&wP1,&wP2,&wP3,&wP4, (BYTE*)&dwClock);
	if (dwRet != ERR_SUCCESS)
	{
		return false;
	}

	return true;
}

WORD getRequiredVersion(WORD wHandle)
{
	WORD wReqVersion = 0;

	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = ROCKEY_REQVER_BASE_ADDRESS_EX;
	WORD  wP2   = 2;
	WORD  wP3   = 0;
	WORD  wP4   = 0;
	
	dwRet = Rockey(RY_READ_EX, &wHandle, &dwLP1, &dwLP2, &wP1, &wP2, &wP3, &wP4, (BYTE*)&wReqVersion);
	if (dwRet != ERR_SUCCESS)
	{
		return 0;
	}
	return wReqVersion;
}

bool readClientInfo(PROCKEY_BKCLIENTINFO pClientInfo,WORD wHandle)
{
	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = ROCKEY_CLIENTINFO_BASE_ADDRESS_EX;
	WORD  wP2   = sizeof(ROCKEY_BKCLIENTINFO);
	WORD  wP3   = 0;
	WORD  wP4   = 0;
		
	dwRet = Rockey(RY_READ_EX, &wHandle, &dwLP1, &dwLP2, &wP1, &wP2, &wP3, &wP4, (BYTE*)pClientInfo);
	if (dwRet != ERR_SUCCESS)
	{
		return false;
	}
	return true;
	
}

bool updateKey(WORD wHandle,BYTE* pUpdateContent,int iUpdateContentLength)
{
	DWORD dwRet = 0;
	DWORD dwLP1 = 0;
	DWORD dwLP2 = 0;
	WORD  wP1   = 0;//ROCKEY_BASIC_PASSWD_1;
	WORD  wP2   = 0;//ROCKEY_BASIC_PASSWD_2;
	WORD  wP3   = 0;
	WORD  wP4   = 0;
		
	dwRet = Rockey(RY_UPDATE_EX, &wHandle, &dwLP1, &dwLP2, &wP1, &wP2, &wP3, &wP4, (BYTE*)pUpdateContent);
	if (dwRet != ERR_SUCCESS)
	{
		return false;
	}
	return true;
}

size_t decodeBase64(BYTE* pbBase64Decoded,const char* pbBase64Encoded,size_t encodedSize)
{
	size_t outSize = 0;
	if(!base64_decode(pbBase64Encoded,encodedSize,pbBase64Decoded,&outSize))
	{
		outSize = (size_t)-1;
	}
	return outSize;
}

bool updateKey(WORD wHandle,DWORD keySerialNo)
{

	ROCKEY_BKCLIENTINFO clientInfo;
	if(!readClientInfo(&clientInfo,wHandle))
	{
		return false;
	}

	KeyServiceBinding* binding = new KeyServiceBinding();
	binding->endpoint = "http://localhost:8181/key";


	ns1__validateKeyRequest *ns1__ValidateKeyRQ = soap_new_ns1__validateKeyRequest(binding->soap,-1);
	ns1__validateKeyResponse *ns1__ValidateKeyRS = soap_new_ns1__validateKeyResponse(binding->soap,-1);

	ns1__ValidateKeyRQ->KeySerialNo = keySerialNo;
	ns1__ValidateKeyRQ->BkVersion = 2500;
	ns1__ValidateKeyRQ->CompanyName = clientInfo.szCompany;
	ns1__ValidateKeyRQ->FarmNo = clientInfo.szFarmNo;
	ns1__ValidateKeyRQ->HerdNo = 1;
	ns1__ValidateKeyRQ->Street = clientInfo.szStreet;
	ns1__ValidateKeyRQ->PoBox = clientInfo.szPOBox;
	ns1__ValidateKeyRQ->City = clientInfo.szCity;
	ns1__ValidateKeyRQ->Zip = clientInfo.szZipCode;
	
	int soapResult = binding->__ns1__validateKey(ns1__ValidateKeyRQ,ns1__ValidateKeyRS);
	soap_print_fault(binding->soap,stdout);
	
	if(soapResult == SOAP_OK)
	{
		if(ns1__ValidateKeyRS->updateContentBase64 != NULL)
		{
			BYTE* pbUpdateContent = new BYTE[ns1__ValidateKeyRS->updateContentBase64->length()];
			bool bUpdateResult = false;
			int decodedLength = decodeBase64(pbUpdateContent,ns1__ValidateKeyRS->updateContentBase64->c_str(),ns1__ValidateKeyRS->updateContentBase64->length());
			if(decodedLength != (size_t)-1)
			{
				bUpdateResult = updateKey(wHandle,pbUpdateContent,decodedLength);
			}
			delete[] pbUpdateContent;
			return bUpdateResult;
		}
	}
	return false;

}

bool updateKey2(WORD wHandle,DWORD keySerialNo)
{
	BYTE* pbUpdateContent = new BYTE[1024];
	FILE* updateFile = fopen("c:\\update.ry4s","rb");
	fread(pbUpdateContent,1024,1,updateFile);
	fclose(updateFile);
	bool bUpdateResult = updateKey(wHandle,pbUpdateContent,0);

	return bUpdateResult;
}

void discoverClock(WORD wHandle)
{
	struct tm  startTm;
	memset(&startTm,0,sizeof(struct tm));

	startTm.tm_year = 2011-1900;
	startTm.tm_mon = 11;
	startTm.tm_mday = 1;
	startTm.tm_hour = 0;
	startTm.tm_min = 0;
	startTm.tm_sec = 0;

	char bTimeBuffer[50];
	strftime(bTimeBuffer,50,"%Y/%m/%d %H:%M:%S",&startTm);
	printf("startTime = %s\n",bTimeBuffer);

	time_t clockTest = mktime(&startTm);
	struct tm clockTestTm;

	while(!adjustClockWithTime(wHandle,clockTest))
	{
		clockTest += ((60*60)*24);
		localtime_s(&clockTestTm,&clockTest);
		strftime(bTimeBuffer,50,"%Y/%m/%d %H:%M:%S",&clockTestTm);
		printf("testing = %s\n",bTimeBuffer);
		Sleep(100);

	}

	printf("stop.\n");
	

}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD keySerialNo = 0;
	WORD wKeyHandle;

	if(openKey(&wKeyHandle,&keySerialNo))
	{

		discoverClock(wKeyHandle);
		//updateKey2(wKeyHandle,keySerialNo);
		//closeKey(wKeyHandle);
		getchar();
		return 0;

		time_t clockTime = getClock(wKeyHandle);
		struct tm tmClock;

		localtime_s(&tmClock,&clockTime);
		char szVerboseClockTime[20];							
		strftime(szVerboseClockTime,20,"%Y/%m/%d %H:%M:%S",&tmClock);
		_tprintf(_T("Clock is %s\n"),szVerboseClockTime);	


		time_t currentTime = time(NULL);

		if(false)//currentTime < clockTime)
		{
			_tprintf(_T("Current time is before key clock.\n"));
		}
		else
		{
			if(currentTime-clockTime > 30*24*60*60)
			{
				_tprintf(_T("Clock is more than 30 days old.\n"));
			}
			else
			{
				if(!adjustClockWithTime(wKeyHandle,time(NULL)))
				{
					_tprintf(_T("Failed to adjust clock.\n"));
				}
				else
				{
					time_t expiryDt = getExpiryDt(wKeyHandle);
					if(expiryDt == (time_t)-1)
					{
						_tprintf(_T("Unable to determine expiry date.\n"));
					}
					else
					{
						if(currentTime > expiryDt)
						{
							_tprintf(_T("Key is expired.\n"));
						}
						else
						{
							_tprintf(_T("Key is valid.\n"));

							updateKey(wKeyHandle,keySerialNo);

							ROCKEY_BKCLIENTINFO clientInfo;
							if(readClientInfo(&clientInfo,wKeyHandle))
							{
								_tprintf(_T("Company: %s\n"),clientInfo.szCompany);
								_tprintf(_T("Street: %s\n"),clientInfo.szStreet);
								_tprintf(_T("POBox: %s\n"),clientInfo.szPOBox);
								_tprintf(_T("City: %s\n"),clientInfo.szCity);
								_tprintf(_T("ZipCode: %s\n"),clientInfo.szZipCode);
								_tprintf(_T("NIP: %s\n"),clientInfo.szNIP);
								_tprintf(_T("FarmNo: %s\n"),clientInfo.szFarmNo);
								_tprintf(_T("Client Id: %04X\n"),clientInfo.wClientId);
												

							}


						}

					}

				}
			}
		}

		closeKey(wKeyHandle);
	}	
		
	getchar();

	return 0;
}

#endif