#ifndef __TEXTSTYLELIST_H__
#define __TEXTSTYLELIST_H__

#include "TextStyle.h"

class CTextStyleList : public CTypedPtrList<CObList, CTextStyle*> 
{
public:
	CTextStyle* GetTextStyle(UINT Id);
};

#endif