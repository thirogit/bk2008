#ifndef __DOCTEXTPROVIDER_H__
#define __DOCTEXTPROVIDER_H__

#include "BaseTextProvider.h"
#include "../datatypes/Doc.h"

class DocTextProvider : public BaseTextProvider<Doc>
{
public:
	virtual CString GetDataText(Doc *pData,UINT memberID);

};

#endif