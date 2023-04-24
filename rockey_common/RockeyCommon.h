#ifndef __ROCKEY_COMMON_H__
#define __ROCKEY_COMMON_H__

#define COMPANY_MAX_LEN 200
#define STREET_MAX_LEN 48
#define POBOX_MAX_LEN 10
#define CITY_MAX_LEN 48
#define ZIPCODE_MAX_LEN 10
#define NIP_MAX_LEN 10
#define FARMNO_MAX_LEN 14


#define ROCKEY_BASIC_PASSWD_1  0x0000
#define ROCKEY_BASIC_PASSWD_2  0x0000

#define ROCKEY_CLOCK_BASE_ADDRESS 0 //8 bytes for time_t, number of seconds from 1970/01/01 00:00


#define ROCKEY_REQVER_BASE_ADDRESS_EX 500 //2 bytes for required version
#define ROCKEY_EXPIRY_DT_BASE_ADDRESS_EX 502
#define ROCKEY_CLIENTINFO_BASE_ADDRESS_EX 510

#define KEY_EXPIRY_DT_TIMER_UNIT (0)

#define FEATURE_MASK 0x0007
#define FEATURE_OCR 3
#define FEATURE_COLLECTOR 4

typedef struct tagROCKEY_BKCLIENTINFO
{
	WORD	wClientId;
	WORD	wFeaturesEnabled;
	TCHAR	szCompany[COMPANY_MAX_LEN+1];
	TCHAR	szStreet[STREET_MAX_LEN+1];
	TCHAR	szPOBox[POBOX_MAX_LEN+1];
	TCHAR	szCity[CITY_MAX_LEN+1];
	TCHAR	szZipCode[ZIPCODE_MAX_LEN+1];
	TCHAR	szNIP[NIP_MAX_LEN+1];
	TCHAR	szFarmNo[FARMNO_MAX_LEN+1];	
} ROCKEY_BKCLIENTINFO,*PROCKEY_BKCLIENTINFO;


BOOL WriteRockeyKey(PROCKEY_BKCLIENTINFO pBkClientInfo,WORD wRequiredVersion,time_t tmExpiry,time_t tmClock);


#endif