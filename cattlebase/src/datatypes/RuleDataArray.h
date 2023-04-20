#ifndef __RULEDATAARRAY_H__
#define __RULEDATAARRAY_H__

#include "../SortArray.h"
#include "Rule.h"

class CRuleDataArray : public CSortArray<Rule> 
{
public:
	Rule* GetPtrByRuleId(int ruleid);
};




#endif