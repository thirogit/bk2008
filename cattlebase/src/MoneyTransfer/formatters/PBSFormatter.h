#ifndef __PBSFORMATTER_H__
#define __PBSFORMATTER_H__

#include "Formatter.h"
#include <xercesc/dom/DOM.hpp>

class PBSFormatter : public Formatter
{
public:
	PBSFormatter();	
	virtual CString GetFormatterName();
	virtual CString GetFileExtension();
	virtual void FormatMoneyTransfers(  PtrFlushArray<MoneyTransferEntry>& moneyTransfers,
										MoneyTransferContext* pContext,
										CFile& outputFile,
										ITaskProgres *progress);
	virtual ~PBSFormatter();
private:
	void SerializeToXml(MoneyTransferContext* pContext,MoneyTransferEntry* pEntry,xercesc::DOMElement * pParentElement);


};

#endif