// drawobj.cpp - implementation for drawing objects
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#ifndef FOR_CATTLEBASE
#include "drawcli.h"
#endif
#include "drawdoc.h"
#include "drawvw.h"
#include "drawobj.h"
#include "cntritem.h"

#ifndef FOR_CATTLEBASE
#include "rectdlg.h"
#include "textdlg.h"
#include "posdlg.h"
#include "figpropdlg.h"
#include "resource.h"
#include "ImgOpenDlg.h"
#endif


#define MAX_TEXT 255
#define MAX_LABEL 32

IMPLEMENT_SERIAL(CDrawObj, CObject, 0)

CDrawObj::CDrawObj() : m_group(NULL)
{
}

CDrawObj::~CDrawObj()
{
}

CDrawObj::CDrawObj(const CRect& position)
{
	m_position = position;
	m_pDocument = NULL;
	m_group = NULL;

	m_bPen = TRUE;
	m_logpen.lopnStyle = PS_INSIDEFRAME;
	m_logpen.lopnWidth.x = 1;
	m_logpen.lopnWidth.y = 1;
	m_logpen.lopnColor = RGB(0, 0, 0);

	m_bBrush = TRUE;
	m_logbrush.lbStyle = BS_SOLID;
	m_logbrush.lbColor = RGB(192, 192, 192);
	m_logbrush.lbHatch = HS_HORIZONTAL;
}

void CDrawObj::Serialize(CArchive& ar)
{
	char label[MAX_LABEL+1];
	memset(label,0,sizeof(label));
	int grpID ;
	POSITION pos;
	CGroupList *grpList;
	CDrawObjGroup *pGrp;

	CObject::Serialize(ar);
	if (ar.IsStoring())
	{

		_snprintf(label,MAX_LABEL,"%s",(LPCSTR)m_label);

		ar.Write(label,sizeof(label));
		ar << m_position;
		ar << (WORD)m_bPen;
		ar.Write(&m_logpen, sizeof(LOGPEN));
		ar << (WORD)m_bBrush;
		ar.Write(&m_logbrush, sizeof(LOGBRUSH));
		if(m_group)
			grpID = m_group->m_groupID;
		else grpID = 0;
		ar << grpID;
	
	}
	else
	{
		// get the document back pointer from the archive
		m_pDocument = (CDrawDoc*)ar.m_pDocument;
		ASSERT_VALID(m_pDocument);
		ASSERT_KINDOF(CDrawDoc, m_pDocument);

		WORD wTemp;
		ar.Read(label,sizeof(label));
		m_label = label;
		ar >> m_position;
		ar >> wTemp; m_bPen = (BOOL)wTemp;
		ar.Read(&m_logpen,sizeof(LOGPEN));
		ar >> wTemp; m_bBrush = (BOOL)wTemp;
		ar.Read(&m_logbrush, sizeof(LOGBRUSH));

#ifndef READ_OLD_DOC
		ar >> grpID;

		if(grpID)
		{
			grpList = m_pDocument->GetGroupList();
			pos = grpList->GetHeadPosition();
			while(pos)
			{
				pGrp = grpList->GetNext(pos);
				if(grpID == pGrp->m_groupID)
				{
					m_group = pGrp;
					break;
				}
			}
		}
#endif

		
	}
}

#ifndef FOR_CATTLEBASE
void CDrawObj::SetLineColor(COLORREF color)
{
	ASSERT_VALID(this);

	m_logpen.lopnColor = color;
	Invalidate();
	m_pDocument->SetModifiedFlag();
}

void CDrawObj::SetFillColor(COLORREF color)
{
	ASSERT_VALID(this);

	m_logbrush.lbColor = color;
	Invalidate();
	m_pDocument->SetModifiedFlag();
}

COLORREF CDrawObj::GetLineColor()
{
	return m_logpen.lopnColor;
}

COLORREF CDrawObj::GetFillColor()
{
	return m_logbrush.lbColor;
}

BOOL CDrawObj::IsNoFill()
{
	return !m_bBrush;
}

BOOL CDrawObj::IsNoOutline()
{
	return !m_bPen;
}

BOOL CDrawObj::SetNoFill( BOOL bSet /*= TRUE*/ )
{
BOOL bPrev = !m_bBrush;
	m_bBrush = !bSet;
	return bPrev;
}


BOOL CDrawObj::SetLogPenSize(int nSize)
{
	if(m_bPen)
	{
		m_logpen.lopnWidth.x = nSize;
		m_logpen.lopnWidth.y = nSize;
	}
	return m_bPen;
}

