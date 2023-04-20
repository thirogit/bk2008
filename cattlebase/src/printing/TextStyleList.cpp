#include "stdafx.h"
#include "TextStyleList.h"

CTextStyle* CTextStyleList::GetTextStyle(UINT Id)
{
	CTextStyle *pStyle = NULL;
	POSITION pos = GetHeadPosition();
	while(pos != NULL)
	{
		pStyle = (CTextStyle*)GetNext(pos);
		if(pStyle->GetId() == Id)
			return pStyle;
	}
	return NULL;
}
