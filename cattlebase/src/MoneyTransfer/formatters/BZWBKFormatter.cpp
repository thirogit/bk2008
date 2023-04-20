#include "stdafx.h"
#include "BZWBKFormatter.h"
#include "fields/MoneyTransferTitleField.h"
#include "fields/MoneyFloatValueField.h"
#include "fields/InvoiceHentNameAndNoField.h"
#include "fields/InvoiceHentAddrField.h"
#include "fields/CurrencyCodeField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "fields/ConstantField.h"
#include "fields/MoneyTransferDateField.h"
#include "fields/MaxSizeField.h"
#include "fields/CompanySourceAccountNo.h"
#include "fields/InvoiceHentCityField.h"
#include "fields/InvoiceHentStreetAddrField.h"
#include "fields/InvoiceHentZipField.h"
#include "../../classes/format/FloatFormat.h"
#include "fields/EmptyField.h"

BZWBKFormatter::BZWBKFormatter() : DelimetedFormatter('|',"CSV","BZ WBK")
{
	AddField(new ConstantField("1"));
	AddField(new CompanySourceAccountNo());
	AddField(new InvoiceHentAccountNoField());
	AddField(new MaxSizeField(new InvoiceHentNameAndNoField(),80));
	AddField(new MaxSizeField(new InvoiceHentAddrField(),60));
	AddField(new MoneyFloatValueField(FloatFormat(2,',')));
	AddField(new ConstantField("1"));
	AddField(new MaxSizeField(new MoneyTransferTitleField(),140));
	AddField(new EmptyField());
	//AddField(new MoneyTransferDateField(CDateTime::DDdashMMdashYYYY));	
}

void BZWBKFormatter::FormatMoneyTransfers(PtrFlushArray<MoneyTransferEntry>& moneyTransfers,
										MoneyTransferContext* pContext,
										CFile& outputFile,
										ITaskProgres *progress)
{
	outputFile.Write("4120414",7);
	WriteDelimiter(outputFile);
	outputFile.Write("1",1);
	WriteLineEnd(outputFile);
	DelimetedFormatter::FormatMoneyTransfers(moneyTransfers,pContext,outputFile,progress);
}