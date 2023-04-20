#ifndef __DELIMETEDFORMATTER_H__
#define __DELIMETEDFORMATTER_H__

#include "Formatter.h"
#include "../../PtrFlushArray.h"
#include "fields/FormatterField.h"

class DelimetedFormatter : public Formatter
{
public:
	DelimetedFormatter(char cDelimiter,const CString& sExtension,const CString& sFormatterName);
	virtual ~DelimetedFormatter();
	void AddField(FormatterField* pField);

	virtual CString GetFormatterName();
	virtual CString GetFileExtension();
	virtual void FormatMoneyTransfers(	PtrFlushArray<MoneyTransferEntry>& moneyTransfers,
										MoneyTransferContext* pContext,
										CFile& outputFile,
										ITaskProgres *progress);
protected:
	void WriteLineEnd(CFile& outputFile);
	void WriteDelimiter(CFile& outputFile);
protected:
	PtrFlushArray<FormatterField> m_fields;
	char m_cDelimiter;
	CString m_sExtension;
	CString m_sFormatterName;

};

#endif