#ifndef __HERDARRAY_H__
#define __HERDARRAY_H__

#include "../DataArray.h"
#include "Herd.h"
#include "HerdComparator.h"


class CHerdArray : public CDataArray<Herd,HerdComparator>
{
public:
	Herd* GetHerdById(UINT Id);
	Herd* GetHerdByCode(const CString& sHerdCode);	
	void CommitChanges();
	void RollbackChanges();	
};

#endif