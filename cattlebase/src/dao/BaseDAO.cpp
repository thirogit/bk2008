#include "stdafx.h"
#include "BaseDAO.h"
#include "db/Transaction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Transaction BaseDAO::OpenTransaction()
{
	return Transaction(new TransactionObject());
}
