#include "stdafx.h"
#include <time.h>
#include "common/console.h"
#include "common/htoi.h"
#include "Rockey.h"
#include "common/strptime.h"

int askUser(TCHAR* szAnswer,const TCHAR* pszAskFor,int iMaxAnswerLength)
{
	_tprintf(_T("\n%s (max %d chars): "),pszAskFor,iMaxAnswerLength);
	return readFromConsole(szAnswer,iMaxAnswerLength);
}

bool askYN(TCHAR* szQuestion)
{
	_tprintf(_T("\n%s? (Y/N):"),szQuestion);

	TCHAR yes_no;
	while(1)
	{
		readFromConsole(&yes_no,1); 	
	
		if (yes_no == 'y' || yes_no == 'Y')
		{
			_tprintf(_T("\nAnswered - Yes\n"));
			return true;
		}
		else if(yes_no == 'n' || yes_no == 'N')
		{
			_tprintf(_T("\nAnswered - No\n"));
			return false;
		}
		else
		{
			_tprintf(_T("\nInvalid answer, expecing Y or y - for Yes, N or n for No \n"));
		}
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	int nRetCode = 0;
		
	ROCKEY_BKCLIENTINFO bkInfo;

	TCHAR szExpiryDt[11];
	TCHAR szClientId[5];
	TCHAR szRequiredVersion[5];
	WORD wFeaturesEnabled = FEATURE_MASK;
	TCHAR yes_no;
	WORD wClientId = 0;
	WORD wRequiredBKVersion= 0;
	time_t tmExpiryTime = 0;

	int l;

	struct _feature_list
	{
		TCHAR *feature_name;
		WORD feature_mask;
	} 
	features[] = 
	{
		{_T("OCR"),				1 << FEATURE_OCR			},
		{_T("COLLECTOR"),		1 << FEATURE_COLLECTOR		},
		{NULL,					0							}
	};
	
	
	

	_feature_list *feature = features;
	while(feature->feature_name)
	{
		while(1)
		{
			_tprintf(_T("\nIs feature %s enabled (Y/N): "),feature->feature_name);
			readFromConsole(&yes_no,1); 

			if(yes_no == 'y' || yes_no == 'Y')
			{
				wFeaturesEnabled |= feature->feature_mask;
				feature++;
				break;
			}
			else if(yes_no == 'n' || yes_no == 'N')
			{
				feature++;
				break;
			}
		}
	}

	while(1)
	{

		_tprintf(_T("\nClient Id (16bit hex, ex. A8E5; RAND for random): "),4);
		l = readFromConsole(szClientId,4);
		szClientId[l] = 0;

		if(_tcsicmp(_T("RAND"),szClientId) == 0)
		{
			srand((UINT)time(0));
			wClientId = rand();
			_tprintf(_T("\nClient Id is %04X\n"),wClientId);
			break;
		}
		else
		{
			UINT uiClientId;
			if(htoi(szClientId,&uiClientId))
			{
				wClientId = uiClientId;
				_tprintf(_T("\n"));
				break;
			}
			else
				_tprintf(_T("\nInvalid ClientId."));

		}
	}

	memset(&bkInfo,0,sizeof(bkInfo));	

	askUser(bkInfo.szCompany,"Company",COMPANY_MAX_LEN);
	askUser(bkInfo.szStreet,"Street",STREET_MAX_LEN);	
	askUser(bkInfo.szPOBox,"POBox",POBOX_MAX_LEN);
	askUser(bkInfo.szCity,"City",CITY_MAX_LEN);
	askUser(bkInfo.szZipCode,"Zip",ZIPCODE_MAX_LEN);
	askUser(bkInfo.szNIP,"NIP",NIP_MAX_LEN);
	askUser(bkInfo.szFarmNo,"Farm No",FARMNO_MAX_LEN);

	
	/*strcpy(bkInfo.szCompany,"\"ARTPOL\" Arkadiusz Gêbicki");
	strcpy(bkInfo.szStreet,"Taraska");	
	strcpy(bkInfo.szPOBox,"11");
	strcpy(bkInfo.szCity,"Aleksandrów");
	strcpy(bkInfo.szZipCode,"26-337");
	strcpy(bkInfo.szNIP,"7712327592");
	strcpy(bkInfo.szFarmNo,"PL054832560002");*/


	/*strcpy(bkInfo.szCompany,"SKUP I SPRZEDA¯ ¯YWCA UBÓJ I SPRZEDA¯ POUBOJOWA JAN BASTA");
	strcpy(bkInfo.szStreet,"GRÓDEK n/DUNAJCEM");	
	strcpy(bkInfo.szPOBox,"42");
	strcpy(bkInfo.szCity,"GRÓDEK");
	strcpy(bkInfo.szZipCode,"33-318");
	strcpy(bkInfo.szNIP,"7341012901");
	strcpy(bkInfo.szFarmNo,"PL038362636001");*/

	/*strcpy(bkInfo.szCompany,"\"Eko-Natura\" Sp. z o.o.");
	strcpy(bkInfo.szStreet,"Moszczenica Ni¿na");	
	strcpy(bkInfo.szPOBox,"9");
	strcpy(bkInfo.szCity,"Stary S¹cz");
	strcpy(bkInfo.szZipCode,"33-340");
	strcpy(bkInfo.szNIP,"7342823079");
	strcpy(bkInfo.szFarmNo,"PL030919740001");*/

	
	/*strcpy(bkInfo.szCompany,"Skup i Sprzeda¿ Zwierz¹t RiH, Handel Miêsem Józef Sikora");
	strcpy(bkInfo.szStreet,"Koziniec");	
	strcpy(bkInfo.szPOBox,"89");
	strcpy(bkInfo.szCity,"MUCHARZ");
	strcpy(bkInfo.szZipCode,"34-106");
	strcpy(bkInfo.szNIP,"5511140963");
	strcpy(bkInfo.szFarmNo,"PL040573590002");*/


	/*strcpy(bkInfo.szCompany,"\"BYKPOL\" Marzena Pryszlak");
	strcpy(bkInfo.szStreet,"£aniewo");	
	strcpy(bkInfo.szPOBox,"58/1");
	strcpy(bkInfo.szCity,"Lidzbark Warmiñski");
	strcpy(bkInfo.szZipCode,"11-100");
	strcpy(bkInfo.szNIP,"7431813289");
	strcpy(bkInfo.szFarmNo,"PL063762172005");*/


	/*strcpy(bkInfo.szCompany,"CHOROMAÑSKI PAWE£");
	strcpy(bkInfo.szStreet,"KWIATOWA");	
	strcpy(bkInfo.szPOBox,"10");
	strcpy(bkInfo.szCity,"KLECZKOWO");
	strcpy(bkInfo.szZipCode,"07-405");
	strcpy(bkInfo.szNIP,"7582048941");
	strcpy(bkInfo.szFarmNo,"PL061096463002");*/


	/*strcpy(bkInfo.szCompany,"Novexim Nowacki Piotr Pawe³, Nowacki Micha³ s.c.");
	strcpy(bkInfo.szStreet,"Gombrowicza");	
	strcpy(bkInfo.szPOBox,"11");
	strcpy(bkInfo.szCity,"Ciechanów");
	strcpy(bkInfo.szZipCode,"06-400");
	strcpy(bkInfo.szNIP,"5661937114");
	strcpy(bkInfo.szFarmNo,"PL061723903002");*/


	/*strcpy(bkInfo.szCompany,"SKUP I SPRZEDA¯ ZWIERZ¥T RZENYCH I HODOWLANYCH JERZY RYNDAK");
	strcpy(bkInfo.szStreet,"OLSZYNY");	
	strcpy(bkInfo.szPOBox,"420");
	strcpy(bkInfo.szCity,"OLSZYNY");
	strcpy(bkInfo.szZipCode,"33-164");
	strcpy(bkInfo.szNIP,"6850007575");
	strcpy(bkInfo.szFarmNo,"PL038393181002");*/


	//strcpy(bkInfo.szCompany,"Sprzeda¿ hurtowa ¿ywych zwierz¹t T.T.D.P.S");
	//strcpy(bkInfo.szStreet,"B. Prusa");	
	//strcpy(bkInfo.szPOBox,"3");
	//strcpy(bkInfo.szCity,"Szczucin");
	//strcpy(bkInfo.szZipCode,"33-230");
	//strcpy(bkInfo.szNIP,"8711268445");
	//strcpy(bkInfo.szFarmNo,"PL040808064002");


	/*strcpy(bkInfo.szCompany,"F.H.U. PAWMARPOL DOBEK PAWE£");
	strcpy(bkInfo.szStreet,"WOLA GRÊBOSZOWSKA");	
	strcpy(bkInfo.szPOBox,"91");
	strcpy(bkInfo.szCity,"GRÊBOSZÓW");
	strcpy(bkInfo.szZipCode,"33-260");
	strcpy(bkInfo.szNIP,"9930231201");
	strcpy(bkInfo.szFarmNo,"PL056714570004");*/

	/*strcpy(bkInfo.szCompany,"F.H.U. PAWMARPOL DOBEK PAWE£");
	strcpy(bkInfo.szStreet,"GOS£AWICE");	
	strcpy(bkInfo.szPOBox,"117");
	strcpy(bkInfo.szCity,"WIERZCHOS£AWICE");
	strcpy(bkInfo.szZipCode,"33-122");
	strcpy(bkInfo.szNIP,"9930231201");
	strcpy(bkInfo.szFarmNo,"PL056714570004");*/

	/*strcpy(bkInfo.szCompany,"SKUP-SPRZEDA¯ ZWIERZ¥T RZE¯NYCH I HODOWLANYCH KRZYSZTOF ŒLÊZAK");
	strcpy(bkInfo.szStreet,"KRAKOWSKA");	
	strcpy(bkInfo.szPOBox,"68");
	strcpy(bkInfo.szCity,"BARANÓW SANDOMIERSKI");
	strcpy(bkInfo.szZipCode,"39-450");
	strcpy(bkInfo.szNIP,"8671939642");
	strcpy(bkInfo.szFarmNo,"PL038126891001");*/


	/*strcpy(bkInfo.szCompany,"P.H.U. TRANS¯YW Wojciech Maciejewski");
	strcpy(bkInfo.szStreet,"Grobla");	
	strcpy(bkInfo.szPOBox,"42");
	strcpy(bkInfo.szCity,"S³upca");
	strcpy(bkInfo.szZipCode,"62-400");
	strcpy(bkInfo.szNIP,"6670001482");
	strcpy(bkInfo.szFarmNo,"PL031749854001");*/


	strcpy(bkInfo.szCompany,"Daniel Markiewicz SKUP I SPRZEDA¯ BYD£A");
	strcpy(bkInfo.szStreet,"Suchedniowska");	
	strcpy(bkInfo.szPOBox,"70");
	strcpy(bkInfo.szCity,"Bodzentyn");
	strcpy(bkInfo.szZipCode,"26-010");
	strcpy(bkInfo.szNIP,"6572730234");
	strcpy(bkInfo.szFarmNo,"PL068090382001");

	bkInfo.wClientId = wClientId;
	bkInfo.wFeaturesEnabled = wFeaturesEnabled;

	
	int iRequiredVersion = 0;
	while(1)
	{
		l = askUser(szRequiredVersion,"Required version",4);
		
		iRequiredVersion = atoi(szRequiredVersion);
		if(iRequiredVersion > 0)
		{
			wRequiredBKVersion = iRequiredVersion;
			break;
		}
		else
		{
			_tprintf(_T("\nInvalid version."));
		}
	}

	struct tm tmExpiry;
	do
	{
		l = askUser(szExpiryDt,"Expiry date yyyy/mm/dd",10);
		szExpiryDt[l] = 0;

		if(!strptime(szExpiryDt,"%Y/%m/%d",&tmExpiry))
		{
			_tprintf(_T("\nInvalid expiry date."));	
			continue;
		}
		else
		{
			char szVerboseExpiryTime[20];
			tmExpiry.tm_hour = 23;
			tmExpiry.tm_min = 59;
			tmExpiry.tm_sec = 59;
			tmExpiryTime = _mkgmtime(&tmExpiry);
			strftime(szVerboseExpiryTime,20,"%Y/%m/%d %H:%M:%S",&tmExpiry);
			_tprintf(_T("\nExpiry date is %s"),szVerboseExpiryTime);			
		}
	} while(!askYN("Correct"));


	TCHAR szClock[20];
	struct tm tmClock;
	memset(&tmClock,0,sizeof(struct tm));
	time_t tmClockTime = 0;
	do
	{
		while(true)
		{
			l = askUser(szClock,"Clock yyyy/mm/dd HH:MM:SS",19);
			szClock[l] = 0;

			if(l > 0 && !strptime(szClock,"%Y/%m/%d %H:%M:%S",&tmClock))
			{
				_tprintf(_T("\nInvalid clock time."));	
				continue;
			}
			else
			{
				if(l == 0)
				{
					tmClockTime = time(NULL);
				}
				else
				{
					tmClockTime = _mkgmtime(&tmClock);
				}
				localtime_s(&tmClock,&tmClockTime);
				char szVerboseClockTime[20];							
				strftime(szVerboseClockTime,20,"%Y/%m/%d %H:%M:%S",&tmClock);
				_tprintf(_T("\nClock is %s"),szVerboseClockTime);
				break;
			}
		}
	}
	while(!askYN("Correct"));
	
	WriteRockeyKey(&bkInfo,wRequiredBKVersion,tmExpiryTime,tmClockTime);
	
	_tprintf(_T("\nPress ENTER to finish.\n"));
	getchar();

	return nRetCode;
}

