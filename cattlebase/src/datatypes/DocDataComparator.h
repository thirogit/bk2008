#ifndef __DOCDATACOMPARATOR_H__
#define __DOCDATACOMPARATOR_H__

#include "RawData.h"
#include "Doc.h"
#include "RawDataComparator.h"


enum DOCDATA_MEMBERS
{
	DOCS_SORT_BY_NUMBER = RAWDATA_MEMBER_BASE+1,
	DOCS_SORT_BY_DOCDATE,
	DOCS_SORT_BY_HENT,
	DOCS_SORT_BY_LOADDATE,
	DOCS_SORT_BY_PLATENO,
	DOCS_SORT_BY_COWCOUNT,
	DOCS_SORT_BY_INVOICE,
	DOCS_SORT_BY_EXTRAS,
	DOCS_MEMBER_AGENT
	
};


class DocDataComparator : public RawDataComparator<Doc>
{
public:
	virtual COMPARE_RESULT CompareDataBy(UINT dataMember,Doc* DataLeft,Doc* DataRight);
};


#endif