BOOL CDrawObj::SetNoOutline( BOOL bSet /*= TRUE*/ )
{
BOOL bPrev = !m_bPen;
	m_bPen = !bSet;
	if( bSet )
	{
		m_logpen.lopnWidth.x = 0;
		m_logpen.lopnWidth.y = 0;
	}
	else
	{
		if( m_logpen.lopnWidth.x == 0
			|| m_logpen.lopnWidth.y == 0
			)
			m_logpen.lopnWidth.x
				= m_logpen.lopnWidth.y
				= 1;
 	}
	return bPrev;
}

void CDrawObj::Remove()
{
	delete this;
}
#endif

void CDrawObj::Draw(CDC*)
{
}

void CDrawObj::NormalizePosition()
{
	NormalizePosition(m_position);
}
void CDrawObj::NormalizePosition(CRect &rect)
{
	if(rect.left > rect.right) rect.SwapLeftRight();
	if(rect.top < rect.bottom)
	{
		int iTemp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = iTemp;
	}
}

#ifndef FOR_CATTLEBASE

void CDrawObj::DrawTracker(CDC* pDC, TrackerState state)
{
	ASSERT_VALID(this);

	switch (state)
	{
	case normal:
		break;

	case selected:
	case active:
		{
			int nHandleCount = GetHandleCount();
			for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
			{
				CPoint handle = GetHandle(nHandle);
				pDC->PatBlt(handle.x - 3, handle.y - 3, 7, 7, DSTINVERT);
			}
		}
		break;
	}
}

// position is in logical
void CDrawObj::MoveTo(const CRect& position, CDrawView* pView)
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

// Note: if bSelected, hit-codes start at one for the top-left
// and increment clockwise, 0 means no hit.
// If !bSelected, 0 = no hit, 1 = hit (anywhere)

// point is in logical coordinates
int CDrawObj::HitTest(CPoint point, CDrawView* pView, BOOL bSelected)
{
	ASSERT_VALID(this);
	ASSERT(pView != NULL);

	if (bSelected)
	{
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in logical coords
			CRect rc = GetHandleRect(nHandle,pView);
			if (point.x >= rc.left && point.x < rc.right &&
				point.y <= rc.top && point.y > rc.bottom)
				return nHandle;
		}
	}
	else
	{
		if (point.x >= m_position.left && point.x < m_position.right &&
			point.y <= m_position.top && point.y > m_position.bottom)
			return 1;
	}
	return 0;
}

// rect must be in logical coordinates
BOOL CDrawObj::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	CRect fixed = m_position;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & fixed).IsRectEmpty();
}

int CDrawObj::GetHandleCount()
{
	ASSERT_VALID(this);
	return 8;
}

// returns logical coords of center of handle
CPoint CDrawObj::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
	int x, y, xCenter, yCenter;

	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = m_position.left + m_position.Width() / 2;
	yCenter = m_position.top + m_position.Height() / 2;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = m_position.left;
		y = m_position.top;
		break;

	case 2:
		x = xCenter;
		y = m_position.top;
		break;

	case 3:
		x = m_position.right;
		y = m_position.top;
		break;

	case 4:
		x = m_position.right;
		y = yCenter;
		break;

	case 5:
		x = m_position.right;
		y = m_position.bottom;
		break;

	case 6:
		x = xCenter;
		y = m_position.bottom;
		break;

	case 7:
		x = m_position.left;
		y = m_position.bottom;
		break;

	case 8:
		x = m_position.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}

// return rectange of handle in logical coords
CRect CDrawObj::GetHandleRect(int nHandleID, CDrawView* pView)
{
	ASSERT_VALID(this);
	ASSERT(pView != NULL);

	CRect rect;
	// get the center of the handle in logical coords
	CPoint point = GetHandle(nHandleID);
	// convert to client/device coords
	pView->DocToClient(point);
	// return CRect of handle in device coords
	rect.SetRect(point.x-3, point.y-3, point.x+3, point.y+3);
	pView->ClientToDoc(rect);

	return rect;
}

HCURSOR CDrawObj::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

// point must be in logical
void CDrawObj::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	ASSERT_VALID(this);

	CRect position = m_position;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		position.left = point.x;
		position.top = point.y;
		break;

	case 2:
		position.top = point.y;
		break;

	case 3:
		position.right = point.x;
		position.top = point.y;
		break;

	case 4:
		position.right = point.x;
		break;

	case 5:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 6:
		position.bottom = point.y;
		break;

	case 7:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 8:
		position.left = point.x;
		break;
	}

	MoveTo(position, pView);
}

