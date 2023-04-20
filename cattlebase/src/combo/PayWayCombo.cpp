#include "stdafx.h"
#include "PayWayCombo.h"
#include "../consts.h"

void CPaywayCombo::ReloadCombo(Invoice::PAYWAYS curSel)
{
	
	ResetContent();
	ModifyStyle(CBS_SORT,0,0);

	struct tagPayWaysPairs
	{
		const char *payWayName;
		Invoice::PAYWAYS payway;
	} 
	PayWayPairs[] =
	{
		{ PayWayNames[1], Invoice::PAY_CASH },
		{ PayWayNames[0], Invoice::PAY_TRANS }
	};
	int i,item,curItem = -1;

	for(i=0;i<(sizeof(PayWayPairs)/sizeof(tagPayWaysPairs));i++)
	{
		item = AddString(PayWayPairs[i].payWayName);
		SetItemData(item,PayWayPairs[i].payway);
		if(curSel == PayWayPairs[i].payway) curItem = item;
	}
	SetCurSel(curItem);

}
	


