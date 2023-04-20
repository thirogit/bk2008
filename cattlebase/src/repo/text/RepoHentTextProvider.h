#ifndef __REPOHENTDATATEXTPROVIDER_H__
#define __REPOHENTDATATEXTPROVIDER_H__

#include "../../textproviders/BaseTextProvider.h"
#include "../RepoHent.h"

class RepoHentTextProvider : public BaseTextProvider<RepoHent>
{
public:
	virtual CString GetDataText(RepoHent *pData,UINT memberId);

};

#endif