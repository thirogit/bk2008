#ifndef _MATCHPTR_H_
#define _MATCHPTR_H_


template<class TYPE,class MATCHER,class PTR>
class MatchPtr : public Match<TYPE>
{
public:
	MatchPtr(MATCHER *pMatcher,const PTR* ptr,bool bAcceptNull,
		PTR* (MATCHER::*getPtr)(TYPE *pData)) :
	  m_pMatcher(pMatcher),m_getPtr(getPtr),m_Ptr(ptr),
	  m_bAcceptNull(bAcceptNull)
	  {
	  }


	virtual bool DoMatch(TYPE *pData)
	{
		PTR* ptr = (m_pMatcher->*m_getPtr)(pData);
		return (ptr != NULL || m_bAcceptNull) && (ptr == m_Ptr);		
	}
private:
	const PTR * m_Ptr;
	bool m_bAcceptNull;

	PTR* (MATCHER::*m_getPtr)(TYPE *pData);

	MATCHER *m_pMatcher;

};

#endif


	
	





