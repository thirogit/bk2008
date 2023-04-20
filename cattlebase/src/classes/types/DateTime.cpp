#include "Stdafx.h"
#include "DateTime.h"
#include "../../missing/strptime.h"
#include "../../exceptions/DateTimeException.h"

CDateTime::DATETIMEFMTS CDateTime::_datetimefmts[] =
{
	{CDateTime::YYYYdotMMdotDD, "%Y.%m.%d"},
	{CDateTime::YYYYMMDD,"%Y%m%d"},
	{CDateTime::DDMMYYYY,"%d%m%Y"},
	{CDateTime::YYYYdashMMdashDD,"%Y-%m-%d"},
	{CDateTime::YYYYdotMMdotDD_HHcolonMMcolonSS,"%Y.%m.%d %H:%M:%S"},
	{CDateTime::YYYYMMDDHHMISS,"%Y%m%d%H%M%S"},
	{CDateTime::DDdashMMdashYYYY,"%d-%m-%Y"},
	{CDateTime::DDSMMSYYYY,"%d/%m/%Y"},
	{CDateTime::YYYYdashMMdashDD_HHcolonMI,"%Y-%m-%d %H:%M"},
	{(CDateTime::DateFormat)0,NULL}
};

#define NULL_VALUE (-1)

CDateTime::CDateTime() : m_year(NULL_VALUE),m_month1_12(NULL_VALUE),m_day1_31(NULL_VALUE),
						hour0_23(NULL_VALUE),minute0_59(NULL_VALUE),sec0_59(NULL_VALUE)
{
}

CDateTime::CDateTime(time_t secsSinceEpoch)
{
	SetDateTime(secsSinceEpoch);
}

CDateTime::CDateTime(UINT year,UINT month,UINT day) 
{
	SetYearMonthDay(year,month,day,true);
	SetHourMinSec(0,0,0,true);
}
CDateTime::CDateTime(UINT year,UINT month,UINT day,UINT hour,UINT min,UINT sec)
{
	SetYearMonthDay(year,month,day,true);
	SetHourMinSec(hour,min,sec,true);
}
CDateTime::CDateTime(const CDateTime& copy)
{
	if(!copy.IsNull())
	{
		m_year = copy.m_year;
		m_month1_12 = copy.m_month1_12;
		m_day1_31 = copy.m_day1_31;		
		hour0_23 = copy.hour0_23;
		minute0_59 = copy.minute0_59;
		sec0_59 = copy.sec0_59;
	}
	else
		MakeNull();
}
bool CDateTime::IsNull() const
{
	return m_year == NULL_VALUE || m_month1_12 == NULL_VALUE || m_day1_31 == NULL_VALUE ||
						hour0_23 == NULL_VALUE || minute0_59 == NULL_VALUE || sec0_59 == NULL_VALUE;
}

LPCSTR CDateTime::GetDateInFmt(DateFormat dtFmt) const
{	
	CDateTimeFormatter::FormatDateTime(*this,dtFmt,const_cast<LPSTR>(m_dateBuff),MAXDATEBUFF);
	return m_dateBuff;
}
LPCSTR CDateTime::GetDayDate() const
{
	return GetDateInFmt(YYYYdashMMdashDD); 
}
LPCSTR CDateTime::GetFullDate() const
{
	return GetDateInFmt(YYYYdotMMdotDD_HHcolonMMcolonSS); 
}

UINT CDateTime::GetYear() const
{
	return m_year;
}
UINT CDateTime::GetMonth() const
{
	return m_month1_12;	
}
UINT CDateTime::GetDay() const
{
	return m_day1_31;	
}
UINT CDateTime::GetHour() const
{
	return hour0_23;	
}
UINT CDateTime::GetMinutes() const
{
	return minute0_59;	
}
UINT CDateTime::GetSeconds() const
{
	return sec0_59;
}

