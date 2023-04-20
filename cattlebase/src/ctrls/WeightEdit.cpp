#include "stdafx.h"
#include "WeightEdit.h"
#include "../missing/util.h"


CWeightEdit::CWeightEdit() : CFilterEdit("[0-9]{1,6}([,.][0-9]{0,3})?")
{
	
}

NullDouble CWeightEdit::GetWeight() const
{
	CString sWeight;
	GetWindowText(sWeight);

	if(sWeight.GetLength() > 0)
		return NullDouble(CUtil::myatof(sWeight));

	return NullDouble();
}

void  CWeightEdit::SetWeight(const NullDouble& weight)
{
	

}