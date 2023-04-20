#ifndef __MONEYTRANSFERTEXTPROVIDER_H__
#define __MONEYTRANSFERTEXTPROVIDER_H__

#include "../../textproviders/BaseTextProvider.h"
#include "../MoneyTransferEntry.h"

class MoneyTransferEntryTextProvider : public BaseTextProvider<MoneyTransferEntry>
{
public:
	virtual CString GetDataText(MoneyTransferEntry *pData,UINT memberID);

};

#endif