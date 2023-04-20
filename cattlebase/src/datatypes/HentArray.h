#ifndef __HENTARRAY_H__
#define __HENTARRAY_H__

#include "../DataArray.h"
#include "Hent.h"
#include "HentComparator.h"

class CHentArray : public CDataArray<Hent,HentComparator>
{
public:
	Hent *GetHentByHentId(int Id);
	Hent *GetHentByHentNo(CString &hentNo);

};

#endif