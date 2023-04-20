#ifndef __EVENTLOG_H__
#define __EVENTLOG_H__

#include "UserData.h"

#include "../classes/types/DateTime.h"

class EventLog
{
public:
	EventLog();
	EventLog& operator=(const EventLog &right);

	CDateTime eventDt;
	UserData *user;

};




#endif