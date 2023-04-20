#ifndef __GROUPLIST_H__
#define __GROUPLIST_H__

#include "DocObjGroup.h"

class CGroupList : public CTypedPtrList<CObList, CDocObjGroup*> 
{
public:
	CDocObjGroup* GetGroup(UINT Id);
};




#endif
