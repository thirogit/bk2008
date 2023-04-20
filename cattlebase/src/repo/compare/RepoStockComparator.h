#ifndef __REPOSTOCKCOMPARATOR_H__
#define __REPOSTOCKCOMPARATOR_H__

#include "../RepoStock.h"
#include "DataComparator.h"

enum REPOSTOCK_MEMBERS
{
	REPOSTOCK_MEMBER_NAME  = 1,	
	REPOSTOCK_MEMBER_CODE,
    REPOSTOCK_MEMBER_MYPRICE,
    REPOSTOCK_MEMBER_PREDEFSEX,

   	REPOSTOCK_MEMBER_MAX
};


class RepoStockComparator : public DataComparator<RepoStock>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,RepoStock* DataLeft,RepoStock* DataRight);
};


#endif