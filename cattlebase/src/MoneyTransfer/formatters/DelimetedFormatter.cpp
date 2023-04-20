#include "stdafx.h"
#include "DelimetedFormatter.h"


DelimetedFormatter::DelimetedFormatter(char cDelimiter,const CString& sExtension,const CString& sFormatterName) : 
	m_cDelimiter(cDelimiter),m_sExtension(sExtension),m_sFormatterName(sFormatterName)
{
}

DelimetedFormatter::~DelimetedFormatter()
{
	m_fields.FreeAndFlush();
}

void DelimetedFormatter::AddField(FormatterField* pField)
{
	m_fields.Add(pField);
}

CString DelimetedFormatter::GetFormatterName()
{
	return m_sFormatterName;
}

CString DelimetedFormatter::GetFileExtension()
{
	return m_sExtension;
}

void DelimetedFormatter::FormatMoneyTransfers(PtrFlushArray<MoneyTransferEntry>& moneyTransfers,
											  MoneyTransferContext* pContext,
											  CFile& outputFile,
											  ITaskProgres *progress)
{

	CString sFieldValue;
	FormatterField* pField = NULL;
	MoneyTransferEntry* pMoneyTransfer = NULL;
	progress->SetTask(0, moneyTransfers.GetCount());

	PtrFlushArray<MoneyTransferEntry>::ArrayIterator moneyTransferIt = moneyTransfers.iterator();
	while(moneyTransferIt.hasNext())
	{
		pMoneyTransfer = *(moneyTransferIt++);
		PtrFlushArray<FormatterField>::ArrayIterator fieldIt = m_fields.iterator();
		while(fieldIt.hasNext())
		{
			pField = *(fieldIt++);
			sFieldValue = pField->GetFieldValue(pContext,pMoneyTransfer);
			
			outputFile.Write((LPCSTR)sFieldValue,sFieldValue.GetLength());
			if(fieldIt.hasNext())
				WriteDelimiter(outputFile);
			
		}

		WriteLineEnd(outputFile);
		if(!progress->OnStep(moneyTransferIt.index()))
			return;
	}
}

void DelimetedFormatter::WriteLineEnd(CFile& outputFile)
{
	outputFile.Write("\r\n",2);
}

void DelimetedFormatter::WriteDelimiter(CFile& outputFile)
{
	outputFile.Write(&m_cDelimiter,1);
}

