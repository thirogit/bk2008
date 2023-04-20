#ifndef __FORMATTER_H__
#define __FORMATTER_H__

#include "../../datatypes/InvoiceDataArray.h"
#include "../../taskprogress/ProgressIFace.h"
#include "../MoneyTransferEntry.h"
#include "../MoneyTransferContext.h"

class Formatter
{
public:
	virtual CString GetFormatterName() = 0;
	virtual CString GetFileExtension() = 0;
	virtual void FormatMoneyTransfers(  PtrFlushArray<MoneyTransferEntry>& moneyTransfers,
										MoneyTransferContext* pContext,
										CFile& outputFile,
										ITaskProgres *progress) = 0;
	virtual ~Formatter() {};
};

#endif