void CDrawObj::Invalidate()
{
	ASSERT_VALID(this);
	m_pDocument->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, this);
}

CDrawObj* CDrawObj::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawObj* pClone = new CDrawObj();
	CopyObjTo(pClone);
	ASSERT_VALID(pClone);
	if (pDoc != NULL) pDoc->Add(pClone);

	return pClone;
}

void CDrawObj::CopyObjTo(CDrawObj *pObj)
{
	pObj->m_position = m_position;
	pObj->m_bPen = m_bPen;
	pObj->m_logpen = m_logpen;
	pObj->m_bBrush = m_bBrush;
	pObj->m_logbrush = m_logbrush;
	pObj->m_group = m_group;	
	pObj->m_label = m_label;
}

void CDrawObj::OnEditProperties(CPropertySheet &propSheet)
{
	ASSERT_VALID(this);
	
	CRectDlg *rectdlg = new CRectDlg(IsNoFill(),
		         IsNoOutline() ? 0 : m_logpen.lopnWidth.x,
				 IsNoFill() ? CRectDlg::g_clrTransparent : GetFillColor(),
				 IsNoOutline() ? CRectDlg::g_clrTransparent : GetLineColor(),
				 m_label);

	CPosDlg *posdlg = new CPosDlg(m_position);

	propSheet.AddPage(rectdlg);
	propSheet.AddPage(posdlg);

}



//void CDrawObj::OnOpen(CDrawView* pView )
//{
//	OnEditProperties(pView);
//}
#endif

bool CDrawObj::IsVisible()
{
	if(m_group)
		return m_group->m_PrintVis;
	return true;
}

#ifdef _DEBUG
void CDrawObj::AssertValid()
{
	ASSERT(m_position.left <= m_position.right);
	ASSERT(m_position.bottom <= m_position.top);
}
#endif

////////////////////////////////////////////////////////////////////////////
// CDrawRect

IMPLEMENT_SERIAL(CDrawRect, CDrawObj, 0)

CDrawRect::CDrawRect() : m_nShape(rectangle),m_roundness(0,0)
{
}

CDrawRect::CDrawRect(const CRect& position)
	: CDrawObj(position),m_nShape(rectangle),m_roundness(0,0)
{
	ASSERT_VALID(this);
	
}

void CDrawRect::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CDrawObj::Serialize(ar);
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

void CDrawRect::Draw(CDC* pDC)
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

	CRect rect = m_position;
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

#ifndef FOR_CATTLEBASE
void CDrawRect::OnEditProperties(CPropertySheet &propSheet)
{
	CDrawObj::OnEditProperties(propSheet);
	CFigPropDlg *figprop = new CFigPropDlg(this);
	propSheet.AddPage(figprop);
}

int CDrawRect::GetHandleCount()
{
	ASSERT_VALID(this);

	return m_nShape == line ? 2 :
		CDrawObj::GetHandleCount()/* + (m_nShape == roundRectangle)*/;
}

// returns center of handle in logical coordinates
CPoint CDrawRect::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	if (m_nShape == line && nHandle == 2)
		nHandle = 5;
	/*else if (m_nShape == roundRectangle && nHandle == 9)
	{
		CRect rect = m_position;
		rect.NormalizeRect();
		CPoint point = rect.BottomRight();
		point.x -= m_roundness.x / 2;
		point.y -= m_roundness.y / 2;
		return point;
	}*/

	return CDrawObj::GetHandle(nHandle);
}

HCURSOR CDrawRect::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	if (m_nShape == line && nHandle == 2)
		nHandle = 5;
	else if (m_nShape == roundRectangle && nHandle == 9)
		return AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	return CDrawObj::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawRect::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	ASSERT_VALID(this);

	if (m_nShape == line && nHandle == 2)
		nHandle = 5;
	else if (m_nShape == roundRectangle && nHandle == 9)
	{
		CRect rect = m_position;
		rect.NormalizeRect();
		if (point.x > rect.right - 1)
			point.x = rect.right - 1;
		else if (point.x < rect.left + rect.Width() / 2)
			point.x = rect.left + rect.Width() / 2;
		if (point.y > rect.bottom - 1)
			point.y = rect.bottom - 1;
		else if (point.y < rect.top + rect.Height() / 2)
			point.y = rect.top + rect.Height() / 2;

		//m_roundness.x = 2 * (rect.right - point.x);
		//m_roundness.y = 2 * (rect.bottom - point.y);

		m_pDocument->SetModifiedFlag();
		if (pView == NULL)
			Invalidate();
		else
			pView->InvalObj(this);
		return;
	}

	CDrawObj::MoveHandleTo(nHandle, point, pView);
}

