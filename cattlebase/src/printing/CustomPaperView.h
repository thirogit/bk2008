#ifndef __CUSTOMPAPERVIEW_H__
#define __CUSTOMPAPERVIEW_H__

#include "BaseDocView.h"

class CCustomPaperView  : public CBaseDocView
{
public:
	virtual CSize GetPaperSize() = 0;
}; 

#endif 
