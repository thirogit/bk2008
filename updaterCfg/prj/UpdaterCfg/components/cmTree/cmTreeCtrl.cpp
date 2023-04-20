// cmTreeCtrl.cpp : implementation file
//


#include "stdafx.h"
#include "cmTreeCtrl.h"
#include "MemDC.h"
#include "cmDrawUtils.h"
#include "cmTreeNodeDescriptor.h"
#include "resource.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cmTreeCtrl

cmTreeCtrl::cmTreeCtrl():
	bkColor_(::GetSysColor(COLOR_WINDOW)),
	txtColor_(::GetSysColor(COLOR_WINDOWTEXT)),
	hoverItem_(NULL)
{
}

cmTreeCtrl::~cmTreeCtrl()
{
	CleanUp();
}


BEGIN_MESSAGE_MAP(cmTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(cmTreeCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmTreeCtrl message handlers

void cmTreeCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);

	UpdateDefaultDescriptor();

	GetClientRect(&m_rect);
	SCROLLINFO scroll_info;
	// Determine window portal to draw into taking into account
	// scrolling position
	if ( GetScrollInfo( SB_HORZ, &scroll_info, SIF_POS | SIF_RANGE ) )
	{
		m_h_offset = -scroll_info.nPos;
		m_h_size = max( scroll_info.nMax+1, m_rect.Width());
	}
	else
	{
		m_h_offset = m_rect.left;
		m_h_size = m_rect.Width();
	}
	if ( GetScrollInfo( SB_VERT, &scroll_info, SIF_POS | SIF_RANGE ) )
	{
		if ( scroll_info.nMin == 0 && scroll_info.nMax == 100) 
			scroll_info.nMax = 0;
		m_v_offset = -scroll_info.nPos * GetItemHeight();
		m_v_size = max( (scroll_info.nMax+2)*((int)GetItemHeight()+1), m_rect.Height() );
	}
	else
	{
		m_v_offset = m_rect.top;
		m_v_size = m_rect.Height();
	}

	DrawBackground(memDC);
	DrawItems(memDC);
}

BOOL cmTreeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void cmTreeCtrl::DrawBackground(CDC* pDC)
{
	CRect rc;

	GetClientRect(rc);
	pDC->FillSolidRect(rc, bkColor_);
}

void cmTreeCtrl::SetBkColor(COLORREF bkColor)
{
	bkColor_ = bkColor;
}

COLORREF cmTreeCtrl::GetBkColor() const
{
	return bkColor_;
}

void cmTreeCtrl::SetTextColor(COLORREF txtColor)
{
	txtColor_ = txtColor;
}

COLORREF cmTreeCtrl::GetTextColor() const
{
	return txtColor_;
}

