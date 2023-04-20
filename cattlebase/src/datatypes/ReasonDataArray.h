#ifndef __REASONDATAARRAY_H__
#define __REASONDATAARRAY_H__

#include "Reason.h"
#include "../SortArray.h"

class CReasonDataArray : public CSortArray<Reason>
{
public:
	Reason *GetReasonDataByReasonId(int reasonid);
};


#endif