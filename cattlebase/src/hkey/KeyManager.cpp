#include "stdafx.h"
#include "KeyManager.h"
#include "../classes/types/DateTime.h"
#include "../VersionNo.h"

//#include "../datatypes/CompanyInfo.h"
//#include "../httpdownloader/HttpFileGetter.h"
//#include "../httpdownloader/TransferBuffer.h"
//#include "../httpdownloader/URLBuilder.h"
#include "../datatypes/CompanyInfo.h"
#include "../missing/strptime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ONE_SECOND_MS (1000)

#define ONE_MINUTE_MS ((ONE_SECOND_MS)*60)

#define ONE_HOUR_MS ((ONE_MINUTE_MS)*60)

#define MAX_KEY_ABSENCE_SEC ((ONE_HOUR_MS)*12)

#define REFRESH_INTERVAL_MS_WHEN_KEYPLUGGEDIN (ONE_MINUTE_MS*1)
#define REFRESH_INTERVAL_MS_WHEN_KEYPLUGGEDOUT (ONE_SECOND_MS*10)

#define HARDCODED_COMPANY

IMPLEMENT_DYNCREATE(KeyManager, CWinTimerThread)

KeyManager::KeyManager() : m_Lock(&m_Working),m_TickTimerId(0),m_KeyState(NOT_FOUND),
							m_KeyAbsenceTimeSecs(0),m_KeyExpiryDt(INVALID_TIME),m_RequiredVersion(0),
							m_KeyLastSeenTime(INVALID_TIME),m_wFeatures(0),m_KeyClock(INVALID_TIME)
{
}

KeyManager::~KeyManager()
{
	
}

BEGIN_MESSAGE_MAP(KeyManager, CWinTimerThread)
    ON_THREAD_MESSAGE(WM_TIMER,    OnTimer)
END_MESSAGE_MAP()


void KeyManager::Shutdown()
{
	PostThreadMessage(WM_QUIT,0,0);
	//WaitForSingleObject(m_hThread,INFINITE);
}

KeyManager::KEY_STATE KeyManager::GetKeyState()
{
	KeyManager::KEY_STATE keyState;
	m_Lock.Lock();
	keyState = m_KeyState;
	m_Lock.Unlock();
	return keyState;
}

time_t KeyManager::GetAbsentLimitSecs() const
{
	return MAX_KEY_ABSENCE_SEC;
}

time_t KeyManager::GetAbsentTimeSecs() const
{
	time_t keyAbsenceTimeSecs;

	KeyManager* pKeyManager = const_cast<KeyManager*>(this);

	pKeyManager->Lock();
	keyAbsenceTimeSecs = m_KeyAbsenceTimeSecs;
	pKeyManager->Unlock();
	return keyAbsenceTimeSecs;
}

time_t KeyManager::GetKeyClock() const
{
	time_t keyClock;
	KeyManager* pKeyManager = const_cast<KeyManager*>(this);
	pKeyManager->Lock();
	keyClock = m_KeyClock;
	pKeyManager->Unlock();
	return keyClock;
}

bool KeyManager::IsFeatureEnabled(UINT uFeature) const
{
	bool bFeatureEnabled = false;
	ASSERT(uFeature < sizeof(m_wFeatures)*8); 
	if(uFeature >= sizeof(m_wFeatures)*8)
		return false;
	WORD feature_mask = 1 << uFeature;

	KeyManager* pKeyManager = const_cast<KeyManager*>(this);
	pKeyManager->Lock();
	bFeatureEnabled = (m_wFeatures & feature_mask) != 0;
	pKeyManager->Unlock();
	
	return bFeatureEnabled;
}

time_t KeyManager::GetKeyExpiryDt() const
{
	time_t keyExpiryDt;
	KeyManager* pKeyManager = const_cast<KeyManager*>(this);
	pKeyManager->Lock();
	keyExpiryDt = m_KeyExpiryDt;
	pKeyManager->Unlock();
	return keyExpiryDt;
}

