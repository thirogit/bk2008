#include "stdafx.h"
#include "DocOleObj.h"

IMPLEMENT_SERIAL(CDocOleObj, CDocObj, 0)

BOOL CDocOleObj::c_bShowItems = TRUE;

CDocOleObj::CDocOleObj() : m_extent(0,0)
{
	m_pClientItem = NULL;
}

CDocOleObj::CDocOleObj(const CRect& position) : CDocObj(position), m_extent(0, 0)
{
	m_pClientItem = NULL;
}

CDocOleObj::~CDocOleObj()
{
	if (m_pClientItem != NULL)
	{
		m_pClientItem->Release();
		m_pClientItem = NULL;
	}
}

void CDocOleObj::Serialize( CArchive& ar )
{
	ASSERT_VALID(this);

	CDocObj::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_extent;
		ar << m_pClientItem;
	}
	else
	{
		ar >> m_extent;
		ar >> m_pClientItem;
//		m_pClientItem->m_pDrawObj = this;
	}
}


void CDocOleObj::Draw(CDC* pDC)
{
	ASSERT_VALID(this);

	CDocOleItem* pItem = m_pClientItem;
	if (pItem != NULL)
	{
		// draw the OLE item itself
		pItem->Draw(pDC, GetPosition());

		// don't draw tracker in print preview or on printer
		if (!pDC->IsPrinting())
		{
			// use a CRectTracker to draw the standard effects
			CRectTracker tracker;
			tracker.m_rect = GetPosition();
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







