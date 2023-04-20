#include "stdafx.h"
#include "CCheckSK.h"

CCheckSK::CCheckSK(ORIENTATION iOrientation) : m_Orientation(iOrientation)
{
    m_bCheck = FALSE;
    m_bMouseOver = FALSE;
}

CCheckSK::~CCheckSK()
{
}


BEGIN_MESSAGE_MAP(CCheckSK, CButton)
	//{{AFX_MSG_MAP(CCheckSK)
	ON_WM_MOUSEMOVE()
	//ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
END_MESSAGE_MAP()


//  ===========================================================================
//  Set check status of control
//  ===========================================================================
void
CCheckSK::SetCheck(BOOL bCheck, BOOL bRepaint)
{
    m_bCheck = bCheck;
    if (bRepaint) Invalidate();
}

//  ===========================================================================
//  Return check status of control
//  ===========================================================================
BOOL CCheckSK::GetCheck()
{
    return m_bCheck;
}

//  ...........................................................................
//  This method is called when mouse leaves the control. See _TrackMouseEvent
//  ...........................................................................
LRESULT CCheckSK::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	lParam;
    CancelHover();
    return 0;
}

//  ...........................................................................
//  BM_SETCHECK message handler
//  ...........................................................................
LRESULT CCheckSK::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
		lParam;
    switch (wParam)
    {
    case BST_CHECKED:
    case BST_INDETERMINATE:	// Indeterminate state is handled like checked state
        SetCheck(1);
        break;
    default:
        SetCheck(0);
        break;
    }
    return 0;
}

//  ...........................................................................
//  BM_GETCHECK message handler
//  ...........................................................................
LRESULT CCheckSK::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
	lParam;
	return GetCheck();
}

//  ...........................................................................
//  Change control style in this function
//  ...........................................................................
void CCheckSK::PreSubclassWindow() 
{
    UINT nBS = GetButtonStyle();
    
    //  the button should not be owner draw
    ASSERT((nBS & SS_TYPEMASK) != BS_OWNERDRAW);

    //  This class supports only check boxes
    ASSERT(nBS & BS_CHECKBOX);
    
    // Switch to owner-draw
    ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);
    
    m_nStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);

    CButton::PreSubclassWindow();
}

