#ifndef __RAWDATACOMPARATOR_H__
#define __RAWDATACOMPARATOR_H__

#include "../DataComparator.h"

template<class DATATYPE>
class RawDataComparator : public DataComparator<DATATYPE>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,DATATYPE* DataLeft,DATATYPE* DataRight)
	{
		switch(dataMember)
		{
			case RAWDATA_MARK:
				if(DataLeft->bMark && DataRight->bMark) 
					return COMPARE_EQUAL;
				if(DataLeft->bMark > DataRight->bMark) 
					return COMPARE_GREATHER;
				return COMPARE_LESS;
		}
		return CompareDataBy(dataMember,DataLeft,DataRight);
	};
protected:
	virtual COMPARE_RESULT CompareDataBy(UINT dataMember,DATATYPE* DataLeft,DATATYPE* DataRight) = 0;

	
};


#endif