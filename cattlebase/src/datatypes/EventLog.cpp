#include "stdafx.h"
#include "EventLog.h"


EventLog::EventLog() :user(NULL), eventDt() 
{
}

EventLog& EventLog::operator=(const EventLog &right)
{
	eventDt = right.eventDt;
	user = right.user;
	return *this;
}