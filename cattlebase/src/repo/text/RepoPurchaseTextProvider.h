#ifndef __REPOPURCHASETEXTPROVIDER_H__
#define __REPOPURCHASETEXTPROVIDER_H__

#include "../../textproviders/BaseTextProvider.h"
#include "../RepoPurchase.h"

class RepoPurchaseTextProvider : public BaseTextProvider<RepoPurchase>
{
public:
	virtual CString GetDataText(RepoPurchase *pData,UINT memberId);

};

#endif