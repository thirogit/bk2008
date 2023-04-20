#ifndef __HENTDATATEXTPROVIDER_H__
#define __HENTDATATEXTPROVIDER_H__

#include "BaseTextProvider.h"
#include "../datatypes/Hent.h"

class HentTextProvider : public BaseTextProvider<Hent>
{
public:
	virtual CString GetDataText(Hent *pData,UINT memberID);

};

#endif