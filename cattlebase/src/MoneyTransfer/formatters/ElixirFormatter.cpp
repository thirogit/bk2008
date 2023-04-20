#include "stdafx.h"
#include "ElixirFormatter.h"
#include "fields/MoneyTransferTitleField.h"
#include "fields/MoneyCentValueField.h"
#include "fields/MoneyTransferDateField.h"
#include "fields/InvoiceHentAddrField.h"
#include "fields/CurrencyCodeField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "fields/ConstantField.h"
#include "../../classes/types/DateTime.h"
#include "fields/Elixir4x35Field.h"
#include "fields/EmptyField.h"
#include "fields/QuotedField.h"
#include "fields/InvoiceHentNameAndAddrField.h"

ElixirFormatter::ElixirFormatter() : DelimetedFormatter(',',"CSV","Elixir-0")
{
	AddField(new ConstantField("110"));
	AddField(new MoneyTransferDateField(CDateTime::YYYYMMDD));
	AddField(new MoneyCentValueField());
	AddField(new EmptyField());
	AddField(new ConstantField("0"));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new InvoiceHentAccountNoField()));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new Elixir4x35Field(new InvoiceHentNameAndAddrField())));
	AddField(new ConstantField("0"));
	AddField(new EmptyField());
	AddField(new QuotedField(new Elixir4x35Field(new MoneyTransferTitleField())));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new ConstantField("51")));
	AddField(new EmptyField());
}
