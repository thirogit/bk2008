#include "stdafx.h"
#include "TipWnd.h"

#define CLASSNAME "MFC_TipWnd"
#define MARGIN 10
#define START_SIZE_X 10
#define START_SIZE_Y 10
#define DELTA_SIZE_Y 20
#define SHOW_TIMER_ID 1001
#define SHOW_STEP_DELAY 50

BEGIN_MESSAGE_MAP(CTipWnd,CWnd)
	
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_TIMER()

	

END_MESSAGE_MAP()


CTipWnd::CTipWnd() : m_bShowing(false) ,m_targetSize(0,0)
{
	RegisterWndClass(AfxGetInstanceHandle());
	m_rgbBorderOuter = RGB( 0, 128, 0 );
	m_rgbBorderInner = RGB( 0, 128, 64 );
}

CTipWnd::~CTipWnd()
{

}

BOOL CTipWnd::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.lpszClassName = CLASSNAME; 
	wc.hInstance = hInstance;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = NULL;
	wc.hbrBackground = (HBRUSH) ::GetStockObject(COLOR_WINDOW);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_SAVEBITS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	return (::RegisterClass(&wc) != 0);
}

void CTipWnd::SetColors(COLORREF inner,COLORREF outer)
{
	m_rgbBorderOuter = outer;
	m_rgbBorderInner = inner;
}

BOOL CTipWnd::Create( CWnd* pParent )
{

	m_rgbTextColour =  GetSysColor( COLOR_INFOTEXT );		
	/*m_rgbBorderOuter = RGB( 128, 0, 0 );
	m_rgbBorderInner = RGB( 255, 0, 0 );
	m_rgbBackgroundTop = RGB( 255, 149, 149 );
	m_rgbBackgroundBottom = RGB( 255, 85, 85 );
	*/
	
	
	m_rgbBackgroundTop = m_rgbBorderOuter;
	m_rgbBackgroundBottom = m_rgbBorderInner;
	

	if(!CWnd::CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST ,CLASSNAME,"",WS_POPUP,0,0,0,0,pParent->GetSafeHwnd(),NULL,NULL))
		return FALSE;
	
	
	// get system message font

	NONCLIENTMETRICS ncm = { 0 };
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	
	if ( m_fntTitleFont.m_hObject )
		m_fntTitleFont.DeleteObject();
	return ( m_fntTitleFont.CreateFontIndirect( &ncm.lfMessageFont ) != NULL );
}

BOOL CTipWnd::Show(  CPoint& pPoint, LPCTSTR lpszItemText )
{
	CString strItemText = lpszItemText;
	
	if ( !m_hWnd || strItemText.IsEmpty() )
		return FALSE;
	

	SetWindowText( strItemText );
	
	CClientDC dcClient( this );
	
	CFont *OldFont = dcClient.SelectObject( &m_fntTitleFont );
		
	CRect rcTextExtent;
			
	// calculate item text extent...
	dcClient.DrawText( strItemText, strItemText.GetLength(), rcTextExtent, DT_LEFT | DT_NOPREFIX | DT_VCENTER | DT_CALCRECT );
	
	dcClient.SelectObject( OldFont );
	
	
	m_bShowing = true;
	m_targetSize.SetSize(rcTextExtent.Width() + 2*MARGIN, rcTextExtent.Height()+ 2*MARGIN);
	m_currentSize.SetSize(START_SIZE_X,START_SIZE_Y);
	m_lastDivRemainder = 0;

	// show titletip at new location
	if ( !SetWindowPos( NULL, pPoint.x, pPoint.y, m_currentSize.cx,m_currentSize.cy,
							SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOCOPYBITS ) )
		return FALSE;

	
	
	SetTimer(SHOW_TIMER_ID,SHOW_STEP_DELAY,NULL);

	
	return TRUE;
}

void CTipWnd::OnTimer(UINT nTimerID)
{
	if(nTimerID == SHOW_TIMER_ID && m_bShowing)
	{
		KillTimer(nTimerID);

		if((m_currentSize.cy + DELTA_SIZE_Y) < m_targetSize.cy)
		{	
			m_currentSize.cy += DELTA_SIZE_Y;
			
			int iNumerator = m_targetSize.cx*DELTA_SIZE_Y;
			int iDenominator = m_targetSize.cy;

			m_currentSize.cx += (iNumerator+m_lastDivRemainder)/iDenominator;
			m_lastDivRemainder = (iNumerator+m_lastDivRemainder)%iDenominator;

		}
		else
		{
			m_currentSize.SetSize(m_targetSize.cx,m_targetSize.cy);
			m_bShowing = false;
			m_lastDivRemainder = 0;
		}

		SetWindowPos( NULL, 0, 0, m_currentSize.cx,m_currentSize.cy,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS );

		if(m_bShowing)
			SetTimer(SHOW_TIMER_ID,SHOW_STEP_DELAY,NULL);	

	}

}