void cmTreeCtrl::DrawItems(CDC* pDC)
{
		// draw items
	HTREEITEM show_item, parent;
	CRect rc_item;
	CRect rc;
	CRect buttonRect;
	cmString name;
	COLORREF color;
	DWORD tree_style;
	//BITMAP bm;
	CDC dc_mem;
	//CBitmap *button;
	int count = 0;
	int state;
	bool selected;
	bool has_children;

	show_item = GetFirstVisibleItem();
	if ( show_item == NULL )
		return;

	GetClientRect(rc);
	dc_mem.CreateCompatibleDC(NULL);
	color = pDC->GetTextColor();
	tree_style = ::GetWindowLong( m_hWnd, GWL_STYLE ); 

	do
	{
		state = GetItemState( show_item, TVIF_STATE );
		parent = GetParentItem( show_item );
		has_children = (ItemHasChildren( show_item ) == TRUE)/* || parent == NULL*/;
		selected = (state & TVIS_SELECTED) && ((this == GetFocus()) || 
				(tree_style & TVS_SHOWSELALWAYS));

		if ( GetItemRect( show_item, rc_item, TRUE ) )
		{
			if ( has_children  || selected )
			{
				CRect rect;
				
				// Show 
				rect.top = rc_item.top;
				rect.bottom = rc_item.bottom;
				rect.right = m_h_size + m_h_offset;
				if ( !has_children )
					rect.left = rc_item.left + m_h_offset;
				else
					rect.left = m_h_offset;
				
				//DrawUtils::DrawRect(pDC, rect, RGB(0, 255, 0));

				pDC->SetTextColor(txtColor_);

				if ( has_children )
				{
					CPoint topLeft;

					topLeft = rect.TopLeft();
					topLeft.Offset(3, 3);

					// draw button
					bool drawButtons;

					if (show_item == GetRootItem()) {
						drawButtons = (tree_style & TVS_HASBUTTONS) && (tree_style & TVS_LINESATROOT);
					} else {
						drawButtons = (tree_style & TVS_HASBUTTONS);
					}
					
					if (drawButtons) {
						buttonRect = rc_item;
						buttonRect.OffsetRect(-15, 3);
						if (GetImageList(TVSIL_NORMAL))
							buttonRect.OffsetRect(-18, 0);

						if ( state & TVIS_EXPANDED )
							cmDrawUtils::DrawBitmap(pDC, IDB_NODE_MINUS, buttonRect.TopLeft(), 0);
						else
							cmDrawUtils::DrawBitmap(pDC, IDB_NODE_PLUS, buttonRect.TopLeft(), 0);
					}

					
					/***
					if (GetImageList(TVSIL_NORMAL)) {
						CRect iconRect;

						iconRect = rc_item;
						iconRect.OffsetRect(-18, 0);

						DrawNodeIcon(pDC, show_item, iconRect);
						GetImageList(TVSIL_NORMAL)->Draw(pDC, 0, iconRect.TopLeft(), ILD_NORMAL);
					}
					***/
				}
			}
			if ( !has_children ) {
				
				// draw icon
				//
				if (GetImageList(TVSIL_NORMAL)) {
					CRect iconRect;

					iconRect = rc_item;
					iconRect.OffsetRect(-18, 0);

					GetImageList(TVSIL_NORMAL)->Draw(pDC, 0, iconRect.TopLeft(), ILD_NORMAL);
				}
			}
			name = GetItemText( show_item );
			
			// draw icon
			//
			DrawNodeIcon(pDC, show_item, rc_item);

			//rc_item.right = rc.right - 5;
			//rc_item.DeflateRect( 0,1,0,1 );
			
			pDC->DrawText( _T(" ") + name, rc_item, DT_LEFT | DT_CALCRECT);   
			
			cmTreeNodeDescriptor* desc;

			desc = GetItemDescriptor2(show_item);

			//DrawUtils::DrawRect(pDC, rc_item, RGB(255, 0, 0));
			if ( selected )
			{
				if ( !has_children  )
					pDC->SetTextColor( GetSysColor(COLOR_HIGHLIGHTTEXT) );
				COLORREF col = pDC->GetBkColor();

				pDC->SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );
				
				pDC->FillSolidRect(rc_item, ::GetSysColor(COLOR_HIGHLIGHT));
				rc_item.DeflateRect(0,1,0,0);
				cmDrawUtils::DrawString(pDC, 
									  _T(" ") + name, 
									  rc_item, 
									  DT_LEFT, 
									  desc->GetFontName(), 
									  desc->GetFontSize(), 
									  col, 
									  desc->GetFontBold());
				
				
				//pDC->SetTextColor( color );
				//pDC->SetBkColor( col );
			}
			else
			{
				if (desc->GetTransparent()) {
					pDC->SetBkMode(TRANSPARENT);
				} else {
					//pDC->SetBkMode(OPAQUE);
					//pDC->SetBkColor(desc->GetBkColor());
					pDC->FillSolidRect(rc_item, desc->GetBkColor());
				}

				rc_item.DeflateRect(0,1,0,0);
				cmDrawUtils::DrawString(pDC, 
									  _T(" ") + name, 
									  rc_item, 
									  DT_LEFT, 
									  desc->GetFontName(), 
									  desc->GetFontSize(), 
									  desc->GetTextColor(), 
									  desc->GetFontBold());
			}
			//if ( state & TVIS_BOLD )
			//	pDC->SelectObject( font );
		}
	} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );
}

int cmTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

BOOL cmTreeCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL cmTreeCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CTreeCtrl::PreCreateWindow(cs);
}

void cmTreeCtrl::UpdateDefaultDescriptor()
{
	CWnd* parent;

	parent = GetParent();
	defaultDescriptor_.SetFontName(_T("tahoma"));
	defaultDescriptor_.SetFontSize(12);
	defaultDescriptor_.SetFontBold(false);

	if (parent && parent->GetSafeHwnd()) {
		CFont* f;

		f = parent->GetFont();
		if (f) {
			LOGFONT* l = NULL;

			f->GetLogFont(l);
			if (l) {
				defaultDescriptor_.SetFontName(l->lfFaceName);
				defaultDescriptor_.SetFontSize(l->lfHeight);
				defaultDescriptor_.SetFontBold(l->lfWeight >= FW_BOLD);
			}
		}
	}
}

cmTreeNodeDescriptor* cmTreeCtrl::AddItemDescriptor(HTREEITEM hNode)
{
	cmTreeNodes::iterator it;
	cmTreeNodeDescriptor* ret = NULL;

	it = nodes_.find(hNode);
	if (it == nodes_.end()) {
		ret = new cmTreeNodeDescriptor(defaultDescriptor_);
		ret->SetTreeNode(hNode);
		nodes_[hNode] = ret;
	}

	return ret;
}

cmTreeNodeDescriptor* cmTreeCtrl::GetNodeDescriptor(HTREEITEM hNode)
{
	cmTreeNodes::iterator it;
	cmTreeNodeDescriptor* ret = NULL;

	it = nodes_.find(hNode);
	if (it != nodes_.end())
		ret = it->second;
	else
		ret = AddItemDescriptor(hNode);

	return ret;
}

cmTreeNodeDescriptor* cmTreeCtrl::GetItemDescriptor2(HTREEITEM hNode)
{
	cmTreeNodes::iterator it;
	cmTreeNodeDescriptor* ret = NULL;

	it = nodes_.find(hNode);
	if (it != nodes_.end())
		ret = it->second;
	else
		ret = &defaultDescriptor_;

	return ret;
}

BOOL cmTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (tooltip_.GetSafeHwnd())
		tooltip_.RelayEvent(pMsg);
	
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void cmTreeCtrl::PreSubclassWindow() 
{
	CTreeCtrl::PreSubclassWindow();

	UpdateDefaultDescriptor();

	InitializeTooltip();
	InitializeImageList();
}

void cmTreeCtrl::InitializeTooltip()
{
	BOOL ret;

	ret = tooltip_.Create(this, TTS_ALWAYSTIP | WS_VISIBLE | WS_POPUP);
	tooltip_.SetDelayTime(TTDT_INITIAL, 0);
	tooltip_.AddTool(this, _T("alskdjalskdjalksdjlasj"));
	SetToolTips(&tooltip_);
	tooltip_.SetMaxTipWidth(250);
	//tooltip_.Pop();
}

void cmTreeCtrl::InitializeImageList()
{
	imageList_.Create(16, 16, ILC_COLOR32 | ILC_MASK, 50, 50);
	SetImageList(&imageList_, TVSIL_NORMAL);
}

void cmTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM item;
	UINT flags;
	
	
	item = HitTest(point, &flags);
	//TRACE1("Flags=0x%08X\n", flags);
	if (flags & TVHT_ONITEM) {
		if (item != hoverItem_) {
			if (item) {
				tooltip_.SetTreeNodeDescriptor(GetItemDescriptor2(item));
				tooltip_.Activate(FALSE);
				tooltip_.Activate(TRUE);
				
				hoverItem_ = item;
			} else {
				tooltip_.Activate(FALSE);
				tooltip_.SetTreeNodeDescriptor(NULL);
				hoverItem_ = NULL;
			}
		}
	} else {
		tooltip_.Activate(FALSE);
		tooltip_.SetTreeNodeDescriptor(NULL);
		hoverItem_ = NULL;
	}
	
	CTreeCtrl::OnMouseMove(nFlags, point);
}

cmTreeNodeDescriptor* cmTreeCtrl::InsertNode(const cmString& text, 
											 HTREEITEM hParent, 
											 HTREEITEM hInsertAfter,
											 UINT iconSelExpanded, 
											 UINT iconSelCollapsed,
											 UINT iconNotSelExpanded,
											 UINT iconNotSelCollapsed)
{
	HTREEITEM item;
	cmTreeNodeDescriptor* desc;

	item = CTreeCtrl::InsertItem(text, 0, 0, hParent, hInsertAfter);
	desc = AddItemDescriptor(item);

	desc->SetText(text);
	desc->SetTreeIcon(enCmTreeIcon_SelExpanded, iconSelExpanded);
	desc->SetTreeIcon(enCmTreeIcon_SelCollapsed, iconSelCollapsed);
	desc->SetTreeIcon(emCmTreeIcon_NotSelExpanded, iconNotSelExpanded);
	desc->SetTreeIcon(enCmTreeIcon_NotSelCollapsed, iconNotSelCollapsed);

	return desc;
}

void cmTreeCtrl::DrawNodeIcon(CDC* pDC, HTREEITEM item, CRect itemRect)
{
	cmTreeNodeDescriptor* desc;
	UINT nodeState;
	enCmTreeIcon iconType;
	UINT icon;
	CRect iconRect;

	iconRect = itemRect;
	iconRect.OffsetRect(-18, 0);

	desc = GetItemDescriptor2(item);
	assert(desc);
	if (!desc)
		return;

	nodeState = GetItemState(item, TVIS_EXPANDED | TVIS_SELECTED);
	nodeState &= (TVIS_EXPANDED |  TVIS_SELECTED);

	if (nodeState == (TVIS_EXPANDED |  TVIS_SELECTED))
		iconType = enCmTreeIcon_SelExpanded;
	else if (nodeState == TVIS_SELECTED)
		iconType = enCmTreeIcon_SelCollapsed;
	else if (nodeState == TVIS_EXPANDED)
		iconType = emCmTreeIcon_NotSelExpanded;
	else
		iconType = enCmTreeIcon_NotSelCollapsed;
	
	icon = desc->GetTreeIcon(iconType);

	if (icon == 0)
		pDC->FillSolidRect(iconRect, bkColor_);
	else
		cmDrawUtils::DrawIcon(pDC, icon, iconRect.TopLeft(), 16);
}

void cmTreeCtrl::CleanUp()
{
	cmTreeNodes::iterator it;

	for (it = nodes_.begin(); it != nodes_.end(); it++)
		delete it->second;

	nodes_.clear();
	
	if (GetSafeHwnd())
		CTreeCtrl::DeleteAllItems();
}

void cmTreeCtrl::DeleteNode(HTREEITEM node)
{
	if (ItemHasChildren(node)) {
		HTREEITEM nextChild;
		HTREEITEM child = GetChildItem(node);	
	
		while (child != NULL) {
		  nextChild = GetNextItem(child, TVGN_NEXT);
		  
		  DeleteNode(child);
		  child = nextChild;
		}	
	}

	cmTreeNodeDescriptor* d;

	d = GetNodeDescriptor(node);	
	if (d) {
		delete d;
		nodes_.erase(node);
		DeleteItem(node);
	}
}