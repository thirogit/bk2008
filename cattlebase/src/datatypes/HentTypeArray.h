#ifndef __HENTTYPEARRAY_H__
#define __HENTTYPEARRAY_H__

#include "../SortArray.h"
#include "HentType.h"

class CHentTypeArray : public CSortArray<HentType>
{
public:
	HentType* GetHentTypeByHentTypeId(int htid);
};

#endif