BOOL CTipWnd::Hide()
{
	//if ( GetCapture() == this )
	//	ReleaseCapture();
	return m_hWnd ? ShowWindow( SW_HIDE ) : FALSE;
}


UINT CTipWnd::HitTest(CPoint &ptClient)
{
	CPoint ptScreen(ptClient);
	ClientToScreen( &ptScreen );	
	return GetParent()->SendMessage( WM_NCHITTEST, 0, MAKELPARAM( ptScreen.x, ptScreen.y ) );
}



void CTipWnd::OnMouseMessage(UINT message,UINT ncMessage,UINT nFlags,CPoint point)
{
	CPoint screenPt(point);
	ClientToScreen(&screenPt);
	CWnd *pParent = GetParent();
	CPoint parentPt(screenPt);
	pParent->ScreenToClient( &parentPt );
	pParent->PostMessage(  message, nFlags, MAKELPARAM( parentPt.x, parentPt.y ) );			
	
}

void CTipWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_LBUTTONDBLCLK, WM_NCLBUTTONDBLCLK,nFlags,point );
		
}

void CTipWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_LBUTTONDOWN, WM_NCLBUTTONDOWN,nFlags,point );
		
}

void CTipWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_LBUTTONUP, WM_NCLBUTTONUP,nFlags,point );
		
}

void CTipWnd::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_MBUTTONDBLCLK, WM_NCMBUTTONDBLCLK,nFlags,point );
		
}

void CTipWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_MBUTTONDOWN, WM_NCMBUTTONDOWN,nFlags,point );
		
}

void CTipWnd::OnMButtonUp(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_MBUTTONUP, WM_NCMBUTTONUP,nFlags,point );
}

void CTipWnd::OnMouseMove(UINT nFlags, CPoint point)
{
		
}

void CTipWnd::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_RBUTTONDBLCLK, WM_NCRBUTTONDBLCLK, nFlags,point );
}

void CTipWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_RBUTTONDOWN, WM_NCRBUTTONDOWN,nFlags,point );
}

void CTipWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	OnMouseMessage(WM_RBUTTONUP, WM_NCRBUTTONUP,nFlags,point );
}

void CTipWnd::OnPaint()
{
	CPaintDC dcPaint( this );
	
	int nContextState = dcPaint.SaveDC();
	
	CRect rcClient;
	GetClientRect( rcClient );
	
	CRect rcTitleTip( rcClient );
				
	CPen penBorder;
	penBorder.CreatePen( PS_SOLID, 1, m_rgbBorderOuter );
	
	dcPaint.SelectObject( &penBorder );
	dcPaint.SelectStockObject( HOLLOW_BRUSH );
	
	dcPaint.RoundRect( rcTitleTip, CPoint( 5, 5 ) );
	rcTitleTip.DeflateRect( 1, 1 );
	
	CPen penInnerBorder;
	penInnerBorder.CreatePen( PS_SOLID, 1, m_rgbBorderInner );
	dcPaint.SelectObject( &penInnerBorder );
	
	dcPaint.RoundRect( rcTitleTip, CPoint( 2, 2 ) );
	rcTitleTip.DeflateRect( 1, 1 );
	
	GRADIENT_RECT grdRect = { 0, 1 };
	TRIVERTEX triVertext[ 2 ] = {
									rcTitleTip.left,
									rcTitleTip.top,
									GetRValue( m_rgbBackgroundTop ) << 8,
									GetGValue( m_rgbBackgroundTop ) << 8,
									GetBValue( m_rgbBackgroundTop ) << 8,
									0x0000,			
									rcTitleTip.right,
									rcTitleTip.bottom,
									GetRValue( m_rgbBackgroundBottom ) << 8,
									GetGValue( m_rgbBackgroundBottom ) << 8,
									GetBValue( m_rgbBackgroundBottom ) << 8,
									0x0000
								};
	
	dcPaint.GradientFill( triVertext, 2, &grdRect, 1, GRADIENT_FILL_RECT_V );
	
	int nTextLength = GetWindowTextLength() + 1;
	CString strItemText;
	GetWindowText( strItemText.GetBuffer( nTextLength ), nTextLength );
	strItemText.ReleaseBuffer();
	
	dcPaint.SelectObject( &m_fntTitleFont );
	dcPaint.SetTextColor( m_rgbTextColour );
	dcPaint.SetBkMode( TRANSPARENT );
	
	CRect rcItemText( rcClient );
	rcItemText.OffsetRect( MARGIN, MARGIN );
	
	dcPaint.DrawText( strItemText, strItemText.GetLength(), rcItemText, DT_LEFT  | DT_NOPREFIX | DT_VCENTER );

	dcPaint.RestoreDC( nContextState );
}