UINT KeyManager::GetKeyRequiredVersion() const
{
	UINT uRequiredVersion;
	KeyManager* pKeyManager = const_cast<KeyManager*>(this);
	pKeyManager->Lock();
	uRequiredVersion = m_RequiredVersion;
	pKeyManager->Unlock();
	return uRequiredVersion;
}

void KeyManager::Lock()
{
	m_Lock.Lock();
}

void KeyManager::Unlock()
{
	m_Lock.Unlock();
}

BOOL KeyManager::InitInstance()
{ 	
	UINT uRefreshInterval = REFRESH_INTERVAL_MS_WHEN_KEYPLUGGEDIN;
	if(!TimerTick())
	{
		uRefreshInterval = REFRESH_INTERVAL_MS_WHEN_KEYPLUGGEDOUT;	
	}
	

	m_TickTimerId = AddTimer(uRefreshInterval);
    return TRUE;
}

void KeyManager::OnTimer(WPARAM idEvent, LPARAM dwTime)
{	
	RemoveTimer(m_TickTimerId);

	UINT uRefreshInterval = REFRESH_INTERVAL_MS_WHEN_KEYPLUGGEDIN;
	if(!TimerTick())
	{
		uRefreshInterval = REFRESH_INTERVAL_MS_WHEN_KEYPLUGGEDOUT;	
	}	

	m_TickTimerId = AddTimer(uRefreshInterval);

}

bool KeyManager::ReadKey(PKEY_CONTENT pKeyContent,RockeyWrapper &key)
{

	ROCKEY_BKCLIENTINFO clientInfo;
	key.Read((BYTE*)&clientInfo,sizeof(ROCKEY_BKCLIENTINFO),ROCKEY_CLIENTINFO_BASE_ADDRESS_EX);
	
	time_t expiryDt = INVALID_TIME;
	
	key.Read((BYTE*)&expiryDt,8,ROCKEY_EXPIRY_DT_BASE_ADDRESS_EX);		

	time_t clockTm = INVALID_TIME;
	key.Read((BYTE*)&clockTm,8,ROCKEY_CLOCK_BASE_ADDRESS);
	
	WORD wRequiredVersion = 0;
	key.Read((BYTE*)&wRequiredVersion,2,ROCKEY_REQVER_BASE_ADDRESS_EX);
	
	memcpy(&pKeyContent->clientInfo,&clientInfo,sizeof(ROCKEY_BKCLIENTINFO));
	pKeyContent->keyClock = clockTm;
	pKeyContent->keyExpiryDt = expiryDt;
	
	pKeyContent->wRequiredVersion = wRequiredVersion;		
	
	return true;
	
}

bool KeyManager::AdjustClock(time_t currentTime,RockeyWrapper &key)
{
	if(key.AdjustClock(currentTime))
	{
		key.Write((BYTE*)&currentTime,8,ROCKEY_CLOCK_BASE_ADDRESS);
		return true;
	}
	return false;
}