// rect must be in logical coordinates
BOOL CDrawRect::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	bool bOneDim = false;

	CRect rectT = rect;
	rectT.NormalizeRect();


	CRect fixed = m_position;
	fixed.NormalizeRect();
	


	if(fixed.left == fixed.right || fixed.top == fixed.bottom)
	{
		if(!rectT.PtInRect(fixed.BottomRight()) &&
		   !rectT.PtInRect(fixed.TopLeft())	) return FALSE;
		bOneDim = true;
	}
	else if ((rectT & fixed).IsRectEmpty())
		return FALSE;

	CRgn rgn;
	switch (m_nShape)
	{
	case rectangle:
		return TRUE;

	case roundRectangle:
		rgn.CreateRoundRectRgn(fixed.left, fixed.top, fixed.right, fixed.bottom,
			m_roundness.x, m_roundness.y);
		break;

	case ellipse:
		rgn.CreateEllipticRgnIndirect(fixed);
		break;

	case line:
		{
			int x = (m_logpen.lopnWidth.x + 5) / 2;
			int y = (m_logpen.lopnWidth.y + 5) / 2;
			POINT points[4];
			points[0].x = fixed.left;
			points[0].y = fixed.top;
			points[1].x = fixed.left;
			points[1].y = fixed.top;
			points[2].x = fixed.right;
			points[2].y = fixed.bottom;
			points[3].x = fixed.right;
			points[3].y = fixed.bottom;

			if (fixed.left < fixed.right)
			{
				points[0].x -= x;
				points[1].x += x;
				points[2].x += x;
				points[3].x -= x;
			}
			else
			{
				points[0].x += x;
				points[1].x -= x;
				points[2].x -= x;
				points[3].x += x;
			}

			if (fixed.top < fixed.bottom)
			{
				points[0].y -= y;
				points[1].y += y;
				points[2].y += y;
				points[3].y -= y;
			}
			else
			{
				points[0].y += y;
				points[1].y -= y;
				points[2].y -= y;
				points[3].y += y;
			}
			rgn.CreatePolygonRgn(points, 4, ALTERNATE);
		}
		break;
	}
	return rgn.RectInRegion(fixed) || bOneDim;
}

CDrawObj* CDrawRect::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawRect* pClone = new CDrawRect();
	ASSERT_VALID(pClone);
	if (pDoc != NULL)	pDoc->Add(pClone);
	ASSERT_VALID(pClone);
	return pClone;
}

void CDrawRect::CopyObjTo(CDrawRect *pObj)
{
	CDrawObj::CopyObjTo(pObj);
	pObj->m_nShape = m_nShape;
	pObj->m_roundness = m_roundness;
}
#endif
////////////////////////////////////////////////////////////////////////////
// CDrawPoly

IMPLEMENT_SERIAL(CDrawPoly, CDrawObj, 0)

CDrawPoly::CDrawPoly()
{
	m_points = NULL;
	m_nPoints = 0;
	m_nAllocPoints = 0;
}

CDrawPoly::CDrawPoly(const CRect& position)
	: CDrawObj(position)
{
	m_points = NULL;
	m_nPoints = 0;
	m_nAllocPoints = 0;
	m_bPen = TRUE;
	m_bBrush = FALSE;
}

CDrawPoly::~CDrawPoly()
{
	if (m_points != NULL)
		delete[] m_points;
}

void CDrawPoly::Serialize( CArchive& ar )
{
	int i;
	CDrawObj::Serialize( ar );
	if( ar.IsStoring() )
	{
		ar << (WORD) m_nPoints;
		ar << (WORD) m_nAllocPoints;
		for (i = 0;i< m_nPoints; i++)
			ar << m_points[i];
	}
	else
	{
		WORD wTemp;
		ar >> wTemp; m_nPoints = wTemp;
		ar >> wTemp; m_nAllocPoints = wTemp;
		m_points = new CPoint[m_nAllocPoints];
		for (i = 0;i < m_nPoints; i++)
			ar >> m_points[i];
	}
}

void CDrawPoly::Draw(CDC* pDC)
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

	pDC->Polygon(m_points, m_nPoints);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}
#ifndef FOR_CATTLEBASE

