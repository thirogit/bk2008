#include "stdafx.h"
#include "RawData.h"


RawData::RawData() : creation(),modification(),bMark(false) 
{
}
void RawData::Copy(const RawData &copy)
{
	creation = copy.creation;
	modification = copy.modification;
}


bool RawData::CanDo(UserData *pUser,UserData::PRIVACTION action) const
{
	bool bCan = false,
		 ownedBythis = creation.user == pUser;

	switch(action)
	{
	case UserData::CANDELETE:
		if(ownedBythis)
			bCan = pUser->HasPriv(UserData::DELOWN);
		else
			bCan = pUser->HasPriv(UserData::DELFORG);
	break;
	case UserData::CANEDIT:
		if(ownedBythis)
			bCan = pUser->HasPriv(UserData::ADDMODOWN);
		else
			bCan = pUser->HasPriv(UserData::MODFORG);
	break;
	}

	return bCan;
}
