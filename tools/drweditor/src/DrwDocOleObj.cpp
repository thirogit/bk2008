#include "stdafx.h"
#include "DrwDocOleObj.h"

IMPLEMENT_SERIAL(CDrwDocOleObj, CDocOleObj, 0)


CDrwDocOleObj::CDrwDocOleObj()
{
}

CDrwDocOleObj::CDrwDocOleObj(const CRect& position) : CDocOleObj(position)
{
	
}