// position must be in logical coordinates
void CDrawPoly::MoveTo(const CRect& position, CDrawView* pView)
{
	ASSERT_VALID(this);
	if (position == m_position)
		return;

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);

	for (int i = 0; i < m_nPoints; i += 1)
	{
		m_points[i].x += position.left - m_position.left;
		m_points[i].y += position.top - m_position.top;
	}

	m_position = position;

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);
	m_pDocument->SetModifiedFlag();
}

int CDrawPoly::GetHandleCount()
{
	return m_nPoints;
}

CPoint CDrawPoly::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	ASSERT(nHandle >= 1 && nHandle <= m_nPoints);
	return m_points[nHandle - 1];
}

HCURSOR CDrawPoly::GetHandleCursor(int )
{
	return AfxGetApp()->LoadStandardCursor(IDC_ARROW);
}

// point is in logical coordinates
void CDrawPoly::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	ASSERT_VALID(this);
	ASSERT(nHandle >= 1 && nHandle <= m_nPoints);
	if (m_points[nHandle - 1] == point)
		return;

	m_points[nHandle - 1] = point;
	RecalcBounds(pView);

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);
	m_pDocument->SetModifiedFlag();
}

// rect must be in logical coordinates
BOOL CDrawPoly::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);
	CRgn rgn;
	rgn.CreatePolygonRgn(m_points, m_nPoints, ALTERNATE);
	return rgn.RectInRegion(rect);
}

CDrawObj* CDrawPoly::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawPoly* pClone = new CDrawPoly();

	CopyObjTo(pClone);
	ASSERT_VALID(pClone);

	if (pDoc != NULL) pDoc->Add(pClone);

	ASSERT_VALID(pClone);
	return pClone;
}

void CDrawPoly::CopyObjTo(CDrawPoly *pObj)
{
	CDrawObj::CopyObjTo(pObj);
	pObj->m_points = new CPoint[m_nAllocPoints];
	memcpy(pObj->m_points, m_points, sizeof(CPoint) * m_nPoints);
	pObj->m_nAllocPoints = m_nAllocPoints;
	pObj->m_nPoints = m_nPoints;
}

// point is in logical coordinates
void CDrawPoly::AddPoint(const CPoint& point, CDrawView* pView)
{
	ASSERT_VALID(this);
	if (m_nPoints == m_nAllocPoints)
	{
		CPoint* newPoints = new CPoint[m_nAllocPoints + 10];
		if (m_points != NULL)
		{
			memcpy(newPoints, m_points, sizeof(CPoint) * m_nAllocPoints);
			delete[] m_points;
		}
		m_points = newPoints;
		m_nAllocPoints += 10;
	}

	if (m_nPoints == 0 || m_points[m_nPoints - 1] != point)
	{
		m_points[m_nPoints++] = point;
		if (!RecalcBounds(pView))
		{
			if (pView == NULL)
				Invalidate();
			else
				pView->InvalObj(this);
		}
		m_pDocument->SetModifiedFlag();
	}
}

BOOL CDrawPoly::RecalcBounds(CDrawView* pView)
{
	ASSERT_VALID(this);

	if (m_nPoints == 0)
		return FALSE;

	CRect bounds(m_points[0], CSize(0, 0));
	for (int i = 1; i < m_nPoints; ++i)
	{
		if (m_points[i].x < bounds.left)
			bounds.left = m_points[i].x;
		if (m_points[i].x > bounds.right)
			bounds.right = m_points[i].x;
		if (m_points[i].y < bounds.top)
			bounds.top = m_points[i].y;
		if (m_points[i].y > bounds.bottom)
			bounds.bottom = m_points[i].y;
	}

	if (bounds == m_position)
		return FALSE;

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);

	m_position = bounds;

	if (pView == NULL)
		Invalidate();
	else
		pView->InvalObj(this);

	return TRUE;
}
#endif
////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDrawOleObj, CDrawObj, 0)

BOOL CDrawOleObj::c_bShowItems = TRUE;

CDrawOleObj::CDrawOleObj() : m_extent(0,0)
{
	m_pClientItem = NULL;
}

CDrawOleObj::CDrawOleObj(const CRect& position)
	: CDrawObj(position), m_extent(0, 0)
{
	m_pClientItem = NULL;
}

CDrawOleObj::~CDrawOleObj()
{
	if (m_pClientItem != NULL)
	{
		m_pClientItem->Release();
		m_pClientItem = NULL;
	}
}


#ifndef FOR_CATTLEBASE
void CDrawOleObj::Remove()
{
	if (m_pClientItem != NULL)
	{
		m_pClientItem->Delete();
		m_pClientItem = NULL;
	}
	CDrawObj::Remove();
}
#endif

