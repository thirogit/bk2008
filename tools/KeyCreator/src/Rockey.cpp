#include "stdafx.h"
#include <Ry4S.h>
#include "common/console.h"
#include "Rockey.h"
#include <time.h>

#define _ERRORCODES_RECORD(code,desc) {code,_T(#code),_T(desc)}

struct ROCKEY_ERRORCODES
{
	WORD wErrorCode;
	TCHAR* szError;
	TCHAR* szErrorDesc;
} _rockeyErrorCodes[] =
{
_ERRORCODES_RECORD(ERR_NO_ROCKEY,				"No driver installed"),
_ERRORCODES_RECORD(ERR_INVALID_PASSWORD,		"Found Rockey, but basic passwords (pass1, pass2) are wrong"),
_ERRORCODES_RECORD(ERR_INVALID_PASSWORD_OR_ID,	"Wrong password or Hardware ID"),
_ERRORCODES_RECORD(ERR_SETID,					"Error in Hardware ID setting"),
_ERRORCODES_RECORD(ERR_INVALID_ADDR_OR_SIZE,	"Error in read/write address or length"),
_ERRORCODES_RECORD(ERR_UNKNOWN_COMMAND,			"No such command"),
_ERRORCODES_RECORD(ERR_NOTBELEVEL3,				"Internal Error"),
_ERRORCODES_RECORD(ERR_READ,					"Read Error"),
_ERRORCODES_RECORD(ERR_WRITE,					"Write Error"),
_ERRORCODES_RECORD(ERR_RANDOM,					"Random Number Error"),
_ERRORCODES_RECORD(ERR_SEED,					"Seed Code Error"),
_ERRORCODES_RECORD(ERR_CALCULATE,				"Error in Calculation"),
_ERRORCODES_RECORD(ERR_NO_OPEN,					"Unopen dongle before operation"),
_ERRORCODES_RECORD(ERR_OPEN_OVERFLOW,			"Too many open dongles (>16)"),
_ERRORCODES_RECORD(ERR_NOMORE,					"No more dongles"),
_ERRORCODES_RECORD(ERR_NEED_FIND,				"No 'Find' function called before using 'FindNext'"),
_ERRORCODES_RECORD(ERR_DECREASE,				"Decrease Error"),
_ERRORCODES_RECORD(ERR_AR_BADCOMMAND,			"Error in Arithmetic Instruction"),
_ERRORCODES_RECORD(ERR_AR_UNKNOWN_OPCODE,		"Error in Arithmetic Operator"),
_ERRORCODES_RECORD(ERR_AR_WRONGBEGIN,			"Constant cannot be used in the first arithmetic instruction"),
_ERRORCODES_RECORD(ERR_AR_WRONG_END,			"Constant cannot be used in the last arithmetic instruction"),
_ERRORCODES_RECORD(ERR_AR_VALUEOVERFLOW,		"The value of Constant cannot be greater than 63"),
_ERRORCODES_RECORD(ERR_TOOMUCHTHREAD,			"The threads opened in the same process cannot be greater than 100"),
_ERRORCODES_RECORD(ERR_INVALID_RY4S,			"Try to handle non-ROCKEY4 SMART dongle"),
_ERRORCODES_RECORD(ERR_INVALID_PARAMETER,		"Invalid parameter"),
_ERRORCODES_RECORD(ERR_INVALID_TIMEVALUE,		"Invalid Date-Time value"),
_ERRORCODES_RECORD(ERR_SET_DES_KEY,				"Set DES Key Error"),
_ERRORCODES_RECORD(ERR_DES_ENCRYPT,				"DES Encryption Error"),
_ERRORCODES_RECORD(ERR_DES_DECRYPT,				"DES Decryption Error"), 
_ERRORCODES_RECORD(ERR_SET_RSAKEY_N,			"Error in writing N into RSA keypair"),
_ERRORCODES_RECORD(ERR_SET_RSAKEY_D,			"Error in writing D into RSA keypair"),
_ERRORCODES_RECORD(ERR_RSA_ENCRYPT,				"RSA Encryption Error"), 
_ERRORCODES_RECORD(ERR_RSA_DECRYPT,				"RSA Decryption Error"),
_ERRORCODES_RECORD(ERR_INVALID_LENGTH,			""),
_ERRORCODES_RECORD(ERR_RECEIVE_NULL,			"Receive nothing"),
_ERRORCODES_RECORD(ERR_INVALID_BUFFER,			"NULL Pointer"),
_ERRORCODES_RECORD(ERR_UNKNOWN_SYSTEM,			"Unknown Operating System"),
_ERRORCODES_RECORD(ERROR_UNINIT_TIME_UNIT,		"Time uint uninitialized"),
_ERRORCODES_RECORD(ERR_UNKNOWN,					"Unknown Error"),
{0,NULL,NULL}
};



