#ifndef __REPOSTOCKTEXTPROVIDER_H__
#define __REPOSTOCKTEXTPROVIDER_H__

#include "../../textproviders/BaseTextProvider.h"
#include "../RepoStock.h"

class RepoStockTextProvider : public BaseTextProvider<RepoStock>
{
public:
	virtual CString GetDataText(RepoStock *pData,UINT memberId);

};

#endif