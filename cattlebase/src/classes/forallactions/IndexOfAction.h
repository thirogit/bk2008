#ifndef __INDEXOFACTION_H__
#define __INDEXOFACTION_H__


template<class DATATYPE>
class CIndexOfAction : public PtrFlushArray<DATATYPE>::ArrayAction
{
public:
	CIndexOfAction(DATATYPE *pPtrToFind) : m_Index(-1),m_pPtrToFind(pPtrToFind) 
	{
		ASSERT(pPtrToFind);
	};
	virtual bool operator()(DATATYPE* pElement)
	{	
		if(m_pPtrToFind != NULL && m_pPtrToFind == pElement)
		{
			m_Index = GetCurrentIndex();
			return false;
		}
		return true;
	}

	int GetIndex() const
	{
		return m_Index;
	}
private:
	int m_Index;
	DATATYPE* m_pPtrToFind;
};



#endif


	
	





