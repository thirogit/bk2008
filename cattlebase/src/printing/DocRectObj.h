#ifndef __DOCRECTOBJ_H__
#define __DOCRECTOBJ_H__

#include "DocObj.h"

class CDocRectObj : public CDocObj
{
protected:
	DECLARE_SERIAL(CDocRectObj);
	CDocRectObj();

public:
	enum Shape { rectangle, roundRectangle, ellipse, line };
	CDocRectObj(const CRect& position);
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);
protected:	
	CPoint m_roundness; 
	Shape m_nShape;
};


class CDrawRect : public CDocRectObj
{
public:
	DECLARE_SERIAL(CDrawRect);
	CDrawRect() {};
};



#endif 
