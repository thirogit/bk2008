#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "Nullable.h"

#define MAXDATEBUFF 100
#define SECONDS_IN_DAY (24*60*60)
#define SECONDS_IN_YEAR (365 * SECONDS_IN_DAY)     

/*
 * Macro to determine if a given year, expressed as the number of years since
 * 1900, is a leap year.
 */
#define _IS_LEAP_YEAR(y)        (((y % 4 == 0) && (y % 100 != 0)) || ((y) % 400 == 0))

#define _BASE_YEAR         1970                   /* 1970 is the base year */
#define _MAX_YEAR          2038                  /* 2038 is the max year */

#define _LEAP_YEAR_ADJUST  17                   /* Leap years 1900 - 1970 */

/*
 * Number of leap years from 1970 up to, but not including, the specified year
 * (expressed as the number of years since 1900).
 */
#define _ELAPSED_LEAP_YEARS(y)  (((y - 1)/4) - ((y - 1)/100) + ((y + 299)/400) \
                                - _LEAP_YEAR_ADJUST)

//#define _BASE_DOW          4                    /* 01-01-70 was a Thursday */

class CDateTime : public Nullable
{
public:

	enum DateFormat
	{
		YYYYdotMMdotDD,
		YYYYMMDD,
		DDMMYYYY,
		YYYYdashMMdashDD, //YYYY-MM-DD
		YYYYdotMMdotDD_HHcolonMMcolonSS, //30.12.2007 11:17:01
		YYYYMMDDHHMISS,
		DDdashMMdashYYYY,
		DDSMMSYYYY, //DD/MM/YYYY
		YYYYdashMMdashDD_HHcolonMI //YYYY-MM-DD HH:MI
	};

	typedef struct _tagDATETIMEFMTS
	{
		CDateTime::DateFormat dtFmt;
		LPCSTR strFmt;
	} DATETIMEFMTS;


	CDateTime();
	CDateTime(time_t secsSinceEpoch);	
	CDateTime(UINT year,UINT month,UINT day);
	CDateTime(UINT year,UINT month,UINT day,UINT hour,UINT min,UINT sec);
	CDateTime(const CDateTime& copy);
	bool IsNull() const;
	LPCSTR GetDateInFmt(DateFormat dtFmt) const;
	LPCSTR GetDayDate() const;
	LPCSTR GetFullDate() const;
	bool ParseDate(DateFormat fmt,const char* inputDt,bool bThrow = false);
	int GetDiffrenceInSeconds(const CDateTime& dt) const;
	int GetDiffrenceInDays(const CDateTime& dt) const;


	UINT GetYear() const;
	UINT GetMonth() const;
	UINT GetDay() const;
	UINT GetHour() const;
	UINT GetMinutes() const;
	UINT GetSeconds() const;

	bool SetYearMonthDay(UINT year,UINT month,UINT day,bool bThrow = false);
	bool SetHourMinSec(UINT hour,UINT minutes,UINT seconds,bool bThrow = false);

	bool EqualDay(const CDateTime& dt) const;
	UINT SecondsSinceEpoh() const;
	void SetDateTime(time_t secsSinceEpoch);
	
	void MakeNull();

	static const CDateTime now();

	bool SetTmStruct(struct tm &tm1) const;

	static DATETIMEFMTS* GetDateTimeFmts();
protected:

	static DATETIMEFMTS _datetimefmts[];

	
	UINT m_year;
	UINT m_month1_12;
	UINT m_day1_31;
	
	UINT hour0_23;
	UINT minute0_59;
	UINT sec0_59;
	
	char m_dateBuff[MAXDATEBUFF];

	
};

	bool operator>=(const CDateTime& left, const CDateTime& right);
	bool operator<=(const CDateTime& left, const CDateTime& right);
	bool operator>(const CDateTime& left, const CDateTime& right);
	bool operator<(const CDateTime& left, const CDateTime& right);
	bool operator==(const CDateTime& left, const CDateTime& right);
	bool operator!=(const CDateTime& left, const CDateTime& right);
	
/////////////////////////////////////////////////////////////////////////////////////////

class CDateTimeFormatter
{
private:
	CDateTimeFormatter();
	CMap<CDateTime::DateFormat,CDateTime::DateFormat,LPCSTR,LPCSTR> _datetimefmtsMap;
	static CDateTimeFormatter instance;
public:
	static void FormatDateTime(const CDateTime& dt,CDateTime::DateFormat dtFmt,LPSTR szBuff,int maxsize);

};

class CDateTimeParser
{
private:
	CDateTimeParser();
	CMap<CDateTime::DateFormat,CDateTime::DateFormat,LPCSTR,LPCSTR> _datetimefmtsMap;
	static CDateTimeParser instance;
public:
	static void ParseDateTime(CDateTime::DateFormat dtFmt, LPCSTR dateStr,CDateTime &dt);
};


#endif