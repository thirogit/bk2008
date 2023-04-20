#ifndef __BASEDAO_H__
#define __BASEDAO_H__

#include "db/Transaction.h"

class BaseDAO 
{
public:
	Transaction OpenTransaction();
};


#endif