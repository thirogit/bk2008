// cmSplitDialog.cpp : implementation file
//

#include "stdafx.h"
#include "cmSplitDialog.h"
#include "MemDC.h"
#include "cmDrawUtils.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BORDER 5


/////////////////////////////////////////////////////////////////////////////
// cmSplitDialog dialog


cmSplitDialog::cmSplitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SPLITTERDLG, pParent),
	initialized_(false),
	splitterPos_(200),
	selectedChildIndex_(-1),
	draggingSplitter_(false),
	embedded_(false),
	bkColor_(::GetSysColor(COLOR_BTNFACE))
{
	//{{AFX_DATA_INIT(cmSplitDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cmSplitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cmSplitDialog)
	DDX_Control(pDX, IDOK, cmdOk_);
	DDX_Control(pDX, IDCANCEL, cmdCancel_);
	DDX_Control(pDX, IDC_TREE, tree_);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cmSplitDialog, CDialog)
	//{{AFX_MSG_MAP(cmSplitDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelChangedTree)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE, OnItemExpandedTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmSplitDialog message handlers

void cmSplitDialog::OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	if (pNMTreeView->itemOld.hItem != pNMTreeView->itemNew.hItem)
		OnTreeNodeSelection(pNMTreeView->itemOld.hItem, pNMTreeView->itemNew.hItem);
	
	*pResult = 0;
}

void cmSplitDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	UpdateWindowSize();
}

void cmSplitDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);

	DrawBackground(memDC);
	DrawSplitter(memDC);

	cmdCancel_.RedrawWindow();
	cmdOk_.RedrawWindow();
}

BOOL cmSplitDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint pt;
	
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (HitSplitter(pt))  {
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL cmSplitDialog::PreTranslateMessage(MSG* pMsg) 
{
	CPoint pt;
	CRect rc;

	pt = pMsg->pt;
	ScreenToClient(&pt);
	GetClientRect(rc);

	// first of all check if we are dragging the splitter
	//
	switch (pMsg->message) {
		case WM_LBUTTONDOWN:
			if (HitSplitter(pt)) {
				draggingSplitter_ = true;
				SetCapture();
				return TRUE;
			}
			break;

		case WM_MOUSEMOVE:
			if (draggingSplitter_ && (pMsg->wParam & MK_LBUTTON)) {
				CRect rc;
				int minPos;
				int maxPos;
				CPoint point(pt);
				GetClientRect(rc);
				
				minPos = 2 * BORDER;
				maxPos = rc.right - 3 * BORDER;;
				point.x = min(point.x, maxPos);
				point.x = max(point.x, minPos);

				if (splitterPos_ != point.x) {
					splitterPos_ = point.x;
					UpdateWindowSize();
				}						
				//TRACE3("width=%d, min=%d, max=%d", rc.Width(), minPos, maxPos);
				//TRACE1("x = %d\n", point.x);
				return TRUE;
			} else {
				//TRACE1("MouseMove x=%d\n", pt.x);
			}
			
		case WM_LBUTTONUP:
			if (draggingSplitter_) {
				draggingSplitter_ = false;
				ReleaseCapture();
				return TRUE;
			}
			break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL cmSplitDialog::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void cmSplitDialog::DrawBackground(CDC* pDC)
{
	CRect rc;
	
	GetClientRect(rc);
	pDC->FillSolidRect(rc, bkColor_);

	cmDrawUtils::DrawRect(pDC, treeRect_, ::GetSysColor(COLOR_3DSHADOW));
}

void cmSplitDialog::DrawSplitter(CDC* pDC)
{
	
}

void cmSplitDialog::UpdateWindowSize()
{
	if (!initialized_)
		return;

	CRect rc;
	CRect btnRect(0, 0, 0, 0);
	CRect tmp;
	int bottomStripHeight = 1;
	int border = 0;

	GetClientRect(rc);
	if (!embedded_) {
		cmdCancel_.GetWindowRect(btnRect);
		ScreenToClient(btnRect);
		bottomStripHeight = btnRect.Height() + 2 * BORDER;

		border = BORDER;
	}

	treeRect_ = rc;
	treeRect_.DeflateRect(border, border, 0, bottomStripHeight);
	treeRect_.right = splitterPos_;
	tmp = treeRect_;
	tmp.DeflateRect(1, 1, 0, 0);
	tree_.SetWindowPos(&CWnd::wndTop, tmp.left, tmp.top, tmp.Width(), tmp.Height(), SWP_SHOWWINDOW);

	childRect_ = rc;
	childRect_.DeflateRect(splitterPos_ + BORDER, border, border, bottomStripHeight);
	if (selectedChildIndex_ != -1) {
		CWnd* child;
		tmp = childRect_;
		tmp.InflateRect(0, 0, 0, 1);
		
		child = rightPaneChildren_[selectedChildIndex_];
		
		child->SetParent(this);
		child->SetOwner(this);
		child->SetWindowPos(&CWnd::wndTop, tmp.left, tmp.top, tmp.Width(), tmp.Height(), SWP_SHOWWINDOW);
	}

	//tmp.left = rc.right - btnRect.Width() - BORDER;
	
	if (!embedded_) {
		cmdCancel_.SetWindowPos(&CWnd::wndTop, rc.right - btnRect.Width() - BORDER, treeRect_.bottom + BORDER, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
		cmdCancel_.GetWindowRect(btnRect);
		ScreenToClient(btnRect);
	
		cmdOk_.SetWindowPos(&CWnd::wndTop, btnRect.left - btnRect.Width() - BORDER, treeRect_.bottom + BORDER, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
	}

	RedrawWindow();
}

void cmSplitDialog::OnTreeNodeSelection(HTREEITEM oldItem, HTREEITEM newItem)
{

}

BOOL cmSplitDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	initialized_ = true;
	
	UpdateWindowSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int cmSplitDialog::AppendChild(CWnd* childWindow)
{
	rightPaneChildren_.push_back(childWindow);

	return rightPaneChildren_.size() - 1;
}

void cmSplitDialog::SelectChild(unsigned int index)
{
	if (index < 0 || index >= rightPaneChildren_.size())
		return;

	if (selectedChildIndex_ != -1)
		rightPaneChildren_[selectedChildIndex_]->ShowWindow(SW_HIDE);

	selectedChildIndex_ = index;

	UpdateWindowSize();
}

bool cmSplitDialog::HitSplitter(const CPoint& pt)
{
	if (pt.y > treeRect_.bottom)
		return false;

	if (pt.x < splitterPos_)
		return false;
	
	if (pt.x > splitterPos_ + BORDER)
		return false;

	return true;
}

void cmSplitDialog::OnItemExpandedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	OnTreeNodeExpansion(pNMTreeView->itemNew.hItem);

	*pResult = 0;
}

void cmSplitDialog::OnTreeNodeExpansion(HTREEITEM item)
{
	
}

CWnd* cmSplitDialog::GetChild(unsigned int index)
{
	if (index < 0 || index >= rightPaneChildren_.size())
		return NULL;

	return rightPaneChildren_[index];
}

CWnd* cmSplitDialog::GetActiveChild()
{
	if (selectedChildIndex_ == -1)
		return NULL;
	
	return rightPaneChildren_[selectedChildIndex_];
}

void cmSplitDialog::SetBkColor(COLORREF bkColor)
{
	bkColor_ = bkColor;
}

COLORREF cmSplitDialog::GetBkColor() const
{
	return bkColor_;
}
