#ifndef __TIME_H__
#define __TIME_H__

#include "NullInt.h"
#include "DateTime.h"

#define NULL_TIME_VALUE (-1)

class Time : public NullInt
{
public:

	enum TimeFormat
	{
		HHcolonMIcolonSS,
		HHcolonMI,
	};

	typedef struct _tagTIMEFMTS
	{
		Time::TimeFormat tmFmt;
		LPCSTR strFmt;
	} TIMEFMTS;


	Time();
	Time(unsigned short hour,unsigned short minutes,unsigned short seconds);
	Time(const CDateTime& datetime);
	Time(const Time& copy);
	Time(int secsFromMidnight);

	bool Set(unsigned short hour,unsigned short minutes,unsigned short seconds); 
	
	Time& operator=(const Time& right);
	
	CString GetTime(Time::TimeFormat format) const;
	bool ParseTime(TimeFormat fmt,const char* inputTm,bool bThrow = false);

	static const Time now();

	unsigned short GetHour() const;
	unsigned short GetMinutes() const;
	unsigned short GetSeconds() const;

	static TIMEFMTS* GetTimeFmts();
protected:

	static TIMEFMTS _timefmts[];

};

bool operator>=(const Time& left, const Time& right);
bool operator<=(const Time& left, const Time& right);
bool operator>(const Time& left, const Time& right);
bool operator<(const Time& left, const Time& right);
bool operator==(const Time& left, const Time& right);
bool operator!=(const Time& left, const Time& right);

class CTimeParser
{
private:
	CTimeParser();
	CMap<Time::TimeFormat,Time::TimeFormat,LPCSTR,LPCSTR> _timefmtsMap;
	static CTimeParser instance;
public:
	static void ParseTime(Time::TimeFormat timeFmt, LPCSTR timeStr,Time &t);
};

class CTimeFormatter
{
private:
	CTimeFormatter();
	CMap<Time::TimeFormat,Time::TimeFormat,LPCSTR,LPCSTR> _timefmtsMap;
	static CTimeFormatter instance;
public:
	static void FormatTime(const Time& t,Time::TimeFormat tmFmt,LPSTR szBuff,int maxsize);

};


#endif
