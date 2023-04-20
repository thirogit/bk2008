// ToolTipHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DTList.h"
#include "ToolTipHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipHeaderCtrl


CToolTipHeaderCtrl::CToolTipHeaderCtrl()
{
}

CToolTipHeaderCtrl::~CToolTipHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CToolTipHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CToolTipHeaderCtrl)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTipHeaderCtrl message handlers

//This function called form DataTipListCtrl's InsertColumn() overriden function.
int CToolTipHeaderCtrl::AddToolTip(int nCol, LPCTSTR lpszTip, int nFormat)
{
	// Get header string extent
	CDC *pDC = GetDC();
	CFont *pOldFont = (CFont*)pDC->SelectObject(GetFont());
	int cxText = pDC->GetTextExtent(lpszTip, _tcslen(lpszTip)).cx;
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	RECT rcColumn;
	GetItemRect(nCol, &rcColumn);

	// Depending on column format, add margin to header string extent
	RECT rcText = rcColumn;
	if(nFormat & LVCFMT_CENTER)
	{
		rcText.left = rcColumn.right - (rcColumn.right-rcColumn.left)/2 - cxText/2;
		rcText.right = rcText.left + cxText;
	}
	else if(nFormat & LVCFMT_RIGHT)
	{
		rcText.right = rcColumn.right - 6;
		rcText.left = rcText.right - cxText;
	}
	else
	{
		rcText.left = rcColumn.left + 6;
		rcText.right = rcText.left + cxText;
	}

	// Calculate tooltip hover rectangle
	CRect rectTool = rcColumn;
	RECT rcList;
	GetParent()->GetClientRect(&rcList);
	if(cxText > rectTool.Width()-12)
	{
		if(rectTool.Width() > 24)
		{
			rectTool.left += 8;
			rectTool.right -= 8;
		}
	}
	else if(rcText.left < 0)
	{
		if(rectTool.Width() > 24)
			rectTool.right -= 8;
	}
	else if(rcText.right > rcList.right)
	{
		if(rectTool.Width() > 24)
			rectTool.left += 8;
	}
	else
		rectTool.SetRectEmpty();

	return m_Tooltip.AddTool(this, lpszTip, (LPCRECT)rectTool, nCol+1);
}

// This function called when scrolling or column resizing.
void CToolTipHeaderCtrl::RecalcToolRect()
{
	// This prevent debug assertion fail on last WM_MOVE on app close.
	if(!::IsWindow(m_Tooltip.m_hWnd))
		return;

	CDC *pDC = GetDC();
	CFont *pOldFont = (CFont*)pDC->SelectObject(GetFont());

	TCHAR buf[MAX_TIP_LENGTH+1];

	HDITEM hdCol;
	hdCol.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
	hdCol.pszText = buf;
	hdCol.cchTextMax = MAX_TIP_LENGTH; 

	RECT rcColumn;
	GetClientRect(&rcColumn);

	// Get scroll position
	int nHScrollPos = GetParent()->GetScrollPos(SB_HORZ);
	rcColumn.left -= nHScrollPos;
	rcColumn.right -= nHScrollPos;

	RECT rcList;
	GetParent()->GetClientRect(&rcList);

	CRect rectTool;
	RECT rcText;
	int cxText;
	int numcol = GetItemCount();
	for(int col=0; col<numcol; col++)
	{
		// Get column string extent
		GetItem(col, &hdCol);
		rcColumn.right = rcColumn.left + LOWORD(hdCol.cxy);
		rcText = rcColumn;
		cxText = pDC->GetTextExtent(hdCol.pszText, _tcslen(hdCol.pszText)).cx;

		// Depending on column format, add margin to header string extent
		if(hdCol.fmt & HDF_CENTER)
		{
			rcText.left = rcColumn.right - (rcColumn.right-rcColumn.left)/2 - cxText/2;
			rcText.right = rcText.left + cxText;
		}
		else if(hdCol.fmt & HDF_RIGHT)
		{
			rcText.right = rcColumn.right - 6;
			rcText.left = rcText.right - cxText;
		}
		else
		{
			rcText.left = rcColumn.left + 6;
			rcText.right = rcText.left + cxText;
		}

		// Calculate ToolTip hover rectangle
		rectTool = rcColumn;
		if(cxText > rectTool.Width()-12)
		{
			if(rectTool.Width() > 24)
			{
				rectTool.left += 8;
				rectTool.right -= 8;
			}
		}
		else if(rcText.left < 0)
		{
			rectTool = rcColumn;
			if(rectTool.Width() > 24)
				rectTool.right -= 8;
		}
		else if(rcText.right > rcList.right)
		{
			rectTool = rcColumn;
			if(rectTool.Width() > 24)
				rectTool.left += 8;
		}
		else
			rectTool.SetRectEmpty();

		rectTool.OffsetRect(nHScrollPos, 0);
		m_Tooltip.SetToolRect(this, col+1, &rectTool);

		rcColumn.left = rcColumn.right;
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}


void CToolTipHeaderCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Tooltip.Create(this);
	m_Tooltip.Activate(TRUE);
	
	CHeaderCtrl::PreSubclassWindow();
}


BOOL CToolTipHeaderCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Tooltip.RelayEvent(pMsg);
	
	return CHeaderCtrl::PreTranslateMessage(pMsg);
}


void CToolTipHeaderCtrl::OnMove(int x, int y) 
{
	CHeaderCtrl::OnMove(x, y);
	
	// TODO: Add your message handler code here
	RecalcToolRect();
}
