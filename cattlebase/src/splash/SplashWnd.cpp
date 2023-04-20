#include "stdafx.h"
#include "SplashWnd.h"

#include <Prof-UIS.h>

//#include <intshcut.h>

#if (!defined __EXT_REGISTRY_H)
	#include <ExtRegistry.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd

static bool g_bSplashWndClassRegistered = false;

CSplashWnd::CSplashWnd(
	CWnd * pWndParent,
	UINT nBitmapID
	)
	: m_nMaxTextLines( -1 )
	, m_nLineHeight( -1 )
	, m_clrText( RGB(255,255,255) )
	, m_rcText( 0, 0, 0, 0 )
{
	VERIFY( RegisterSplashWndClass() );
	VERIFY( Create( pWndParent, nBitmapID ) );
}

CSplashWnd::~CSplashWnd()
{
	if( GetSafeHwnd() != NULL )
		DestroyWindow();
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	__EXT_MFC_ON_WM_NCHITTEST()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplashWnd message handlers

void CSplashWnd::AddTextLine(
	LPCTSTR sText // = NULL
	)
{
	ASSERT_VALID( this );
	m_arrTextLines.Add( (sText == NULL) ? _T("") : sText );
	if( GetSafeHwnd() == NULL )
		return;
	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;
	Invalidate();
	UpdateWindow();
}

void CSplashWnd::ReplaceLastLine(
	LPCTSTR sText // = NULL
	)
{
	if( m_arrTextLines.GetSize() == 0 )
		return;
	m_arrTextLines.ElementAt( m_arrTextLines.GetSize() - 1 ) = (sText == NULL) ? _T("") : sText;
	if( GetSafeHwnd() == NULL )
		return;
	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;
	Invalidate();
	UpdateWindow();
}

void CSplashWnd::ClearLines()
{
	ASSERT_VALID( this );
	if( m_arrTextLines.GetSize() == 0 )
		return;
	m_arrTextLines.RemoveAll();
	if( GetSafeHwnd() == NULL )
		return;
	ASSERT( ::IsWindow( GetSafeHwnd() ) );
	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;
	Invalidate();
	UpdateWindow();
}

bool CSplashWnd::Create(
	CWnd * pWndParent,
	UINT nBitmapID
	)
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( m_bitmap.GetSafeHandle() == NULL );
	ASSERT( m_wndInvisibleParent.GetSafeHwnd() == NULL );
	if( ! RegisterSplashWndClass() )
	{
		ASSERT( FALSE );
		return false;
	}
	if( ! m_bitmap.LoadBitmap( nBitmapID ) )
	{
		ASSERT( FALSE );
		return false;
	}
BITMAP _bmpInfo;
	::memset( &_bmpInfo, 0, sizeof(BITMAP) );
	m_bitmap.GetBitmap( &_bmpInfo );
	m_sizeBitmap.cx = _bmpInfo.bmWidth;
	m_sizeBitmap.cy = _bmpInfo.bmHeight;
CExtPaintManager::monitor_parms_t _mp;
	CExtPaintManager::stat_GetMonitorParms( _mp, pWndParent );
CRect rcWnd( 0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy );
	rcWnd.OffsetRect(
		( _mp.m_rcWorkArea.Width() - m_sizeBitmap.cx ) / 2,
		( _mp.m_rcWorkArea.Height() - m_sizeBitmap.cy ) / 2
		);
	if( ! m_wndInvisibleParent.CreateEx( 0, _T("STATIC"), _T(""), WS_POPUP, 0,0,0,0, pWndParent->GetSafeHwnd(), (HMENU)NULL ) )
	{
		ASSERT( FALSE );
		m_bitmap.DeleteObject();
		return false;
	}
	if( ! CWnd::CreateEx(
			0, __SIMPLE_SPLASH_WINDOW_CLASS_NAME__, _T("Initizlizing ..."), WS_POPUP,
			rcWnd.left, rcWnd.top, m_sizeBitmap.cx, m_sizeBitmap.cy, m_wndInvisibleParent.GetSafeHwnd(), (HMENU)NULL
			)
		)
	{
		ASSERT( FALSE );
		m_bitmap.DeleteObject();
		m_wndInvisibleParent.DestroyWindow();
		return false;
	}
	VERIFY(
		::SetWindowPos(
			m_hWnd, HWND_TOP, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW|SWP_NOACTIVATE|SWP_NOOWNERZORDER
			)
		);
	CExtPopupMenuWnd::PassMsgLoop( false );
	return true;
}

BOOL CSplashWnd::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

