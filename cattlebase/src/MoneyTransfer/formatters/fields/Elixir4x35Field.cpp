#include "stdafx.h"
#include "Elixir4x35Field.h"


Elixir4x35Field::Elixir4x35Field(FormatterField* pField) : m_pField(pField)
{
}

Elixir4x35Field::~Elixir4x35Field()
{
	delete m_pField;
}

CString Elixir4x35Field::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	CString s4x35Value;

	CString sValue = m_pField->GetFieldValue(pContext,pMoneyTransfer);
	sValue.Remove('|');

	CString sLine;
	int iFirst = 0;
	for(int i = 0; i < 4;i++)
	{
		if(iFirst >= sValue.GetLength())
			break;
		sLine = sValue.Mid(iFirst,35);

		s4x35Value += sLine;

		iFirst += sLine.GetLength();

		if(iFirst < sValue.GetLength())
			s4x35Value += '|';
	}
	return s4x35Value;
}