bool CDateTime::SetYearMonthDay(UINT year,UINT month,UINT day,bool bThrow/* = false*/)
{
	static UINT _monthDays[] = {0,
							   31, // Jan
							   0 , // Feb
							   31, // Mar
							   30, // Apr
							   31, // May
							   30, // Jun
							   31, // Jul
							   31, // Aug
							   30, // Sep
							   31, // Oct
							   30, // Nov
							   31  // Dec
							  };
	char errorBuf[100];
	

	if(year >=  _BASE_YEAR && year <= _MAX_YEAR)
	{
		if(month >= 1 && month <= 12)
		{
			if(month == 2)
			{
				 if(_IS_LEAP_YEAR(year))
				 {
					 if(day >= 1 && day <= 29)
					 {
						m_year = year;
						m_month1_12 = month;
						m_day1_31 = day;
						SetHourMinSec(0,0,0,bThrow);
						return true;
					 }
					 else
					 {
						sprintf(errorBuf,"%d is a wrong day for February in leap year",day);
						if(bThrow) throw new CDateTimeException(errorBuf);
						
					 }
				 }
				 else
				 {
					if(day >= 1 && day <= 28)
					{
						m_year = year;
						m_month1_12 = month;
						m_day1_31 = day;
						SetHourMinSec(0,0,0,bThrow);
						return true;
					}
					else
					{
						sprintf(errorBuf,"%d is a wrong day for February",day);
						if(bThrow) throw new CDateTimeException(errorBuf);
					}
				 }
				
			}
			else
			{
				if(day >= 1 && day <= _monthDays[month])
				{
					m_year = year;
					m_month1_12 = month;
					m_day1_31 = day;
					SetHourMinSec(0,0,0,bThrow);
					return true;
				}
				else
				{
					sprintf(errorBuf,"%d is a wrong day for month %d",day,month);
					if(bThrow) throw new CDateTimeException(errorBuf);					
				}
			}
		}
		else
		{
			sprintf(errorBuf,"%d is a month",month);
			if(bThrow) throw new CDateTimeException(errorBuf);			
		}
	}
	else
	{
		if(bThrow) throw new CDateTimeException("Accepting only dates from 1970 to 2038");
	}
	
	return false;
}
bool CDateTime::SetHourMinSec(UINT hour,UINT minutes,UINT seconds,bool bThrow/* = false*/)
{
	if((hour >= 0 && hour <= 23) &&
	   (minutes >= 0 && minutes <= 59) &&
	   (seconds >= 0 && seconds <= 59))
	{
		hour0_23 = hour;
		minute0_59 = minutes;
		sec0_59 = seconds;
		return true;
	}
	else
	{
		if(bThrow) throw new CDateTimeException("Invalid time values");
	}
	
	return false;
}
	
const CDateTime CDateTime::now()
{
	time_t nowTime = time(NULL);
	struct tm *tm1 = localtime(&nowTime);
	return CDateTime(tm1->tm_year+1900,tm1->tm_mon+1,tm1->tm_mday,tm1->tm_hour,tm1->tm_min,tm1->tm_sec);
}

CDateTime::DATETIMEFMTS* CDateTime::GetDateTimeFmts()
{
	return _datetimefmts;
}

int CDateTime::GetDiffrenceInSeconds(const CDateTime& dt) const
{
	if(dt.IsNull() || IsNull()) return -1;
	return dt.SecondsSinceEpoh()-SecondsSinceEpoh();
}

UINT CDateTime::SecondsSinceEpoh() const
{
        UINT tmpy, tmpmondays, tmpyeardays;
		int _days[] = { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };
        
		tmpy = GetYear();

               
        tmpmondays = _days[GetMonth()-1];
        if ( 
			!(tmpy & 3) && // in years 1970 - 2038 leap years are mod 4 
			(GetMonth() > 2) // if leap year and current month > Feb
			)
                tmpmondays++;

        tmpyeardays = (tmpy - _BASE_YEAR) * 365L + ((tmpy - 1L - 1900L) >> 2) - _LEAP_YEAR_ADJUST;        
        
        UINT totalDays = tmpyeardays + tmpmondays + GetDay();
        
        UINT totalHours = totalDays * 24L + GetHour();
        
        UINT totalMinutes = totalHours * 60L + GetMinutes();
                
        UINT totalSeconds = totalMinutes * 60L + GetSeconds(); 
      
		return totalSeconds;
}


