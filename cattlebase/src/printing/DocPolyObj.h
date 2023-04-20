#ifndef __DOCPOLYOBJ_H__
#define __DOCPOLYOBJ_H__

#include "DocObj.h"

class CDocPolyObj : public CDocObj
{
protected:
	DECLARE_SERIAL(CDocPolyObj);
	CDocPolyObj();
public:
	CDocPolyObj(const CRect& position);
	virtual ~CDocPolyObj();
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);

protected:
	int m_nPoints;
	int m_nAllocPoints;
	CPoint* m_points;
	CDocPolyObj* m_pDrawObj;
};


class CDrawPoly : public CDocPolyObj
{
public:
	DECLARE_SERIAL(CDrawPoly);
	CDrawPoly() {};
};

#endif 
