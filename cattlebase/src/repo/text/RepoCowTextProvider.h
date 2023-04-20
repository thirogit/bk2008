#ifndef __REPOCOWTEXTPROVIDER_H__
#define __REPOCOWTEXTPROVIDER_H__

#include "../../textproviders/BaseTextProvider.h"
#include "../RepoCow.h"

class RepoCowTextProvider : public BaseTextProvider<RepoCow>
{
public:
	virtual CString GetDataText(RepoCow *pData,UINT memberId);

};

#endif