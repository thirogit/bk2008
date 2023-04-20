#ifndef __DATAARRAY_H__
#define __DATAARRAY_H__

#include "SortArray.h"


class DataMemberSortParams 
{
public:
	DataMemberSortParams() : m_sortByDataMember((UINT)-1),m_bAsc(false) {};
	DataMemberSortParams(UINT sortByDataMember,bool bAsc) : m_sortByDataMember(sortByDataMember),m_bAsc(bAsc) {};
	bool IsAscending() const { return m_bAsc; }
	UINT GetDataMember() const { return m_sortByDataMember; }
	void SetAscending(bool bAsc) { m_bAsc = bAsc; };

protected:
	UINT m_sortByDataMember;
	bool m_bAsc;
}; 

typedef CArray<DataMemberSortParams,DataMemberSortParams&> DataMemberSortParamsArray;



template<class DATATYPE,class COMPARATORTYPE>
class CDataArray : public CSortArray<DATATYPE>
{
public:
	class DataMemberComparator : public SortArrayComparator,public COMPARATORTYPE
	{
	public:	
		DataMemberComparator(UINT nDataMember,bool bAsc) : SortArrayComparator(bAsc),m_nDataMember(nDataMember)
		{
		}
		virtual COMPARE_RESULT Compare(DATATYPE *DataLeft,DATATYPE *DataRight)
		{			
			return CompareBy(m_nDataMember,DataLeft,DataRight);
		}
	protected:
		UINT m_nDataMember;
	};

	
	virtual void SortBy(UINT sortBy,bool bAscending)
	{
		Sort(DataMemberComparator(sortBy,bAscending));
	}

	virtual void SortBy(DataMemberSortParamsArray &sort_by)
	{
		SortArrayComparatorArray sortComparators;
		for(int i = 0;i < sort_by.GetCount();i++)
		{
			DataMemberSortParams sortParam = sort_by[i];
			sortComparators.Add(new DataMemberComparator(sortParam.GetDataMember(),sortParam.IsAscending()));
		}
		Sort(sortComparators);
		sortComparators.FreeAndFlush();
	}


};


#endif