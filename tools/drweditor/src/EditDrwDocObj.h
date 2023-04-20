#ifndef __EDITDRWDOCOBJ_H__
#define __EDITDRWDOCOBJ_H__

#include "../../src/printing/DocOleObj.h"
#include "DrwDocView.h"

template<class BASEDOCOBJCLASS>
class CEditDocOleObj : public BASEDOCOBJCLASS
{	
public:
	virtual void MoveTo(const CRect& positon, CDrwDocView* pView = NULL)
	{
		ASSERT_VALID(this);

		if (position == m_position)
			return;

		if (pView == NULL)
		{
			Invalidate();
			m_position = position;
			Invalidate();
		}
		else
		{
			pView->InvalObj(this);
			m_position = position;
			pView->InvalObj(this);
		}
		m_pDocument->SetModifiedFlag();
	}
	
};

#endif 
