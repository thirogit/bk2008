#ifndef __COWARRAY_H__
#define __COWARRAY_H__

#include "../DataArray.h"
#include "Cow.h"
#include "CowDataComparator.h"
#include "../RefCountingPtr.h"

class CCowArray;

typedef  RefCountingPtr<CCowArray> CowArrayPtr ;


class CCowArray : public CDataArray<Cow,CowDataComparator>
{
public:
	Cow* GetCowByEAN(const CString &ean);
	Cow* GetCowByCowId(int cowid);	
	void CommitChanges();
	void RollbackChanges();
	void AppendPtr(CowArrayPtr cowPtr);
	void RemoveArrayPtr(CowArrayPtr cowPtr);
	
	
};




#endif