void CDrawOleObj::Serialize( CArchive& ar )
{
	ASSERT_VALID(this);

	CDrawObj::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_extent;
		ar << m_pClientItem;
	}
	else
	{
		ar >> m_extent;
		ar >> m_pClientItem;
		m_pClientItem->m_pDrawObj = this;
	}
}



#ifndef FOR_CATTLEBASE
CDrawObj* CDrawOleObj::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	AfxGetApp()->BeginWaitCursor();

	CDrawOleObj* pClone = NULL;
	CDrawItem* pItem = NULL;
	TRY
	{
		// perform a "deep copy" -- need to copy CDrawOleObj and the CDrawItem
		//  that it points to.
		CDrawOleObj* pClone = new CDrawOleObj(m_position);

		CDrawItem* pItem = new CDrawItem(m_pDocument, pClone);
		if (!pItem->CreateCloneFrom(m_pClientItem))	AfxThrowMemoryException();

		pClone->m_pClientItem = pItem;

		CopyObjTo(pClone);

		ASSERT_VALID(pClone);

		if (pDoc != NULL)	pDoc->Add(pClone);
	}
	CATCH_ALL(e)
	{
		pItem->Delete();
		pClone->m_pClientItem = NULL;
		pClone->Remove();
		AfxGetApp()->EndWaitCursor();

		THROW_LAST();
	}
	END_CATCH_ALL

	AfxGetApp()->EndWaitCursor();
	return pClone;
}
#endif

void CDrawOleObj::Draw(CDC* pDC)
{
	ASSERT_VALID(this);

	CDrawItem* pItem = m_pClientItem;
	if (pItem != NULL)
	{
		// draw the OLE item itself
		pItem->Draw(pDC, m_position);

		// don't draw tracker in print preview or on printer
		if (!pDC->IsPrinting())
		{
			// use a CRectTracker to draw the standard effects
			CRectTracker tracker;
			tracker.m_rect = m_position;
			pDC->LPtoDP(tracker.m_rect);

			if (c_bShowItems)
			{
				// put correct border depending on item type
				if (pItem->GetType() == OT_LINK)
					tracker.m_nStyle |= CRectTracker::dottedLine;
				else
					tracker.m_nStyle |= CRectTracker::solidLine;
			}

			// put hatching over the item if it is currently open
			if (pItem->GetItemState() == COleClientItem::openState ||
				pItem->GetItemState() == COleClientItem::activeUIState)
			{
				tracker.m_nStyle |= CRectTracker::hatchInside;
			}
			tracker.Draw(pDC);
		}
	}
}


#ifndef FOR_CATTLEBASE
void CDrawOleObj::OnOpen(CDrawView* pView)
{
	AfxGetApp()->BeginWaitCursor();
	m_pClientItem->DoVerb(
		GetKeyState(VK_CONTROL) < 0 ? OLEIVERB_OPEN : OLEIVERB_PRIMARY,
		pView);
	AfxGetApp()->EndWaitCursor();
}

void CDrawOleObj::OnEditProperties()
{
	// using COlePropertiesDialog directly means no scaling
	COlePropertiesDialog dlg(m_pClientItem, 100, 100, NULL);

	dlg.DoModal();
}

// position is in logical
void CDrawOleObj::MoveTo(const CRect& position, CDrawView* pView)
{
	ASSERT_VALID(this);

	if (position == m_position)
		return;

	// call base class to update position
	CDrawObj::MoveTo(position, pView);

	// update position of in-place editing session on position change
	if (m_pClientItem->IsInPlaceActive())
		m_pClientItem->SetItemRects();
}
#endif
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// CDrawText



IMPLEMENT_SERIAL(CDrawText, CDrawRect, 0)

CDrawText::CDrawText() : CDrawRect(),m_bOwnStyle(TRUE),m_OwnStyle(),m_pStyle(NULL)
{
#ifndef FOR_CATTLEBASE
	m_text.LoadString(IDS_DEFAULTTEXT);
#endif
}

CDrawText::CDrawText(const CRect& position)
	: CDrawRect(position),m_bOwnStyle(TRUE),m_OwnStyle(),m_pStyle(NULL)
{
#ifndef FOR_CATTLEBASE
	m_text.LoadString(IDS_DEFAULTTEXT);
#endif
}

CDrawText::CDrawText(const CRect& position, CString& text, CTextStyle &pStyle)
	: CDrawRect(position), m_text(text),m_OwnStyle(pStyle),m_bOwnStyle(TRUE),m_pStyle(NULL)
{
	m_OwnStyle.m_styleid = 0;
	m_OwnStyle.m_stylename = "";
}