#define ROCKEY_ADV_PASSWD_3  0x53C0
#define ROCKEY_ADV_PASSWD_4  0xC313


static const TCHAR* UPDATE_RSA_PUBLIC_KEY = 
	"\x46\x53\xF4\xEA\xA1\xFC\xD3\x1B\x03\xE8\x9E\xDA\x1D\x83\xC0\xFA"
	"\xE3\x69\x47\x16\x22\xB7\x19\x58\x5F\xEB\x4D\x1E\x64\xDD\xF5\xA5"
	"\x62\xE9\xF8\x84\x75\x69\xF5\x18\x16\x7B\xC0\x1C\x31\xC5\x66\x29"
	"\xEF\xA9\xDD\xBE\x74\xA6\xBF\x1F\x0E\x4A\x35\x9B\xA5\x76\xF4\x22"
	"\x6D\xEC\x05\x1C\x0F\x45\x0C\xF2\xE5\x5C\x6E\xD6\x03\xD4\x0C\x96"
	"\x7D\xAE\x48\x11\x59\xAA\xBC\x2D\x07\x5D\xC2\x51\x36\x3B\x2D\x45"
	"\x13\xFA\x5D\x02\x9E\x1C\x95\xC9\xB1\xEA\xAB\x23\xE9\xCC\x80\x7A"
	"\x82\x37\xE5\x8E\x6E\x0F\xC8\xF5\x93\x67\xD4\xC9\x16\xC7\xE5\x59";


static const TCHAR* UPDATE_RSA_PRIVATE_KEY =
	"\x0E\x2C\x88\x95\xFE\xE8\xE4\xFA\x4F\xA8\xFE\x5C\xC9\xBF\x79\xCB"
	"\x1A\xCF\x2F\x24\x9C\x38\x86\x6E\x4D\x73\x60\xD7\x74\x0E\x3E\x11"
	"\x8B\xA1\xF4\x97\x07\x65\xEE\x23\x07\x52\x6D\x47\x0F\xDE\x43\x8A"
	"\x54\x5D\xEB\x6D\x2F\x78\x13\x32\x54\x4E\xD1\xE2\xDA\x47\x4B\xDB"
	"\xA8\x7D\x69\x58\x59\xF6\xDA\xCF\x38\x79\x20\xA1\x52\xA5\xB9\x50"
	"\xCA\x4A\xD3\x58\x6C\x7F\xF7\x62\xAA\x79\x28\xCD\x7D\xB8\xE1\x68"
	"\x5E\x6E\xF4\xC5\xC5\xCF\xB0\x88\x6D\x9D\x40\x11\x41\x24\x50\x68"
	"\xB7\x01\xC8\x62\x06\xD9\xB0\xB0\x5C\xEA\x1D\x86\xAE\x8C\x25\xEB";

void PrintError(WORD errorCode)
{
	ROCKEY_ERRORCODES* pErrorCodes = _rockeyErrorCodes;
	while(pErrorCodes->wErrorCode)
	{
		if(pErrorCodes->wErrorCode == errorCode)
		{
			_tprintf(_T("Error %s: %s\n"), pErrorCodes->szError,pErrorCodes->szErrorDesc);
			break;	
		}
		pErrorCodes++;
	}
}

void ConvertTime(SYSTEMTIME* pSystemTime, const struct tm* t)
{
	pSystemTime->wSecond = t->tm_sec;
	pSystemTime->wMinute = t->tm_min;
	pSystemTime->wHour = t->tm_hour;
	pSystemTime->wDay = t->tm_mday;
	pSystemTime->wMonth = t->tm_mon+1;
	pSystemTime->wYear = t->tm_year+1900;
}


