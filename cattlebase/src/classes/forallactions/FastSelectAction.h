#ifndef __FASTSELECTACTION_H__
#define __FASTSELECTACTION_H__

enum FASTSELECT_FLAGS
{
	FASTSELECT_NOOPTION = 0x00,
	FASTSELECT_SELROW = 0x01,
	FASTSELECT_MARKROW = 0x02,
	FASTSELECT_FINDFIRST = 0x04,
	FASTSELECT_POSONROW = 0x08
};

template<class ACTION,class MATCHER,class DATA>
class CFastSelectAction : public ACTION
{
public:
	
	CFastSelectAction(MATCHER *pMatcher,BaseDataGrid *pGrid,UINT flags) :  
		m_pMatcher(pMatcher),m_flags(flags),m_pGrid(pGrid) 
		{
			if(m_flags == FASTSELECT_NOOPTION)
				m_flags = FASTSELECT_MARKROW;
			ResetDoneFirstPositioning();
		};
	virtual bool operator()(DATA* pElement)
	{	
		if(m_pMatcher->Matches(pElement))
		{
		
			if(m_flags & FASTSELECT_SELROW)	m_pGrid->SelectRow(GetCurrentIndex());
			if(m_flags & FASTSELECT_MARKROW) m_pGrid->MarkRow(GetCurrentIndex());
		
			if(m_flags & FASTSELECT_POSONROW && !m_doneFirstPositioning)
			{
				m_pGrid->SetCurPos(GetCurrentIndex());
				m_doneFirstPositioning = true;
			}
			return !(m_flags & FASTSELECT_FINDFIRST);
		}
		return true;	
	}

	void ResetDoneFirstPositioning()
	{
		m_doneFirstPositioning = false;
	}
		
private:
	UINT m_flags;
	MATCHER *m_pMatcher;
	BaseDataGrid *m_pGrid;
	bool m_doneFirstPositioning;
	
};



#endif


	
	





