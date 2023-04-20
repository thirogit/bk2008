// AsOptionListWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cmOptionListWnd.h"
#include "MemDC.h"
#include "ICmCellListener.h"
#include "cmDrawUtils.h"
#include "cmOptionRow.h"
#include "cmBaseCellInfo.h"
#include "cmCellDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFO_BAR_HEIGHT 70

/////////////////////////////////////////////////////////////////////////////
// cmOptionListWnd

cmOptionListWnd::cmOptionListWnd():
	bkColor_(::GetSysColor(COLOR_3DFACE)),
	optionList_(NULL),
	cellListener_(NULL)
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, _T("cmOptionListWnd"), &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = _T("cmOptionListWnd");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return;
        }
    }
}

cmOptionListWnd::~cmOptionListWnd()
{
	if (optionList_)
		delete optionList_;
}


BEGIN_MESSAGE_MAP(cmOptionListWnd, cmCompositeWnd)
	//{{AFX_MSG_MAP(cmOptionListWnd)
	ON_WM_CREATE()
	ON_WM_GETDLGCODE()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cmOptionListWnd message handlers

int cmOptionListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->dwExStyle |= WS_EX_CONTROLPARENT;
	lpCreateStruct->style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN/* | WS_VSCROLL | WS_HSCROLL*/);

	if (cmCompositeWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create and initialize optionList
	//
	optionList_ = new cmOptionList;
	optionList_->Create(CRect(0,0,0,0), this, 0);
	//optionList_->RegisterListener(this);
	optionList_->ModifyStyleEx(0, WS_EX_CONTROLPARENT | WS_EX_NOPARENTNOTIFY);
	
	upperPaneWnd_ = optionList_;
	
	UpdateWindowSize();

	return 0;
}

cmOptionList* cmOptionListWnd::GetOptionList()
{
	return optionList_;
}

BOOL cmOptionListWnd::Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID) 
{
	BOOL ret;

	style |= WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	ret = cmCompositeWnd::Create(NULL, _T("cmOptionListWnd"), style, rc, parentWnd, nID, NULL);

	if (ret)
		ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	return ret;
}

void cmOptionListWnd::SetCellListener(ICmCellListener* cellListener)
{
	cellListener_ = cellListener;
}

UINT cmOptionListWnd::OnGetDlgCode() 
{
	return DLGC_WANTARROWS|DLGC_WANTALLKEYS|DLGC_WANTCHARS;
}

void cmOptionListWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	cmCompositeWnd::OnActivate(nState, pWndOther, bMinimized);
	
	SetFocus();
}

int cmOptionListWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{	
	SetFocus();

	return cmCompositeWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

