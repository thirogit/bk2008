#include "stdafx.h"
#include "Time.h"
#include "../../Utils/StringFormat.h"
#include "../../missing/strptime.h"
#include "../../exceptions/DateTimeException.h"



Time::TIMEFMTS Time::_timefmts[] =
{
	{Time::HHcolonMIcolonSS, "%H:%M:%S"},
	{Time::HHcolonMI,"%H:%M"},
	{(Time::TimeFormat)0,NULL}
};


Time::Time() : NullInt(NULL_TIME_VALUE)
{
	MakeNull();
}

Time::Time(unsigned short hour,unsigned short minutes,unsigned short seconds) : NullInt(NULL_TIME_VALUE)
{
	if(!Set(hour,minutes,seconds))
		MakeNull();
}

Time::Time(const CDateTime& datetime) : NullInt(NULL_TIME_VALUE)
{
	Set(datetime.GetHour(),datetime.GetMinutes(),datetime.GetSeconds());
}

Time::Time(const Time& copy)
{
	operator=(copy);
}

Time::Time(int secsFromMidnight) : NullInt(NULL_TIME_VALUE)
{
	if(secsFromMidnight > 24*60*60 || secsFromMidnight < 0)
		MakeNull();
	else
	{
		m_iValue = secsFromMidnight;
	}
}

Time::TIMEFMTS* Time::GetTimeFmts()
{
	return _timefmts;
}


bool Time::ParseTime(TimeFormat fmt,const char* inputTm,bool bThrow/* = false*/)
{
	try
	{
		if(!inputTm || !strlen(inputTm))
			MakeNull();
		else
			CTimeParser::ParseTime(fmt,inputTm,*this);

		return true;
	}
	catch(CDateTimeException *e)
	{
		if(bThrow) 
			throw e;
		else
			e->Delete();
	}
	return false;
}

Time& Time::operator=(const Time& right)
{
	m_iValue = right.m_iValue;
	return *this;
}

const Time Time::now()
{
	CDateTime nowDt = CDateTime::now();
	return Time(nowDt.GetHour(),nowDt.GetMinutes(),nowDt.GetSeconds());
}

CString Time::GetTime(Time::TimeFormat format) const
{
	char tmBuff[100];
	CTimeFormatter::FormatTime(*this,format,tmBuff,sizeof(tmBuff));
	return tmBuff;
}


unsigned short Time::GetHour() const
{
	if(!IsNull())
		return m_iValue/3600;

	return (unsigned short)(-1);
}
unsigned short Time::GetMinutes() const
{
	if(!IsNull())
		return (m_iValue%3600)/60;
	return (unsigned short)(-1);
}
unsigned short Time::GetSeconds() const
{
	if(!IsNull())
		return (m_iValue%3600)%60;
	return (unsigned short)(-1);
}

bool Time::Set(unsigned short hour,unsigned short minutes,unsigned short seconds)
{
	if( hour > 23 || minutes > 60 || seconds > 60)
		return false;
	m_iValue = hour*60*60 + minutes*60 + seconds;
	return true;
}

bool operator>=(const Time& left, const Time& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return	left.ToInt()   >= right.ToInt();
	
}

bool operator<=(const Time& left, const Time& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return left.ToInt() <= right.ToInt();
	
}

bool operator>(const Time& left, const Time& right)
{
	if(left.IsNull())
	{
		return false;
	}
	else
		if(right.IsNull()) return true;

	return left.ToInt() > right.ToInt();
		
}

bool operator<(const Time& left, const Time& right)
{
	return (right > left);
}


bool operator==(const Time& left, const Time& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(right.IsNull() || left.IsNull()) return false;

	return left.ToInt() == right.ToInt();
}

bool operator!=(const Time& left, const Time& right)
{
	return !(left == right);
}



///////////////////////////////////////////////////////////////////////////////

CTimeParser CTimeParser::instance;

CTimeParser::CTimeParser()
{
	Time::TIMEFMTS *timeFmt = Time::GetTimeFmts();
	while(timeFmt->strFmt)
	{
		_timefmtsMap.SetAt(timeFmt->tmFmt,timeFmt->strFmt);
		timeFmt++;
	}
}
	
void CTimeParser::ParseTime(Time::TimeFormat timeFmt, LPCSTR timeStr,Time &t)
{
	struct tm tm1;
	memset(&tm1,0,sizeof(tm1));

	LPCSTR strFmt = NULL;
	if(instance._timefmtsMap.Lookup(timeFmt,strFmt))
	{
		if(!strptime(timeStr,strFmt,&tm1)) 
		{
			throw new CDateTimeException("ParseTime(): Time do not match format specified");		
		}
		else
		{			
			if(!(t.Set(tm1.tm_hour,tm1.tm_min,tm1.tm_sec)))
				 throw new CDateTimeException("ParseTime(): Bad time.");
		}
	}
	else
	{
		throw new CDateTimeException("ParseTime(): Unknown format specified");		
	}
}

///////////////////////////////////////////////////////////////////////////////

CTimeFormatter CTimeFormatter::instance;

CTimeFormatter::CTimeFormatter()
{
	Time::TIMEFMTS *tmFmt = Time::GetTimeFmts();
	while(tmFmt->strFmt)
	{
		_timefmtsMap.SetAt(tmFmt->tmFmt,tmFmt->strFmt);
		tmFmt++;
	}

}

void CTimeFormatter::FormatTime(const Time& t,Time::TimeFormat tmFmt,LPSTR szBuff,int maxsize)
{
	LPCSTR strFmt = NULL;

	if(t.IsNull())
		szBuff[0] = '\0';
	else
	{
		if(instance._timefmtsMap.Lookup(tmFmt,strFmt))
		{
			struct tm tm1;

			memset(&tm1,0,sizeof(struct tm));

			tm1.tm_year = 0;
			tm1.tm_mon = 0;
			tm1.tm_mday = 0;
			tm1.tm_hour = t.GetHour();
			tm1.tm_min = t.GetMinutes();
			tm1.tm_sec = t.GetSeconds();
		
			strftime(szBuff,maxsize,strFmt,&tm1);
		}
		else
			throw new CDateTimeException("FormatTime(): Unknown format specified");	
	}
}
