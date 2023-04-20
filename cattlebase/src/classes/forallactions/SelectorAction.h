#ifndef __SELECTORACTION_H__
#define __SELECTORACTION_H__

template<class ARRAY,class DATA,class MATCHER>
class CSelectorAction : public ARRAY::ArrayAction
{
public:
	CSelectorAction(ARRAY& selectArray,MATCHER *pMatcher) : m_selectArray(selectArray),
		m_pMatcher(pMatcher)
	{
	}

	virtual bool operator()(DATA* pElement)
	{	
		if(m_pMatcher->Matches(pElement))
			m_selectArray.Add(pElement);
		return true;	
	}
private:
	ARRAY & m_selectArray;
	MATCHER *m_pMatcher;

};







#endif


	
	





