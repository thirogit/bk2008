#ifndef __BZWBKFORMATTER_H__
#define __BZWBKFORMATTER_H__

#include "DelimetedFormatter.h"

class BZWBKFormatter : public DelimetedFormatter
{
public:
	BZWBKFormatter();
	virtual void FormatMoneyTransfers(	PtrFlushArray<MoneyTransferEntry>& moneyTransfers,
										MoneyTransferContext* pContext,
										CFile& outputFile,
										ITaskProgres *progress);
};

#endif