CDrawText::~CDrawText() 
{
	
}

void CDrawText::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	int styleid;
	char text[MAX_TEXT+1];
	memset(text,0,sizeof(text));
	
	CDrawRect::Serialize(ar);
	
	if (ar.IsStoring())
	{
	
		m_OwnStyle.Serialize(ar);
		ar << m_bOwnStyle;
		styleid = (m_pStyle ? m_pStyle->m_styleid : 0);  
		ar << styleid;
		
		_snprintf(text,MAX_TEXT,"%s",(LPCSTR)m_text);
		ar.Write(text,sizeof(text));
	
	}
	else
	{
#if defined(READ_OLD_DOC)
		m_bOwnStyle = TRUE;
		m_pStyle = NULL;
		m_OwnStyle.m_halign = CENTER;
		m_OwnStyle.m_valign = VCENTER;
		ar.Read(&m_OwnStyle.m_lf,sizeof(LOGFONT));
		ar >> m_OwnStyle.m_txtclr;

#else
		m_OwnStyle.Serialize(ar);
		ar >> m_bOwnStyle;
		ar >> styleid;
		m_pStyle = NULL;
		if(styleid)
		{
			CTextStyle *pStyle;
			CStyleList *Styles = m_pDocument->GetStyleList();
			POSITION pos = Styles->GetHeadPosition();
			while(pos)
			{
				pStyle = Styles->GetNext(pos);
				if(pStyle->m_styleid == styleid)
				{
					m_pStyle = pStyle;
					break;
				}
			}
			ASSERT(m_pStyle);
		}
		else 
		{
			//ASSERT(!m_bOwnStyle);
			m_pStyle = NULL;
		}

#endif
		
		ar.Read(text,sizeof(text));
		m_text = text;

		
	
	}
}

void CDrawText::Draw(CDC *pDC)
{
	ASSERT_VALID(this);

	
	CDrawRect::Draw(pDC);
	DrawText(pDC,m_text);
}
void CDrawText::DrawText(CDC* pDC,CString &drawText)
{
	ASSERT_VALID(this);

	if(!IsVisible()) return;

	CTextStyle *pStyle;
	if(m_bOwnStyle) pStyle = &m_OwnStyle;
	else pStyle = m_pStyle;

	CRect calcrc(&m_position);
	NormalizePosition(calcrc);
	CRect rc(&calcrc);


	COLORREF oldColor = pDC->GetTextColor();
	pDC->SetTextColor(pStyle->m_txtclr);
	int oldMode = pDC->GetBkMode();
	pDC->SetBkMode(TRANSPARENT);

	HFONT oldFont = (HFONT)::SelectObject(pDC->GetSafeHdc(),pStyle->RealizeStyle());
		
	VER_ALIGN valign = pStyle->m_valign;
	int height = abs(pDC->DrawText(drawText,&calcrc,  DT_WORDBREAK | DT_CALCRECT )),
	rcheight = abs((rc.bottom - rc.top));

	if(height < rcheight) 
	{
	switch(valign)
	{
		case VCENTER:
			rc.top -= (rcheight-height)/2; // Y rises up
			break;
		case BOTTOM:
			rc.top -= (rcheight-height);	
			break;
		
	}
	valign = (VER_ALIGN)0; //get rid of VCENTER or BOTTOM
	}
	
	pDC->DrawText(drawText,rc, pStyle->m_halign | DT_WORDBREAK | valign);

	::SelectObject(pDC->GetSafeHdc(),oldFont);

	pDC->SetBkMode(oldMode);
	pDC->SetTextColor(oldColor);
}


CRect CDrawText::GetRectAlign(CRect &fitRect,CSize &txtSize,HOR_ALIGN halign,VER_ALIGN valign)
{
	CRect txtRect;
	int fitWidth = abs(fitRect.Width()),fitHeight = abs(fitRect.Height());

	if(txtSize.cx >= fitWidth) 
	{
		txtRect.left = fitRect.left;
		txtRect.right = fitRect.right;
	}
	else
	{
		switch(halign)
		{
			case LEFT:
				txtRect.left = fitRect.left;
				txtRect.right = fitRect.left+txtSize.cx;
			break;
			case RIGHT:
				txtRect.left = fitRect.right-txtSize.cx;
				txtRect.right = fitRect.right;
			break;
			case CENTER:
				txtRect.left = fitRect.left+(fitWidth-txtSize.cx)/2;
				txtRect.right = txtRect.left+txtSize.cx;
			break;
		}
	}

	if(txtSize.cy >= fitHeight)
	{
		txtRect.top = fitRect.top;
		txtRect.bottom = fitRect.bottom;
	} 
	else
	{
		switch(valign)
		{
			case TOP:
				txtRect.top = fitRect.top;
				txtRect.bottom = fitRect.top-txtSize.cy;
			break;
			case BOTTOM:
				txtRect.bottom = fitRect.bottom;
				txtRect.top = fitRect.bottom+txtSize.cy;
			break;
			case VCENTER:
				txtRect.top = fitRect.top-(fitHeight-txtSize.cy)/2;
				txtRect.bottom = txtRect.top-txtSize.cy;
			break;
		}
	}

	return txtRect;

}
	

