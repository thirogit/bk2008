#include "stdafx.h"
#include "DocRectObj.h"

IMPLEMENT_SERIAL(CDrawRect, CDocRectObj, 0)

IMPLEMENT_SERIAL(CDocRectObj, CDocObj, 0)

CDocRectObj::CDocRectObj() : m_nShape(rectangle),m_roundness(0,0)
{
}

CDocRectObj::CDocRectObj(const CRect& position)	: CDocObj(position),m_nShape(rectangle),m_roundness(0,0)
{
	ASSERT_VALID(this);
	
}

void CDocRectObj::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CDocObj::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << (WORD) m_nShape;
		ar << m_roundness;
	}
	else
	{
		WORD wTemp;
		ar >> wTemp; m_nShape = (Shape)wTemp;
		ar >> m_roundness;
	}
}

void CDocRectObj::Draw(CDC* pDC)
{
	ASSERT_VALID(this);

	CBrush brush;
	if (!brush.CreateBrushIndirect(&m_logbrush))
		return;
	CPen pen;
	if (!pen.CreatePenIndirect(&m_logpen))
		return;

	CBrush* pOldBrush;
	CPen* pOldPen;

	if (m_bBrush)
		pOldBrush = pDC->SelectObject(&brush);
	else
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	if (m_bPen)
		pOldPen = pDC->SelectObject(&pen);
	else
		pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

	CRect rect = GetPosition();
	switch (m_nShape)
	{
	case rectangle:
		pDC->Rectangle(rect);
		break;

	case roundRectangle:
		pDC->RoundRect(rect, m_roundness);
		break;

	case ellipse:
		pDC->Ellipse(rect);
		break;

	case line:
		if (rect.top > rect.bottom)
		{
			rect.top -= m_logpen.lopnWidth.y / 2;
			rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
		}
		else
		{
			rect.top += (m_logpen.lopnWidth.y + 1) / 2;
			rect.bottom -= m_logpen.lopnWidth.y / 2;
		}

		if (rect.left > rect.right)
		{
			rect.left -= m_logpen.lopnWidth.x / 2;
			rect.right += (m_logpen.lopnWidth.x + 1) / 2;
		}
		else
		{
			rect.left += (m_logpen.lopnWidth.x + 1) / 2;
			rect.right -= m_logpen.lopnWidth.x / 2;
		}

		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.BottomRight());
		break;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}
