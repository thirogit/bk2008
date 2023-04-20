#ifndef __MEMBERDIFF_H__
#define __MEMBERDIFF_H__

#include "../datatypes/datacopy/DataCopy.h"
#include "../DataComparator.h"

template<class DATA,class MEMBERS_ENUM,class DATACOMPARATOR>
class CMemberDiff
{
public:
	CMemberDiff(MEMBERS_ENUM dataMember,CDataCopy<DATA> *pDataCopy) :
	m_dataMember(dataMember),	m_pDataCopy(pDataCopy) {};	

	~CMemberDiff()
	{
		delete m_pDataCopy;
	}

	COMPARE_RESULT CompareMember(DATA* leftData,DATA* rightData)
	{
		return m_dataComparator.CompareBy(m_dataMember,leftData,rightData);
	}
	void CopyMember(DATA* copyTo,DATA* copyFrom)
	{
		m_pDataCopy->DoCopy(copyTo,copyFrom);
	}

protected:
	MEMBERS_ENUM m_dataMember;
private:
	CDataCopy<DATA> *m_pDataCopy;
	DATACOMPARATOR m_dataComparator;
};


#endif