void CSplashWnd::OnPaint() 
{
	ASSERT( m_bitmap.GetSafeHandle() != NULL );
CPaintDC dcPaint( this );
CExtMemoryDC dc( &dcPaint );
CDC dcImg;
	if( ! dcImg.CreateCompatibleDC( &dcPaint ) )
	{
		ASSERT( FALSE );
		return;
	}
CBitmap * pOldBmp = dcImg.SelectObject( &m_bitmap );
	dc.BitBlt(
		0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy,
		&dcImg,
		0, 0,
		SRCCOPY
		);
	dcImg.SelectObject( pOldBmp );
	dcImg.DeleteDC();
int nLen, i, nCount = int( m_arrTextLines.GetSize() );
	if( nCount > 0 )
	{
		if( m_rcText.IsRectEmpty() )
		{
			GetClientRect( &m_rcText );
			m_rcText.DeflateRect(
				10,
				30,
				10,
				10
				);
		}
		if( m_font.GetSafeHandle() == 0 )
		{
			LOGFONT _lf;
			::memset( &_lf, 0, sizeof(LOGFONT) );
			g_PaintManager->m_FontNormal.GetLogFont( &_lf );
			__EXT_MFC_STRCPY( _lf.lfFaceName, LF_FACESIZE, _T("Arial") );
			_lf.lfWeight = 300;
			_lf.lfItalic = FALSE;
			_lf.lfUnderline = FALSE;
			_lf.lfStrikeOut = FALSE;
			_lf.lfWidth = 0;
			_lf.lfHeight = 12;
			VERIFY( m_font.CreateFontIndirect( &_lf ) );
		}
		int nOldBkMode = dc.SetBkMode( TRANSPARENT );
		COLORREF clrTextOld = dc.SetTextColor( m_clrText );
		CFont * pOldFont = dc.SelectObject( &m_font );
		if( m_nLineHeight < 0 )
		{
			TEXTMETRIC _tm;
			::memset( &_tm, 0, sizeof(TEXTMETRIC) );
			dc.GetTextMetrics( &_tm );
			m_nLineHeight = _tm.tmHeight;
		}
		if( m_nMaxTextLines < 0 )
		{
			m_nMaxTextLines = m_rcText.Height() / m_nLineHeight;
			if( m_nMaxTextLines < 0 )
				m_nMaxTextLines = 0;
		}
		if( nCount > m_nMaxTextLines )
		{
			m_arrTextLines.RemoveAt( 0, nCount - m_nMaxTextLines );
			nCount = m_nMaxTextLines;
		}
		int nY = m_rcText.top;
		for( i = 0; i < nCount; i++ )
		{
			LPCTSTR sText = m_arrTextLines[i];
			if( sText != NULL && ( nLen = int( _tcslen(sText) ) ) > 0 )
			{
				CRect rcText = m_rcText;
				rcText.OffsetRect(
					0,
					nY - rcText.top
					);
				dc.DrawText(
					sText,
					nLen,
					&rcText,
					DT_SINGLELINE|DT_TOP|DT_RIGHT|DT_END_ELLIPSIS  
					);
			} // if( sText != NULL && ( nLen = int( _tcslen(sText) ) ) > 0 )
			nY += m_nLineHeight;
		} // for( i = 0; i < nCount; i++ )
		dc.SelectObject( pOldFont );
		dc.SetTextColor( clrTextOld );
		dc.SetBkMode( nOldBkMode );
	} // if( nCount > 0 )
}

void CSplashWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	bCalcValidRects;
	lpncsp;
}

UINT CSplashWnd::OnNcHitTest(CPoint point) 
{
	point;
	return HTCLIENT;
}

void CSplashWnd::OnClose() 
{
}

BOOL CSplashWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ( ! RegisterSplashWndClass() ) || ( ! CWnd::PreCreateWindow( cs ) ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	cs.lpszClass = __SIMPLE_SPLASH_WINDOW_CLASS_NAME__;
	return TRUE;
}

bool CSplashWnd::RegisterSplashWndClass()
{
	if( g_bSplashWndClassRegistered )
		return true;

WNDCLASS _wndClassInfo;
HINSTANCE hInst = AfxGetInstanceHandle();
	if( ! ::GetClassInfo( hInst, __SIMPLE_SPLASH_WINDOW_CLASS_NAME__, &_wndClassInfo ) )
	{
		// otherwise we need to register a new class
		_wndClassInfo.style =
				CS_GLOBALCLASS
				//|CS_DBLCLKS
				|CS_HREDRAW|CS_VREDRAW
				|CS_NOCLOSE
				|CS_SAVEBITS
				;
		_wndClassInfo.lpfnWndProc = ::DefWindowProc;
		_wndClassInfo.cbClsExtra = _wndClassInfo.cbWndExtra = 0;
		_wndClassInfo.hInstance = hInst;
		_wndClassInfo.hIcon = NULL;
		_wndClassInfo.hCursor =
				::LoadCursor(
					NULL, //hInst,
					IDC_WAIT
					)
				;
		ASSERT( _wndClassInfo.hCursor != NULL );
		_wndClassInfo.hbrBackground = NULL; 
		_wndClassInfo.lpszMenuName = NULL;
		_wndClassInfo.lpszClassName = __SIMPLE_SPLASH_WINDOW_CLASS_NAME__;
		if( !::AfxRegisterClass( &_wndClassInfo ) )
		{
			ASSERT( FALSE );
			//AfxThrowResourceException();
			return false;
		}
	}

	g_bSplashWndClassRegistered = true;
	return true;
}

BOOL CSplashWnd::DestroyWindow() 
{
	ShowWindow( SW_HIDE );
	CExtPopupMenuWnd::PassMsgLoop( false );
	CWnd::DestroyWindow();
	if( m_wndInvisibleParent.GetSafeHwnd() != NULL )
		m_wndInvisibleParent.DestroyWindow();
	return TRUE;
}

