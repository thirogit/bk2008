#ifndef __MEMBERDATACHANGE_H__
#define __MEMBERDATACHANGE_H__

#include "UserData.h"
#include "../classes/types/DataTime.h"

template<class MEMBERTYPE>
class MemberDataChange
{
	MemberDataChange(UINT aChangeId)
	{
		changeId = aChangeId;
	}

	UINT GetChangeId() const
	{
		return changeId;
	}

	CDateTime GetChangeDate() const
	{
		return changeDate;
	}

	MEMBERTYPE GetChangedValue() const
	{
		return changedValue;
	}

	UserData* GetChangedBy() const
	{
		return changedBy;
	}

	void SetChangeDate(const CDateTime& aChangeDate)
	{
		changeDate = aChangeDate;
	}
	void SetChangedValue(MEMBERTYPE aOldValue)
	{
		changedValue = aOldValue;
	}
	void SetChangedBy(UserData* pUser)
	{
		changedBy = pUser;
	}

private:
	UINT changeId;
	CDateTime changeDate;
	MEMBERTYPE changedValue;
	UserData* changedBy;
};

#endif