#include "stdafx.h"
#include "GroupList.h"



CDocObjGroup* CGroupList::GetGroup(UINT Id)
{
	CDocObjGroup *pGroup = NULL;
	POSITION pos = GetHeadPosition();
	while(pos != NULL)
	{
		pGroup = (CDocObjGroup*)GetNext(pos);
		if(pGroup->GetId() == Id)
			return pGroup;
	}
	return NULL;
}
