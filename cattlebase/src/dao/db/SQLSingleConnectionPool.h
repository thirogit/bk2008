#include "SingleConnectionPool.h"
#include "..\..\DatabasePool\fbserver\FBSvrPool.h"

typedef SingleConnectionPool<FBSvrPool> SQLSingleConnectionPool;