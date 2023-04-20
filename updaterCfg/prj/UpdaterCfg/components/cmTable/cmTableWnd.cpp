// cmTableWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cmTableWnd.h"
#include "cmDrawUtils.h"
#include "cmCellDefs.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cmTableWnd

cmTableWnd::cmTableWnd():
	table_(NULL)
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, _T("cmTableWnd"), &wndcls)))
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
        wndcls.lpszClassName    = _T("cmTableWnd");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return;
        }
    }
}

cmTableWnd::~cmTableWnd()
{
	if (table_)
		delete table_;
}


BEGIN_MESSAGE_MAP(cmTableWnd, cmCompositeWnd)
	//{{AFX_MSG_MAP(cmTableWnd)
	ON_WM_CREATE()
	ON_WM_GETDLGCODE()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEACTIVATE()	
	ON_MESSAGE(CMC_SETFOCUS, OnCellSetFocus)
	ON_MESSAGE(CMC_LOSTFOCUS, OnCellLostFocus)
	ON_MESSAGE(CMC_SETSEL, OnCellSetSelection)
	ON_MESSAGE(CMC_LOSTSEL, OnCellLostSelection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cmTableWnd message handlers
cmTable* cmTableWnd::GetInnerTable()
{
	return table_;
}

int cmTableWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->dwExStyle |= WS_EX_CONTROLPARENT;
	lpCreateStruct->style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN/* | WS_VSCROLL | WS_HSCROLL*/);

	if (cmCompositeWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create and initialize optionList
	//
	table_ = new cmTable;
	table_->Create(CRect(0, 0, 0, 0), this, WS_CHILD | WS_VISIBLE, 0);
	table_->ModifyStyleEx(0, WS_EX_CONTROLPARENT | WS_EX_NOPARENTNOTIFY);
	table_->EnableBorder(false);

	upperPaneWnd_ = table_;

	UpdateWindowSize();

	return 0;
}


BOOL cmTableWnd::Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID) 
{
	BOOL ret;

	style |= WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	ret = cmCompositeWnd::Create(NULL, _T("cmTableWnd"), style, rc, parentWnd, nID, NULL);

	if (ret)
		ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	return ret;
}

UINT cmTableWnd::OnGetDlgCode() 
{
	return DLGC_WANTARROWS|DLGC_WANTALLKEYS|DLGC_WANTCHARS;
}


