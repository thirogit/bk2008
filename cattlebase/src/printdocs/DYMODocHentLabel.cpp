#include "stdafx.h"
#include "DYMODocHentLabel.h"

IMPLEMENT_DYNCREATE(CDYMODocHentLabel,CDYMOBaseHentLabel)


CDYMODocHentLabel::CDYMODocHentLabel()
{
}

int CDYMODocHentLabel::LoadInputData(CPtrArray &docInputData)
{
	CHentArray hents;
	CDocDataArray docs;
	docs.Append((CDocDataArray&)docInputData);

	CDocDataArray::ArrayIterator docIt = docs.iterator();
	while(docIt.hasNext())
	{
		hents.Add((*(docIt++))->GetHent());
	}
	return CDYMOBaseHentLabel::LoadInputData(hents);
}

