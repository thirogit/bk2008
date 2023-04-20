#ifndef __DOCDATAARRAY_H__
#define __DOCDATAARRAY_H__

#include "Doc.h"
#include "DocDataComparator.h"

class CDocDataArray : public  CDataArray<Doc,DocDataComparator>
{
public:
	Doc *GetDocDataByDocId(int No);
	void CommitChanges();
	void RollbackChanges();
};





#endif