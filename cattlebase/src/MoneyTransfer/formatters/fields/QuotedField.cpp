#include "stdafx.h"
#include "QuotedField.h"


QuotedField::QuotedField(FormatterField* pField) : m_pField(pField)
{
}

QuotedField::~QuotedField()
{
	delete m_pField;
}

CString QuotedField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	const char cQuote = '\"';
	CString sFieldValue = m_pField->GetFieldValue(pContext,pMoneyTransfer);
	return  cQuote + sFieldValue + cQuote;
}
