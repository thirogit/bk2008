#ifndef __DATADIFFMERGE_H__
#define __DATADIFFMERGE_H__

#include "MemberDiff.h"

class CBaseDataDiffMerge
{
public:
	CBaseDataDiffMerge(UINT nResStrMemName) : m_doCopy(false)
	{
		m_memberName.LoadString(nResStrMemName);
	}
	const CString& GetMemberName()
	{
		return m_memberName;
	}
	void	SetDoCopy(bool bDoCopy)
	{
		m_doCopy = bDoCopy;
	}
	bool GetDoCopy()
	{
		return m_doCopy;
	}
	virtual bool	DoCopyIfSet() = 0;
	virtual CString GetMemeber_To_TextValue() = 0;
	virtual CString GetMemeber_From_TextValue() = 0;
	virtual bool AreDiffrent() = 0;

	virtual ~CBaseDataDiffMerge() 
	{
	}
protected:
	CString m_memberName;
	bool	m_doCopy;	
};



template<class DATA,class MEMBERS_ENUM,class DATACOMPARATOR,class TEXTPROVIDER>
class CDataDiffMerge : public CBaseDataDiffMerge,public CMemberDiff<DATA,MEMBERS_ENUM,DATACOMPARATOR>
{
public:
	CDataDiffMerge(MEMBERS_ENUM dataMember,
				   UINT nResStrMemName,
				   CDataCopy<DATA> *pDataCopy,
				   DATA* pToData,
				   DATA* pFromData) : CBaseDataDiffMerge(nResStrMemName),
									  CMemberDiff<DATA,MEMBERS_ENUM,DATACOMPARATOR>(dataMember,pDataCopy),
									  m_pToData(pToData),
									  m_pFromData(pFromData)
	{}

	virtual ~CDataDiffMerge() 
	{
	}

	bool	DoCopyIfSet()
	{
		if(m_doCopy)
			CopyMember(m_pToData,m_pFromData);
		return m_doCopy;
	}
	CString GetMemeber_To_TextValue()
	{
		return m_textProvider.GetDataText(m_pToData,m_dataMember);
	}
	CString GetMemeber_From_TextValue()
	{
		return m_textProvider.GetDataText(m_pFromData,m_dataMember);
	}
	bool AreDiffrent()
	{
		return CompareMember(m_pToData,m_pFromData) != COMPARE_EQUAL;
	}
protected:
	TEXTPROVIDER m_textProvider;
	DATA* m_pToData;
	DATA* m_pFromData;
};

#endif