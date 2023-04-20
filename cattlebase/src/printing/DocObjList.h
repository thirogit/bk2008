#ifndef __DOCOBJLIST_H__
#define __DOCOBJLIST_H__

#include "DocObj.h"

class CDocObjList : public CTypedPtrList<CObList, CDocObj*> 
{
public:
	CDocObj* GetDocObj(UINT Id);
};

#endif