void CDateTime::SetDateTime(time_t secsSinceEpoch)
{
   
	time_t caltim = secsSinceEpoch;   /* calendar time to convert */
	int islpyr = 0;                 /* is-current-year-a-leap-year flag */
	int tmptim;
	int *mdays = NULL;                     /* pointer to days or lpdays */

	UINT tm_year = 0,tm_yday = 0,tm_mon = 0,tm_mday = 0,tm_sec = 0,
		tm_min = 0,tm_hour = 0;

	int _lpdays[] = {
         -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
	};

	int _days[] = {
			 -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
	};

    /*
     * Determine the years since 1900. Start by ignoring leap years.
     */
    tmptim = (LONG)(caltim / SECONDS_IN_YEAR) + 70;
    caltim -= ((LONG)(tmptim - 70) * SECONDS_IN_YEAR);

    /*
     * Correct for elapsed leap years
     */
    caltim -= ((LONG)_ELAPSED_LEAP_YEARS(tmptim) * SECONDS_IN_DAY);

    /*
     * If we have underflowed the __time64_t range (i.e., if caltim < 0),
     * back up one year, adjusting the correction if necessary.
     */
    if ( caltim < 0 ) 
	{
        caltim += (__time64_t)SECONDS_IN_YEAR;
        tmptim--;
        if ( _IS_LEAP_YEAR(tmptim) ) 
		{
            caltim += SECONDS_IN_DAY;
            islpyr++;
        }
    }
    else
        if ( _IS_LEAP_YEAR(tmptim) )
            islpyr++;

    /*
     * tmptim now holds the value for tm_year. caltim now holds the
     * number of elapsed seconds since the beginning of that year.
     */
    tm_year = tmptim;

    /*
     * Determine days since January 1 (0 - 365). This is the tm_yday value.
     * Leave caltim with number of elapsed seconds in that day.
     */
    tm_yday = (UINT)(caltim / SECONDS_IN_DAY);
    caltim -= (LONG)(tm_yday) * SECONDS_IN_DAY;

    /*
     * Determine months since January (0 - 11) and day of month (1 - 31)
     */
    if ( islpyr )
        mdays = _lpdays;
    else
        mdays = _days;


    for ( tmptim = 1 ; mdays[tmptim] < tm_yday ; tmptim++ ) ;

    tm_mon = --tmptim;

    tm_mday = tm_yday - mdays[tmptim];

    /*
     *  Determine hours since midnight (0 - 23), minutes after the hour
     *  (0 - 59), and seconds after the minute (0 - 59).
     */
    tm_hour = (UINT)(caltim / 3600);
    caltim -= (LONG)tm_hour * 3600L;

    tm_min = (UINT)(caltim / 60);
    tm_sec = (UINT)(caltim - tm_min * 60);

	SetYearMonthDay(tm_year+1900,tm_mon+1,tm_mday,true);
	SetHourMinSec(tm_hour,tm_min,tm_sec,true);
	
       
}


bool CDateTime::SetTmStruct(struct tm &tm1) const
{
	if(IsNull()) return false;
	
	memset(&tm1,0,sizeof(struct tm));

	tm1.tm_year = m_year-1900;
	tm1.tm_mon = m_month1_12-1;
	tm1.tm_mday = m_day1_31;
	tm1.tm_hour = hour0_23;
	tm1.tm_min = minute0_59;
	tm1.tm_sec = sec0_59;

	return true;
}

int CDateTime::GetDiffrenceInDays(const CDateTime& dt) const
{
	if(dt.IsNull() || IsNull()) return -1;
	return GetDiffrenceInSeconds(dt) / (SECONDS_IN_DAY);	
}

void CDateTime::MakeNull()
{
	m_year = NULL_VALUE;
	m_month1_12 = NULL_VALUE;
	m_day1_31 = NULL_VALUE;
	
	hour0_23 = NULL_VALUE;
	minute0_59 = NULL_VALUE;
	sec0_59 = NULL_VALUE;
}