BOOL WriteKey(PROCKEY_BKCLIENTINFO pBkClientInfo,WORD wRequiredVersion,time_t tmExpiry,time_t tmClock)
{
	WORD keyHandle, p1, p2, p3, p4, retcode;
	DWORD lp1, lp2;
	BYTE bBuffer[1024];

	

 	p1 = ROCKEY_BASIC_PASSWD_1;
 	p2 = ROCKEY_BASIC_PASSWD_2;
 	p3 = ROCKEY_ADV_PASSWD_3;
	p4 = ROCKEY_ADV_PASSWD_4;


	//Find Rockey4Smart with specified password
	retcode = Rockey(RY_FIND, &keyHandle, &lp1, &lp2, &p1, &p2, &p3, &p4, NULL);
	if (retcode != ERR_SUCCESS)
	{
		PrintError(retcode);
		return FALSE;
	}
	_tprintf(_T("Found Rockey4Smart: %08X\n"), lp1);
	
	//Open Rockey4Smart
	retcode = Rockey(RY_OPEN, &keyHandle, &lp1, &lp2, &p1, &p2, &p3, &p4, NULL);
	if (retcode != ERR_SUCCESS)
	{
		PrintError(retcode);
		return FALSE;
	}

	//Get version
	retcode = Rockey(RY_VERSION,&keyHandle,&lp1,&lp2,&p1,&p2,&p3,&p4,NULL);
	if (retcode != ERR_SUCCESS)
	{
		PrintError(retcode);
		Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return FALSE;
	}

	if (lp2 < 0x0103)
	{
		_tprintf(_T("Old dongle doesn't support new memory read/write interface,please use v1.03 dongle for test!\n"));
		Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return FALSE;
	}

	
	memset(bBuffer,0,sizeof(bBuffer));
	SYSTEMTIME *pRyTimeClock = (SYSTEMTIME*)bBuffer;
	struct tm clock;
	localtime_s(&clock,&tmClock);
	ConvertTime(pRyTimeClock,&clock);
		 	
	p1 = 0;
 	p2 = 0;
 	p3 = 0;
	p4 = 0;
	lp1 = 0;
	lp2 = 0;

	retcode = Rockey(RY_ADJUST_TIMER_EX,&keyHandle,&lp1,&lp2,&p1,&p2,&p3,&p4,(BYTE*)bBuffer);
	if (retcode != ERR_SUCCESS)
	{
		printf("Failed to adjust clock,error code = %d\n",retcode);
		Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return FALSE;
	}	
	
	
	p1 = ROCKEY_CLOCK_BASE_ADDRESS;
	p2 = 8;

	retcode = Rockey(RY_WRITE, &keyHandle, &lp1, &lp2, &p1, &p2, &p3, &p4, (BYTE*)&tmClock);
	if (retcode != ERR_SUCCESS)
	{
		PrintError(retcode);
		Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return FALSE;
	}
	
	p1 = ROCKEY_REQVER_BASE_ADDRESS_EX;
	p2 = 2;	
	retcode = Rockey(RY_WRITE_EX, &keyHandle, &lp1, &lp2, &p1, &p2, &p3, &p4, (BYTE*)&wRequiredVersion);
	if (retcode != ERR_SUCCESS)
	{
		PrintError(retcode);
		Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return FALSE;
	}

	p1 = ROCKEY_CLIENTINFO_BASE_ADDRESS_EX;
	p2 = sizeof(ROCKEY_BKCLIENTINFO);
	retcode = Rockey(RY_WRITE_EX, &keyHandle, &lp1, &lp2, &p1, &p2, &p3, &p4, (BYTE*)pBkClientInfo);
	if (retcode != ERR_SUCCESS)
	{
		PrintError(retcode);
		Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return FALSE;
	}
		
	p1 = ROCKEY_EXPIRY_DT_BASE_ADDRESS_EX;	
	p2 = 8;
	p3 = 0;
	p4 = 0;
	
	retcode = Rockey(RY_WRITE_EX, &keyHandle, &lp1, &lp2, &p1, &p2, &p3, &p4, (BYTE*)&tmExpiry);
	if (retcode != ERR_SUCCESS)
	{
		PrintError(retcode);
		Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return FALSE;
	}


	memcpy(bBuffer,UPDATE_RSA_PUBLIC_KEY,128);
	memcpy(bBuffer+ 128,UPDATE_RSA_PRIVATE_KEY,128);
	retcode = Rockey(RY_SET_UPDATE_KEY, &keyHandle, &lp1, &lp2, &p1, &p2, &p3, &p4, bBuffer);
	if (retcode != ERR_SUCCESS)
	{
		PrintError(retcode);
		Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		return FALSE;
	}
		
	Rockey(RY_CLOSE,&keyHandle,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

	return TRUE;
}


BOOL WriteRockeyKey(PROCKEY_BKCLIENTINFO pBkClientInfo,WORD wRequiredVersion,time_t tmExpiry,time_t tmClock)
{
	TCHAR yes_no;

	_tprintf(_T("\nWrite key? (Y/N): "));
	readFromConsole(&yes_no,1); 
	_tprintf(_T("\n"));

	if(yes_no == 'y' || yes_no == 'Y')
	{
		while(1)
		{
			if(WriteKey(pBkClientInfo,wRequiredVersion,tmExpiry,tmClock))
				_tprintf(_T("\nWrite successful."));
			_tprintf(_T("\nRepeat? (Y/N):"));
			readFromConsole(&yes_no,1);
			_tprintf(_T("\n"));
			if(yes_no == 'N' || yes_no == 'n')
				break;
		}
	}
	return TRUE;

}