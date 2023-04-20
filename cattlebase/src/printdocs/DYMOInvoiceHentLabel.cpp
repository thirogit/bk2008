#include "stdafx.h"
#include "DYMOInvoiceHentLabel.h"

IMPLEMENT_DYNCREATE(CDYMOInvoiceHentLabel,CDYMOBaseHentLabel)


CDYMOInvoiceHentLabel::CDYMOInvoiceHentLabel()
{
}

int CDYMOInvoiceHentLabel::LoadInputData(CPtrArray &docInputData)
{
	CHentArray hents;
	CInvoiceDataArray invs;
	invs.Append((CInvoiceDataArray&)docInputData);

	CInvoiceDataArray::ArrayIterator invIt = invs.iterator();
	while(invIt.hasNext())
	{
		hents.Add((*(invIt++))->GetInvoiceHent());
	}
	return CDYMOBaseHentLabel::LoadInputData(hents);
}