//  ...........................................................................
//  This function is called when the control needs to be drawn
//  ...........................................................................
void CCheckSK::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    //  this class works only for push buttons
    ASSERT (lpDIS->CtlType == ODT_BUTTON);
    
    //  get the device context pointer to draw into
    CDC*	pDC = CDC::FromHandle(lpDIS->hDC);

    //  create brush and pen
    CBrush brushLed;
    CPen penLed(PS_SOLID,1,RGB(0,0,0));

    //  .......................................................................
    //  GET THE BOUNDARY RECTANGLE OF THE BUTTON CONTROL
    //  .......................................................................
    CRect   rectItem = lpDIS->rcItem;
    CPen*	pOldPen;
    CBrush* pOldBrush;

    pDC->SetBkMode(TRANSPARENT);

    //  .......................................................................
    //  GET BUTTON CONDITION
    //  .......................................................................
    m_bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
    m_bIsFocussed = (lpDIS->itemState & ODS_FOCUS);
    m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

    //  .......................................................................
    //  GIVE COLOR TO CHECK BOX BASED ON WHETHER THE MOUSE IS OVER IT
    //  .......................................................................
    CBrush brushForeGnd;
    CPen pen;
    if ( m_bMouseOver ||m_bCheck)
    {
        brushForeGnd.CreateSolidBrush (::GetSysColor(COLOR_BTNHILIGHT));
        pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
        pOldBrush = pDC->SelectObject (&brushForeGnd);
        pOldPen = pDC->SelectObject(&pen);
        CPoint pt (4,4);
        pDC->RoundRect (&rectItem, pt);
        rectItem.DeflateRect (2,2);
    }
    else
    {
        brushForeGnd.CreateSolidBrush (::GetSysColor(COLOR_3DFACE));
        pen.CreatePen (PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));
        pOldBrush = pDC->SelectObject (&brushForeGnd);
        pOldPen = pDC->SelectObject(&pen);

        pDC->Rectangle(&rectItem);
        rectItem.DeflateRect (2,2);
    }

    //  .......................................................................
    //  IF CHECK BOX HAS FOCUS DRAW THE FOCUS BOX
    //  .......................................................................
    if (m_bIsFocussed)    pDC->DrawFocusRect(&rectItem);
    rectItem.DeflateRect(1,1);
    
    
    //  .......................................................................
    //  GET & DISPLAY THE BUTTON TEXT
    //  .......................................................................
    //  get the text on the check box
    CString sTitle = "AAA";
    CRect centerRect = rectItem;
                
    UINT uFormat;
	CFont *currFont ;
	LOGFONT lf;
	CFont newFont;
	CSize stSize;

	currFont = newFont.FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	currFont->GetLogFont(&lf);
	lf.lfEscapement = lf.lfOrientation = m_Orientation;
	newFont.CreateFontIndirect(&lf);
	currFont = pDC->SelectObject(&newFont);
	GetTextExtentPoint32(pDC->GetSafeHdc(), sTitle,sTitle.GetLength(), &stSize);


	switch(m_Orientation)
	{
		case ORIENTATION_HORIZONTAL:
			pDC->MoveTo(rectItem.left+(rectItem.Width()-stSize.cx)/2, rectItem.top+(rectItem.Height()-stSize.cy)/2);
			pDC->SetTextAlign( TA_UPDATECP);

			break;
		case ORIENTATION_VERTICAL_LEFT:
			pDC->MoveTo(rectItem.left+(rectItem.Width()-stSize.cy)/2, rectItem.top+(rectItem.Height()+stSize.cx)/2);
			pDC->SetTextAlign( TA_UPDATECP);
			break;
		case ORIENTATION_VERTICAL_RIGHT:
			pDC->MoveTo(rectItem.left+(rectItem.Width()+stSize.cy)/2,rectItem.top+(rectItem.Height()-stSize.cx)/2);
			pDC->SetTextAlign( TA_UPDATECP);
			break;
		case ORIENTATION_HORIZONTAL_FLIPED:
			pDC->MoveTo(rectItem.left+(rectItem.Width()+stSize.cx)/2,rectItem.top+(rectItem.Height()+stSize.cy)/2);
			pDC->SetTextAlign( TA_UPDATECP);

	}

	

    pDC->DrawText(sTitle, rectItem, DT_CENTER);	

	
	    if (m_bIsPressed)  rectItem.OffsetRect(1, 1);

        pDC->SetBkMode(TRANSPARENT);

        //  ...................................................................
        //  Draw the text
        //  ...................................................................
        if (m_bIsDisabled)
        {
            //  for disabled button draw etched text
            rectItem.OffsetRect(1, 1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
         //   pDC->DrawText(sTitle, -1, &rectItem, uFormat);
            rectItem.OffsetRect(-1, -1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
         //   pDC->DrawText(sTitle, -1, &rectItem, uFormat);
        }
        else
        {
            //  for non-disabled button get the color for background and foreground
            if (m_bMouseOver || m_bIsPressed)
            {
                pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
                pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
            }
            else 
            {
                pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
                pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
            }
            //  draw the text


           // pDC->DrawText(sTitle, -1, &rectItem, uFormat);
        }
    
	pDC->SelectObject(currFont);
    //  .......................................................................
    //  Release resources
    //  .......................................................................
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    penLed.DeleteObject();
    brushForeGnd.DeleteObject();
    pen.DeleteObject();
    brushLed.DeleteObject();
}

//  ...........................................................................
//  WM_MOUSEMOVE message handler
//  ...........................................................................
void CCheckSK::OnMouseMove(UINT nFlags, CPoint point) 
{
    CWnd*               wndUnderMouse = NULL;
    CWnd*               wndActive = this;
    TRACKMOUSEEVENT     csTME;
    
    ClientToScreen(&point);
    wndUnderMouse = WindowFromPoint(point);
    
    // If the mouse enter the button with the left button pressed then do nothing
    if (nFlags & MK_LBUTTON && m_bMouseOver == FALSE) return;
    
    if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd)
    {
        if (!m_bMouseOver)
        {
            m_bMouseOver = true;
            
            Invalidate();
            
            csTME.cbSize = sizeof(csTME);
            csTME.dwFlags = TME_LEAVE;
            csTME.hwndTrack = m_hWnd;
            ::_TrackMouseEvent(&csTME);
        }
    } 
    else 
        CancelHover();

	CButton::OnMouseMove(nFlags, point);
}

void CCheckSK::CancelHover()
{
    if (m_bMouseOver)  m_bMouseOver = FALSE;
    Invalidate();
}


void CCheckSK::OnClicked() 
{
	m_bCheck = !m_bCheck;
    Invalidate();
}


int CCheckSK::Create(CRect &rc,CWnd *pParentWnd,UINT nID)
{
	return CButton::Create("",BS_AUTOCHECKBOX | BS_LEFT | WS_TABSTOP | WS_VISIBLE,rc,pParentWnd,nID);
}