bool KeyManager::TimerTick()
{
	time_t currentTime = time(NULL);
	KEY_CONTENT keyContent;
	memset(&keyContent,0,sizeof(KEY_CONTENT));

	RockeyWrapper key;
	time_t newKeyAbsenceTimeSecs = m_KeyAbsenceTimeSecs;

	bool bKeyPluggedIn = false;
	bool bInvalidClock = false;

#ifndef HARDCODED_COMPANY

	if(key.Open(ROCKEY_BASIC_PASSWD_1,ROCKEY_BASIC_PASSWD_2))
	{
		ReadKey(&keyContent,key);
		if(!AdjustClock(currentTime,key))
		{
			bInvalidClock = true;
		}		
		key.Close();		
		m_KeyLastSeenTime = currentTime;
		bKeyPluggedIn = true;
		newKeyAbsenceTimeSecs = 0;
	}
	else
	{
		if(!bKeyPluggedIn)
		{
			if(currentTime >= m_KeyLastSeenTime+(REFRESH_INTERVAL_MS_WHEN_KEYPLUGGEDOUT/1000))
			{
				newKeyAbsenceTimeSecs  = currentTime - m_KeyLastSeenTime;
			}
			else
			{
				newKeyAbsenceTimeSecs += (REFRESH_INTERVAL_MS_WHEN_KEYPLUGGEDOUT/1000);
			}
		}
	}
#else
	bKeyPluggedIn = true;
	newKeyAbsenceTimeSecs = 0;
	m_KeyLastSeenTime = currentTime;

	struct tm expiryTm;
	memset(&expiryTm,0,sizeof(struct tm));
	strptime("2099/12/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);

	/*keyContent.clientInfo.wClientId = 1;
	strncpy(keyContent.clientInfo.szCompany,"\"Eko-Natura\" Sp. z o.o.",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Moszczenica Ni¿na",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"9",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Stary S¹cz",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-340",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7342823079",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL030919740003",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 1;
	strncpy(keyContent.clientInfo.szCompany,"SKUP I SPRZEDA¯ ¯YWCA Jan Barnach",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"¯ELENIKOWA WIELKA",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"151",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"NAWOJOWA",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-335",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7342559300",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL031303851002",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 1;
	strncpy(keyContent.clientInfo.szCompany,"SKUP I SPRZEDA¯ ZWIERZ¥T RZENYCH I HODOWLANYCH MARCIN RYNDAK",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"OLSZYNY",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"445",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"OLSZYNY",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-164",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"9930327987",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL038416580003",FARMNO_MAX_LEN);
	strptime("2023/06/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/


	/*keyContent.clientInfo.wClientId = 1;
	strncpy(keyContent.clientInfo.szCompany,"SKUP I SPRZEDA¯ ZWIERZ¥T RZENYCH I HODOWLANYCH JERZY RYNDAK",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"OLSZYNY",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"420",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"OLSZYNY",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-164",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"6850007575",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL038393181003",FARMNO_MAX_LEN);
	strptime("2023/06/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"F.H.U. \"DOBEK\" SPÓ£KA JAWNA SEBASTIAN DOBEK ANDRZEJ DOBEK",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"ROMANOWICZA",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"45",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"TARNÓW",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-100",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"8733092918",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL059719580002",FARMNO_MAX_LEN);
	strptime("2099/12/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"UBOJNIA ZWIERZ¥T RZENYCH Marek Grzyb",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"TRZYCIERZ",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"49",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"KORZENNA",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-322",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7340015814",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL037961081003",FARMNO_MAX_LEN);
	strptime("2024/01/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"\"Eko-Natura\" Sp. z o.o.",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Moszczenica Ni¿na",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"9",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Stary S¹cz",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-340",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7342823079",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL030919740003",FARMNO_MAX_LEN);
	strptime("2030/01/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Novexim Nowacki Piotr Pawe³, Nowacki Micha³ s.c.",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Gombrowicza",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"11",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Ciechanów",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"06-400",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"5661937114",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL061723903001",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Novexim Nowacki Piotr Pawe³, Nowacki Micha³ s.c.",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Strzelnia",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"12",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Grudusk",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"06-460",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"5661937114",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL061723903002",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"F.H.U PAWMARPOL DOBEK PAWE£",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"WOLA GRÊBOSZOWSKA",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"91",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"GRÊBOSZÓW",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-260",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"9930231201",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL056714570004",FARMNO_MAX_LEN);
	strptime("2099/12/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"F.H.U. PAWMARPOL DOBEK PAWE£",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"GOS£AWICE",STREET_MAX_LEN);	
	strncpy(keyContent.clientInfo.szPOBox,"117",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"WIERZCHOS£AWICE",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-122",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"9930231201",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL056714570004",FARMNO_MAX_LEN);
	strptime("2099/01/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"PHU \"WISPOL\" Emil Wiœniewski",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Porêba Œrednia",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"70",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Porêba",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"07-308",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7621899727",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL063686182001",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Skup zwierz¹t \"REMUS\" Remigiusz Gêbicki",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Golesze Ma³e, G³ówna",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"65A",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Wolbórz",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"97-320",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7712490174",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL059534731004",FARMNO_MAX_LEN);
	strptime("2021/01/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"\"ARTOPL\" Arkadiusz Gêbicki",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Taraska",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"11",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Aleksandrów",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"26-337",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7712327592",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL054832560005",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"F.H.U Krzysztof Brzegowy",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Kopaliny",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"118",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Nowy Wiœnicz",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"32-720",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"8681740729",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL064352063004",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Firma Handlowa BRZEGOWY sp. z o.o.",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Prusa",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"3",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Szczucin",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-230",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"8711777862",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL078282672001",FARMNO_MAX_LEN);
	strptime("2024/01/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"SKUP I SPRZEDA¯ ¯YWCA UBÓJ I SPRZEDA¯ POUBOJOWA JAN BASTA",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"GRÓDEK n/DUNAJCEM",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"42",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"GRÓDEK n/DUNAJCEM",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-318",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7341012901",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL038362636002",FARMNO_MAX_LEN);
	strptime("2024/01/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"P.H.U TRANS¯YW Wojciech Maciejewski",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Grobla",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"42",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"S³upca",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"62-400",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"6670001482",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL031749854001",FARMNO_MAX_LEN);*/

    /*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"SKUP I SPRZEDA¯ BYD£A RZENEGO Sroka Dariusz",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Moszczenica Ni¿na",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"151",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Stary S¹cz",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-340",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7342713758",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL038380781002",FARMNO_MAX_LEN);
	strptime("2021/01/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	
	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Sprzeda¿ hurtowa ¿ywych zwierz¹t T.T.D.P.S",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"B. Prusa",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"3",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Szczucin",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-230",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"8711268445",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL040808064002",FARMNO_MAX_LEN);
	strptime("2022/01/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/


	/*strcpy(bkInfo.szCompany,"");
	strcpy(bkInfo.szStreet,"");	
	strcpy(bkInfo.szPOBox,"42");
	strcpy(bkInfo.szCity,"GRÓDEK");
	strcpy(bkInfo.szZipCode,"33-318");
	strcpy(bkInfo.szNIP,"7341012901");
	strcpy(bkInfo.szFarmNo,"PL038362636001");*/


	/*keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strcpy(keyContent.clientInfo.szCompany,"SKUP-SPRZEDA¯ ZWIERZ¥T RZE¯NYCH I HODOWLANYCH KRZYSZTOF ŒLÊZAK");
	strcpy(keyContent.clientInfo.szStreet,"KRAKOWSKA");	
	strcpy(keyContent.clientInfo.szPOBox,"68");
	strcpy(keyContent.clientInfo.szCity,"BARANÓW SANDOMIERSKI");
	strcpy(keyContent.clientInfo.szZipCode,"39-450");
	strcpy(keyContent.clientInfo.szNIP,"8671939642");
	strcpy(keyContent.clientInfo.szFarmNo,"PL038126891001");
	strptime("2020/01/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strcpy(keyContent.clientInfo.szCompany,"Obrót zwierzêtami, handel miêsem. Grzegorz Bida, Krzysztof Sulowski s.c.");
	strcpy(keyContent.clientInfo.szStreet,"Zagrody");	
	strcpy(keyContent.clientInfo.szPOBox,"135");
	strcpy(keyContent.clientInfo.szCity,"Goraj");
	strcpy(keyContent.clientInfo.szZipCode,"23-450");
	strcpy(keyContent.clientInfo.szNIP,"9182161907");
	strcpy(keyContent.clientInfo.szFarmNo,"PL068729533001");
	strptime("2023/01/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strcpy(keyContent.clientInfo.szCompany,"F.H.U. \"Mar-Pol\" HANDEL ZWIERZÊTAMI MARIUSZ KIJOWSKI");
	strcpy(keyContent.clientInfo.szStreet,"Nozdrzec");	
	strcpy(keyContent.clientInfo.szPOBox,"17");
	strcpy(keyContent.clientInfo.szCity,"Nozdrzec");
	strcpy(keyContent.clientInfo.szZipCode,"36-245");
	strcpy(keyContent.clientInfo.szNIP,"6861588185");
	strcpy(keyContent.clientInfo.szFarmNo,"PL059619175003");
	strptime("2019/01/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*strcpy(keyContent.clientInfo.szCompany,"CHOROMAÑSKI PAWE£");
	strcpy(keyContent.clientInfo.szStreet,"KWIATOWA");	
	strcpy(keyContent.clientInfo.szPOBox,"10");
	strcpy(keyContent.clientInfo.szCity,"KLECZKOWO");
	strcpy(keyContent.clientInfo.szZipCode,"07-405");
	strcpy(keyContent.clientInfo.szNIP,"7582048941");
	strcpy(keyContent.clientInfo.szFarmNo,"PL061096463002");*/

	/*strcpy(keyContent.clientInfo.szCompany,"Firma Handlowa \"BA£DYGA\" Arkadiusz Ba³dyga");
	strcpy(keyContent.clientInfo.szStreet,"Miastowice");	
	strcpy(keyContent.clientInfo.szPOBox,"34A");
	strcpy(keyContent.clientInfo.szCity,"Kcynia");
	strcpy(keyContent.clientInfo.szZipCode,"89-240");
	strcpy(keyContent.clientInfo.szNIP,"5581718574");
	strcpy(keyContent.clientInfo.szFarmNo,"PL063706123002");*/

	/*strcpy(keyContent.clientInfo.szCompany,"H.A.R.S.iP. Pawe³ Kaczmarek");
	strcpy(keyContent.clientInfo.szStreet,"Rzeczna");	
	strcpy(keyContent.clientInfo.szPOBox,"3");
	strcpy(keyContent.clientInfo.szCity,"Czarnocin");
	strcpy(keyContent.clientInfo.szZipCode,"97-318");
	strcpy(keyContent.clientInfo.szNIP,"7710005370");
	strcpy(keyContent.clientInfo.szFarmNo,"PL038423492003");*/


	/*keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strcpy(keyContent.clientInfo.szCompany,"OBRÓT ZWIERZÊTAMI Marek Go³as");
	strcpy(keyContent.clientInfo.szStreet,"G³owackiego");	
	strcpy(keyContent.clientInfo.szPOBox,"66");
	strcpy(keyContent.clientInfo.szCity,"Kazimierza Wielka");
	strcpy(keyContent.clientInfo.szZipCode,"28-500");
	strcpy(keyContent.clientInfo.szNIP,"8732941568");
	strcpy(keyContent.clientInfo.szFarmNo,"PL047829580005");
	strptime("2017/03/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strcpy(keyContent.clientInfo.szCompany,"KRÓL SPÓ£KA JAWNA");
	strcpy(keyContent.clientInfo.szStreet,"Pi³sudskiego");	
	strcpy(keyContent.clientInfo.szPOBox,"61");
	strcpy(keyContent.clientInfo.szCity,"Janów Lubelski");
	strcpy(keyContent.clientInfo.szZipCode,"23-300");
	strcpy(keyContent.clientInfo.szNIP,"8621259738");
	strcpy(keyContent.clientInfo.szFarmNo,"PL037918392002");*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"FIRMA HANDLOWA Krzysztof Cygankiewicz",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Kijanki",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"21",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Kraków",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"30-693",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"6792943713",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL061686842003",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"SKUP, SPRZEDA¯ BYD£A HODOWLANEGO Beata Serafin",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Stryszowa",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"67",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Gdów",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"32-420",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"6831195564",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL059347076002",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Zootex-PP S.C. Ptasiñski Przemys³aw, Ptasiñski Zbigniew",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Gliniak",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"13",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Miñsk Mazowiecki",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"05-300",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"8222246349",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL061969014003",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"FIRMA HANDLOWA RAFA£ MAZUR",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"POZNACHOWICE GÓRNE",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"141",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"RACIECHOWICE",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"32-415",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"6811794250",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL038017430001",FARMNO_MAX_LEN);
	strptime("2024/01/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"FIRMA HANDLOWA JANECKI ZYGMUNT",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"DRU¯BACKIEJ",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"97",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"KRAKÓW",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"30-699",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"6792132400",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL045145456005",FARMNO_MAX_LEN);
	strptime("2024/01/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/


	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Zak³ady Miêsne \"Wyszyñscy\"",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Kostry Œmiejki",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"3",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Klukowo",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"18-214",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7221313511",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL056969451002",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"AGROMIR S³awomir Krzy¿anowski",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Ruda",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"29A",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Stare Za³ubice",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"05-255",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"1251177133",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL068782654001",FARMNO_MAX_LEN);*/

	/*strcpy(keyContent.clientInfo.szCompany,"Spó³ka Jawna Mikulec");
	strcpy(keyContent.clientInfo.szStreet,"Moszcznica Ni¿na");	
	strcpy(keyContent.clientInfo.szPOBox,"144");
	strcpy(keyContent.clientInfo.szCity,"Stary S¹cz");
	strcpy(keyContent.clientInfo.szZipCode,"33-340");
	strcpy(keyContent.clientInfo.szNIP,"7342599908");
	strcpy(keyContent.clientInfo.szFarmNo,"PL037982735003");
	strptime("2020/01/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*strcpy(keyContent.clientInfo.szCompany,"SKUP SPRZEDA¯ zwierz¹t rzeŸnych i hodowlanych sprzeda¿ miêsa \"KOLTO\" Karol Serafin");
	strcpy(keyContent.clientInfo.szStreet,"Stryszowa");	
	strcpy(keyContent.clientInfo.szPOBox,"67");
	strcpy(keyContent.clientInfo.szCity,"Gdów");
	strcpy(keyContent.clientInfo.szZipCode,"32-420");
	strcpy(keyContent.clientInfo.szNIP,"6831936049");
	strcpy(keyContent.clientInfo.szFarmNo,"PL061018704002");*/


	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"LESZEK GROMADZKI i Wspólnik Spó³ka Jawna",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"P³onka Koœcielna",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"45",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"£apy",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"18-100",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"9661654424",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL038203912010",FARMNO_MAX_LEN);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"'TIM KOT' FIRMA HANDLOWO US£UGOWA TADEUSZ KOT i MICHA£ KOT - SPÓ£KA CYWILNA",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Nieczajna Górna",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"298c",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Nieczajna Górna",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-205",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"8711772528",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL071873410002",FARMNO_MAX_LEN);
	strptime("2024/01/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"ARTRADE ARTUR DOBEK",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"RZUCHOWA",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"169",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"RZUCHOWA",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-114",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"8732930607",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL067398755001",FARMNO_MAX_LEN);*/


	/*keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"GRZES£AW Sp. z o.o.",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Stare Rakowo",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"25",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Ma³y P³ock",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"18-516",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"2910226222",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL071648020001",FARMNO_MAX_LEN);*/


	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"AK-ROL Krzysztof Szafran",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Du¿a Cerkwica",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"62",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Kamieñ Krajeñski",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"89-430",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"5040073640",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL070769835002",FARMNO_MAX_LEN);
	strptime("2023/03/15 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 1;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Gospodarstwo Rolne Krzysztof Szafran",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Du¿a Cerkwica",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"62",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Kamieñ Krajeñski",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"89-430",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"5040073640",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL070769835001",FARMNO_MAX_LEN);
	strptime("2024/03/15 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/


/*
	keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"ZAK£AD MASARSKI ZBÓJNIK 
	
	Mateusz",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Moszczenica Ni¿na",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"255",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Stary S¹cz",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-340",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7343414087",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL064058104002",FARMNO_MAX_LEN);
	strptime("2020/02/28 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

/*
	keyContent.clientInfo.wClientId = 2;
	keyContent.clientInfo.wFeaturesEnabled = 0xFFFF;
	strncpy(keyContent.clientInfo.szCompany,"Simenpol Sroka Tomasz",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Olszynka",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"1",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Stary S¹cz",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-340",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7343517338",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL070893741002",FARMNO_MAX_LEN);
	strptime("2020/02/28 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);
*/

	/*keyContent.clientInfo.wClientId = 1;
	strncpy(keyContent.clientInfo.szCompany,"KERTO Sp. z o.o.",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"Sosnowa",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"10",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"Zambrów D³ugoborz",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"18-300",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"7231630331",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL070804373003",FARMNO_MAX_LEN);
	strptime("2022/09/30 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	/*keyContent.clientInfo.wClientId = 1;
	strncpy(keyContent.clientInfo.szCompany,"FIRMA \"GAWRYŒ\" PAULINA GAWRYŒ & GRZEGORZ GAWRYŒ SPÓ£KA CYWILNA",COMPANY_MAX_LEN); 
	strncpy(keyContent.clientInfo.szStreet,"GORZYCE",STREET_MAX_LEN);
	strncpy(keyContent.clientInfo.szPOBox,"147",POBOX_MAX_LEN);
	strncpy(keyContent.clientInfo.szCity,"OTFINÓW",CITY_MAX_LEN);
	strncpy(keyContent.clientInfo.szZipCode,"33-250",ZIPCODE_MAX_LEN);
	strncpy(keyContent.clientInfo.szNIP,"9930593223",NIP_MAX_LEN);
	strncpy(keyContent.clientInfo.szFarmNo,"PL063693894002",FARMNO_MAX_LEN);
	strptime("2024/01/31 23:59:59","%Y/%m/%d %H:%M:%S",&expiryTm);*/

	keyContent.keyExpiryDt = _mkgmtime(&expiryTm);
	keyContent.keyClock = currentTime;
	keyContent.wRequiredVersion = 1000;


#endif

	Lock();
	if(bKeyPluggedIn)		
	{
		SettleCompany(&keyContent.clientInfo);	
		m_KeyExpiryDt = keyContent.keyExpiryDt;
		m_RequiredVersion = keyContent.wRequiredVersion;
		m_wFeatures = keyContent.clientInfo.wFeaturesEnabled;
		m_KeyClock = keyContent.keyClock;
		
	}

	m_KeyAbsenceTimeSecs = newKeyAbsenceTimeSecs;

	if(bKeyPluggedIn || m_KeyState != NOT_FOUND)
	{
		if(bInvalidClock)
		{
			m_KeyState = INVALID_CLOCK;
		} 
		else if(m_RequiredVersion != 0 && m_RequiredVersion > VERSION_EXTEND)
		{
			m_KeyState = REQUIRES_UPDATE;
		}
		else if(m_KeyExpiryDt < currentTime)
		{
			m_KeyState = EXPIRED;
		}
		else if(m_KeyAbsenceTimeSecs >= GetAbsentLimitSecs())
		{
			m_KeyState = ABSENT_TOO_LONG;
		}
		else
		{
			m_KeyState = VALID;
		}
	}
	Unlock();
	return bKeyPluggedIn;	
}

void KeyManager::SettleCompany(const PROCKEY_BKCLIENTINFO companyInfo)
{
	const Company *pCompany = Company::GetCompany();
	CString sCompany = pCompany->GetCompanyName();
	CString sStreet = pCompany->GetStreet();
	CString sPOBox = pCompany->GetPOBox();
	CString sCity = pCompany->GetCity();
	CString sZipCode = pCompany->GetZipCode();
	CString sNIP = pCompany->GetNIP();
	CString sFarmNo = pCompany->GetFarmNo();


	if(sCompany != companyInfo->szCompany  ||
		sStreet != companyInfo->szStreet ||
		sPOBox != companyInfo->szPOBox ||
		sCity != companyInfo->szCity || 
		sZipCode != companyInfo->szZipCode ||
		sNIP != companyInfo->szNIP ||
		sFarmNo != companyInfo->szFarmNo)
	{
		Company::Lock();
		CompanyForInitialisation* pCompanyInitialisation = Company::GetCompanyForInitialisation();		
		pCompanyInitialisation->SetCompanyName(companyInfo->szCompany);
		pCompanyInitialisation->SetCity(companyInfo->szCity);
		pCompanyInitialisation->SetZipCode(companyInfo->szZipCode);
		pCompanyInitialisation->SetStreet(companyInfo->szStreet);
		pCompanyInitialisation->SetPOBox(companyInfo->szPOBox);
		pCompanyInitialisation->SetFarmNo(companyInfo->szFarmNo);	
		pCompanyInitialisation->SetNIP(companyInfo->szNIP);
		Company::Unlock();
	}

}

int KeyManager::ExitInstance()
{
	RemoveTimer(m_TickTimerId);	
	return CWinTimerThread::ExitInstance();
}