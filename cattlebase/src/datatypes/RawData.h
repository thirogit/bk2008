#ifndef __RAWDATA_H__
#define __RAWDATA_H__

#include "../PtrFlushArray.h"
#include "UserData.h"
#include "EventLog.h"

#define _COPYUPDATE(field,copy)	if(copy.SQLUpdate.Check(update_##field))	 \
							    {											 \
							    	field = copy.field;                      \
								    SQLUpdate.Set(update_##field,true);      \
							    }


#define CAN_DO(canaction,data) (data->CanDo(UserData::GetCurrentUser(),UserData::canaction))

class RawData
{
public:
	RawData();
	virtual void Copy(const RawData &copy);
	bool CanDo(UserData *pUser,UserData::PRIVACTION action) const;

	EventLog creation;
	EventLog modification;
	bool bMark;

};

enum RAWDATA_MEMBERS
{
	RAWDATA_MARK = 999,
	RAWDATA_MEMBER_BASE = 1000
};

typedef PtrFlushArray<RawData> CRawDataArray;




#endif