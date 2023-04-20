#ifndef __RAPORTARIMRDOC_H__
#define __RAPORTARIMRDOC_H__

#include "HerdBook.h"

#define RAPORTPAGESIZE 6

class CRaportARiMR : public CHerdBook
{
	DECLARE_DYNCREATE(CRaportARiMR)
public:
	virtual int GetCowsOnPage() const;
	virtual void DrawCowRow(int iRow,int iSerialNum,const Cow* pCow);

};

#endif
