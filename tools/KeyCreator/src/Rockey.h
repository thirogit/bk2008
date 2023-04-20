#ifndef __ROCKEY_H__
#define __ROCKEY_H__

#include "../../../rockey_common/RockeyCommon.h"

BOOL WriteRockeyKey(PROCKEY_BKCLIENTINFO pBkClientInfo,WORD wRequiredVersion,time_t tmExpiry,time_t tmClock);


#endif