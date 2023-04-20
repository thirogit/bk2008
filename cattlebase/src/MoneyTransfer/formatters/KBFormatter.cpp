#include "stdafx.h"
#include "KBFormatter.h"
#include "fields/MoneyTransferTitleField.h"
#include "fields/MoneyFloatValueField.h"
#include "fields/InvoiceHentNameAndNoField.h"
#include "fields/InvoiceHentAddrField.h"
#include "fields/CurrencyCodeField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "../../classes/format/FloatFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KBFormatter::KBFormatter() : DelimetedFormatter(';',"CSV","Kredyt Bank")
{
	AddField(new MoneyTransferTitleField());
	AddField(new MoneyFloatValueField(FloatFormat(2,'.')));
	AddField(new CurrencyCodeField());
	AddField(new InvoiceHentNameAndNoField());
	AddField(new InvoiceHentAddrField());
	AddField(new InvoiceHentAccountNoField());
}
