#include "stdafx.h"
#include "PekaoFormatter.h"
#include "fields/MoneyTransferTitleField.h"
#include "fields/MoneyFloatValueField.h"
#include "fields/InvoiceHentCityZipField.h"
#include "fields/InvoiceHentStreetAddrField.h"
#include "fields/InvoiceHentNameAndNoField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "fields/EmptyField.h"
#include "fields/CompanySourceAccountNo.h"
#include "../../classes/format/FloatFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PekaoFormatter::PekaoFormatter() : DelimetedFormatter('|',"CSV","Peakao")
{
	AddField(new EmptyField());
	AddField(new CompanySourceAccountNo());
	AddField(new InvoiceHentAccountNoField());
	AddField(new InvoiceHentNameAndNoField());
	AddField(new InvoiceHentStreetAddrField());
	AddField(new InvoiceHentCityZipField());	
	AddField(new MoneyFloatValueField(FloatFormat(2,',')));
	AddField(new MoneyTransferTitleField());
	AddField(new EmptyField());
	
}