bool CDateTime::ParseDate(DateFormat fmt,const char* inputDt,bool bThrow/* = false*/)
{
	try
	{
		if(!inputDt || !strlen(inputDt))
			MakeNull();
		else
			CDateTimeParser::ParseDateTime(fmt,inputDt,*this);

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

bool CDateTime::EqualDay(const CDateTime& dt) const
{
	return GetYear() == dt.GetYear() && GetMonth() == dt.GetMonth() && GetDay() == dt.GetDay();
}

bool operator>=(const CDateTime& left, const CDateTime& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return	left.SecondsSinceEpoh()   >= right.SecondsSinceEpoh();
	
}

bool operator<=(const CDateTime& left, const CDateTime& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return left.SecondsSinceEpoh() <= right.SecondsSinceEpoh();
	
}

bool operator>(const CDateTime& left, const CDateTime& right)
{
	if(left.IsNull())
	{
		return false;
	}
	else
		if(right.IsNull()) return true;

	return left.SecondsSinceEpoh() > right.SecondsSinceEpoh();
		
}

bool operator<(const CDateTime& left, const CDateTime& right)
{
	return (right > left);
}


bool operator==(const CDateTime& left, const CDateTime& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(right.IsNull() || left.IsNull()) return false;

	return left.SecondsSinceEpoh() == right.SecondsSinceEpoh();
}

bool operator!=(const CDateTime& left, const CDateTime& right)
{
	return !(left == right);
}

CDateTimeFormatter CDateTimeFormatter::instance;

CDateTimeFormatter::CDateTimeFormatter()
{
	CDateTime::DATETIMEFMTS *dateFmt = CDateTime::GetDateTimeFmts();
	while(dateFmt->strFmt)
	{
		_datetimefmtsMap.SetAt(dateFmt->dtFmt,dateFmt->strFmt);
		dateFmt++;
	}

}

void CDateTimeFormatter::FormatDateTime(const CDateTime& dt,CDateTime::DateFormat dtFmt,LPSTR szBuff,int maxsize)
{
	LPCSTR strFmt = NULL;

	if(dt.IsNull())
		szBuff[0] = '\0';
	else
	{
		if(instance._datetimefmtsMap.Lookup(dtFmt,strFmt))
		{
			struct tm tm1;
			dt.SetTmStruct(tm1);
			strftime(szBuff,maxsize,strFmt,&tm1);
		}
		else
			throw new CDateTimeException("FormatDateTime(): Unknown format specified");	
	}
}

/***************************************************************************************/

CDateTimeParser CDateTimeParser::instance;

CDateTimeParser::CDateTimeParser()
{
	CDateTime::DATETIMEFMTS *dateFmt = CDateTime::GetDateTimeFmts();
	while(dateFmt->strFmt)
	{
		_datetimefmtsMap.SetAt(dateFmt->dtFmt,dateFmt->strFmt);
		dateFmt++;
	}
}
	
void CDateTimeParser::ParseDateTime(CDateTime::DateFormat dtFmt, LPCSTR dateStr,CDateTime &dt)
{
	struct tm tm1;
	memset(&tm1,0,sizeof(tm1));

	LPCSTR strFmt = NULL;
	if(instance._datetimefmtsMap.Lookup(dtFmt,strFmt))
	{
		if(!strptime(dateStr,strFmt,&tm1)) 
		{
			throw new CDateTimeException("ParseDateTime(): Date do not match format specified");		
		}
		else
		{			
			if(!(dt.SetYearMonthDay(tm1.tm_year+1900,tm1.tm_mon+1,tm1.tm_mday) &&
				 dt.SetHourMinSec(tm1.tm_hour,tm1.tm_min,tm1.tm_sec)))
				 throw new CDateTimeException("ParseDateTime(): Bad date.");
		}
	}
	else
	{
		throw new CDateTimeException("ParseDateTime(): Unknown format specified");		
	}
}
