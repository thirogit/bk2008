#ifndef __KSIEGASTADA_H__
#define __KSIEGASTADA_H__

#include "HerdBook.h"

#define KSIEGASTADA_COWSONPAGE 5

class CKsiegaStada : public CHerdBook
{
	DECLARE_DYNCREATE(CKsiegaStada)
public:
	virtual int GetCowsOnPage() const;
	virtual void DrawCowRow(int iRow,int iSerialNum,const Cow* pCow);
	CString FormatCompany();

};

#endif
