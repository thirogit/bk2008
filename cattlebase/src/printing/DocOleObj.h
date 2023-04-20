#ifndef __DOCOLEOBJ_H__
#define __DOCOLEOBJ_H__

#include "DocObj.h"
#include "DocOleItem.h"

class CDocOleObj : public CDocObj
{
protected:
	DECLARE_SERIAL(CDocOleObj);
	CDocOleObj();
public:
	CDocOleObj(const CRect& position);
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);
	virtual ~CDocOleObj();
	const CSize& GetExtent();

	static BOOL c_bShowItems;
protected:
	CDocOleItem* m_pClientItem;
	CSize m_extent; // current extent is tracked separate from scaled position
};

#endif 