#ifndef FOR_CATTLEBASE

int CDrawText::GetHandleCount()
{
	ASSERT_VALID(this);

	return CDrawRect::GetHandleCount();
}

// returns center of handle in logical coordinates
CPoint CDrawText::GetHandle(int nHandle)
{
	ASSERT_VALID(this);

	
	return CDrawRect::GetHandle(nHandle);
}

HCURSOR CDrawText::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	return CDrawRect::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawText::MoveHandleTo(int nHandle, CPoint point, CDrawView* pView)
{
	ASSERT_VALID(this);

	CDrawRect::MoveHandleTo(nHandle, point, pView);
}

// rect must be in logical coordinates
BOOL CDrawText::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	return CDrawRect::Intersects(rect);
}

CDrawObj* CDrawText::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawText* pClone = new CDrawText();

    CopyObjTo(pClone);
	
	pClone->m_text = m_text;
	pClone->m_bOwnStyle = m_bOwnStyle;
	pClone->m_OwnStyle = m_OwnStyle;
	pClone->m_pStyle = m_pStyle;
   
	ASSERT_VALID(pClone);

	if (pDoc != NULL) 		pDoc->Add(pClone);

	ASSERT_VALID(pClone);
	return pClone;

}


void CDrawText::OnEditProperties(CPropertySheet &propSheet)
{
	ASSERT_VALID(this);

	
	CDrawRect::OnEditProperties(propSheet);

	CTextDlg *txtdlg = new CTextDlg(this);

	propSheet.AddPage(txtdlg);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////
//CDrawImg

IMPLEMENT_SERIAL(CDrawImg, CDrawRect, 0)

CDrawImg::CDrawImg() : CDrawRect(),m_picture()
{
#ifndef FOR_CATTLEBASE
	VERIFY(m_picture.Load("IMG","NULLIMG"));
#endif
	
}

CDrawImg::CDrawImg(const CRect& position)
	: CDrawRect(position),m_picture()
{
#ifndef FOR_CATTLEBASE
	VERIFY(m_picture.Load("IMG","NULLIMG"));
#endif
		
}


CDrawImg::~CDrawImg() 
{

}

void CDrawImg::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CArchiveStream arcstream(&ar);
	IStream* pStream = (IStream*)&arcstream;
	CDrawRect::Serialize(ar);
	m_picture.Serialize(ar);
	
}

void CDrawImg::Draw(CDC* pDC)
{
	ASSERT_VALID(this);
	CDrawRect::Draw(pDC);
	CRect rect = m_position;
	pDC->LPtoDP(&rect);
	rect.NormalizeRect();
	pDC->DPtoLP(&rect);
	m_picture.Draw(pDC,CPoint(rect.left,rect.top),CSize(rect.Width(),rect.Height()));

}




#ifndef FOR_CATTLEBASE
CDrawObj* CDrawImg::Clone(CDrawDoc* pDoc)
{
	ASSERT_VALID(this);

	CDrawImg* pClone = new CDrawImg(m_position);
	
	pClone->m_bPen = m_bPen;
	pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	pClone->m_logbrush = m_logbrush;
	pClone->m_nShape = m_nShape;
	pClone->m_roundness = m_roundness;
	pClone->m_picture.Copy(m_picture);

	ASSERT_VALID(pClone);

	if (pDoc != NULL) 		pDoc->Add(pClone);

	ASSERT_VALID(pClone);
	return pClone;


}


void CDrawImg::OnEditProperties(CPropertySheet &propSheet)
{
	ASSERT_VALID(this);

	CDrawRect::OnEditProperties(propSheet);
	CImgOpenDlg *imgdlg = new CImgOpenDlg(this);
  	propSheet.AddPage(imgdlg);
}
#endif




