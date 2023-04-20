/* 
 * Kenny Liu
 * http://www.codeproject.com/Members/yonken
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 */

// VComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "VComboBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVComboBox

// Virtual edit notification codes. The values have been
// chosen randomly with the requirement that they do not 
// clash with the EN_* notifications.
#define VEN_SETFOCUS				90
#define VEN_KILLFOCUS				91

#define WC_VCOMBOEDITBOX			_T("Edit")

void CommandNotify( HWND hWnd, int nCode, HWND hWndCtrl )
{
	SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hWndCtrl), nCode), (LPARAM)hWndCtrl);
}

// For debugging
//#define VCOMBOTRACE		TRACE

#ifndef VCOMBOTRACE
	#define VCOMBOTRACE		1 ? (void)0 : ::AfxTrace
#endif

#define ID_TRANSITION_TIMER		100
#define	TRANSITION_TIMER_INT	50 // Timer Interval

/*----------------------------------------------------------------------------*/
/* CVComboEdit
/*----------------------------------------------------------------------------*/

CVComboEdit::CVComboEdit()
	: m_pVComboBox(NULL)
{
	
}

CVComboEdit::~CVComboEdit()
{
	
}

BEGIN_MESSAGE_MAP(CVComboEdit, CVComboEditBase)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_SYSKEYDOWN()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

void CVComboEdit::SetBuddyComboBox( CVComboBox* pVComboBox )
{
	m_pVComboBox = pVComboBox;
}

int CVComboEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CVComboEditBase::OnCreate(lpCreateStruct) == -1 )
		return -1;
	ASSERT( m_pVComboBox && m_pVComboBox->GetSafeHwnd() == lpCreateStruct->hwndParent );
	return 0;
}

void CVComboEdit::OnDestroy()
{
	ASSERT(m_pVComboBox);
	VCOMBOTRACE("======= CVComboEdit::OnDestroy()\n");
	CVComboEditBase::OnDestroy();
}

void CVComboEdit::OnSetFocus( CWnd* pOldWnd )
{
	VCOMBOTRACE("======= CVComboEdit::OnSetFocus()\n");
	ASSERT(m_pVComboBox);
	if ( pOldWnd != m_pVComboBox && (!pOldWnd || (pOldWnd && pOldWnd->GetParent() != m_pVComboBox)) )
	{
		CommandNotify(VEN_SETFOCUS);
	}
	CVComboEditBase::OnSetFocus(pOldWnd);
}

void CVComboEdit::OnKillFocus( CWnd* pNewWnd )
{
	VCOMBOTRACE("======= CVComboEdit::OnKillFocus()\n");
	ASSERT(m_pVComboBox);
	if ( pNewWnd != m_pVComboBox && (!pNewWnd || (pNewWnd && pNewWnd->GetParent() != m_pVComboBox)) )
	{
		CommandNotify(VEN_KILLFOCUS);
	}
	CVComboEditBase::OnKillFocus(pNewWnd);
}

void CVComboEdit::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	VCOMBOTRACE("======= CVComboEdit::OnKeyDown(%d)\n", nChar);
	ASSERT(m_pVComboBox);
	if( (VK_UP != nChar) && 
		(VK_DOWN != nChar)  &&
		(VK_PRIOR != nChar) &&
		(VK_NEXT != nChar) 
	  )
		CVComboEditBase::OnKeyDown(nChar, nRepCnt, nFlags);

	m_pVComboBox->SendMessage(WM_KEYDOWN, nChar, MAKELPARAM(nRepCnt, nFlags));
}

BOOL CVComboEdit::PreTranslateMessage( MSG* pMsg )
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( (VK_RETURN == pMsg->wParam) && ::GetFocus() == m_hWnd )
		{
			return FALSE;
			//m_pVComboBox->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		}
	}
	return CVComboEditBase::PreTranslateMessage(pMsg);
}

void CVComboEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	VCOMBOTRACE("======= CVComboEdit::OnChar()\n");
	ASSERT(m_pVComboBox);
	if ( VK_ESCAPE == nChar || VK_RETURN == nChar )
		return;
	CVComboEditBase::OnChar(nChar, nRepCnt, nFlags);
}

void CVComboEdit::OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	VCOMBOTRACE("======= CVComboEdit::OnSysKeyDown()\n");
	ASSERT(m_pVComboBox);
	m_pVComboBox->OnSysKeyDown(nChar, nRepCnt, nFlags);
	CVComboEditBase::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

UINT CVComboEdit::OnGetDlgCode()
{
	const MSG* pCurMsg = GetCurrentMessage();
	ASSERT(pCurMsg && m_pVComboBox);
	if ( pCurMsg && m_pVComboBox )
	{
		LPMSG pMsg = (LPMSG)pCurMsg->lParam;
		if( pMsg
			&& (WM_KEYDOWN == pMsg->message) 
			&& ((VK_ESCAPE == pMsg->wParam) || (VK_RETURN == pMsg->wParam))
			)
		{
			return m_pVComboBox->OnGetDlgCode();
		}
	}
	return CVComboEditBase::OnGetDlgCode();
}

void CVComboEdit::OnLButtonDown( UINT nFlags, CPoint point )
{
	VCOMBOTRACE("======= CVComboEdit::OnLButtonDown()\n");
	ASSERT(m_pVComboBox);
	// If the edit control does not have the focus, just set the focus
	// and exit. This will enable us to correctly select the
	// text in the edit control when we get focus.
	if( GetFocus() != this )
	{
		SetFocus();
	}
	else
		CVComboEditBase::OnLButtonDown(nFlags, point);
}

void CVComboEdit::CommandNotify( int nCode )
{
	ASSERT(m_pVComboBox);
	::CommandNotify(m_pVComboBox->m_hWnd, nCode, m_hWnd);
}

// Constants  we use to specify the edit control size & the size of the
// scroll-down frame control on the right of the combo box.
const int	BORDER_WIDTH			= 3;
const int	BORDER_HEIGHT			= 3;
const int	SCROLLBUTTON_WIDTH		= 16;

// The edit & listbox IDs, and the size of the item text buffer.
#define	EDITCTRL_ID		0x03e9
#define LISTCTRL_ID		0x03e8
#define	ITEMBUFFER_LEN	260


// Virtual listbox notification codes. The values have been
// chosen randomly with the requirement that they do not 
// clash with the LBN_* notifications.
#define	VLBN_SELCHANGE				90
#define	VLBN_CLOSEUP				91
#define VLBN_SETFOCUS				92

// Virtual listbox styles.
#define	VLBS_SIMPLE				0x0001L
#define	VLBS_DROPDOWN			0x0002L
#define VLBS_DROPDOWNLIST		0x0003L

#define WC_VCOMBOLISTBOX		_T("ComboLBox")

/*----------------------------------------------------------------------------*/
/* CVComboListBox
/*----------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CVComboListBox, CVComboListBoxBase)

CVComboListBox::CVComboListBox()
{
	
}

CVComboListBox::~CVComboListBox()
{
	
}

BEGIN_MESSAGE_MAP(CVComboListBox, CVComboListBoxBase)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_SHOWWINDOW()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_MESSAGE(LB_GETCURSEL, OnGetCurSel)
	ON_MESSAGE(LB_SETCURSEL, OnSetCurSel)
END_MESSAGE_MAP()

BOOL CVComboListBox::PreCreateWindow( CREATESTRUCT& cs )
{
	CVComboListBoxBase::PreCreateWindow(cs);

	VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
	if ( cs.lpCreateParams )
	{
		DWORD dwType = *(DWORD *)(cs.lpCreateParams);
		switch( dwType & 0x000f )
		{
		case VLBS_SIMPLE:
			data.listboxType = SIMPLE;
			break;
		case VLBS_DROPDOWN:
			data.listboxType = DROPDOWN;
			break;
		case VLBS_DROPDOWNLIST:
			data.listboxType = DROPDOWNLIST;
			break;
		}
	}
	else
		data.listboxType = DROPDOWN;
	return TRUE;
}

void CVComboListBox::CommandNotify( int nCode )
{
	ASSERT( GetBuddyComboBox() );
	::CommandNotify(GetBuddyComboBox()->m_hWnd, nCode, m_hWnd);
}

void CVComboListBox::OnLButtonDown( UINT nFlags, CPoint point )
{
	if ( GetBuddyComboBox() )
	{
		VCOMBOTRACE("======= CVComboListBox::OnLButtonDown()\n");
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		if ( data.listboxType != SIMPLE )
		{
			// If clicked outside, release the capture. 
			// If the cursor is over the scrollbar, release the mouse capture,
			// and let the scrollbar handling go on. Else do nothing.
			CRect	rect;
			POINT	pt;
			int		nHitTest;
			
			GetCursorPos(&pt);
			nHitTest = DefWindowProc(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));
			BOOL bVScroll = (HTVSCROLL == nHitTest);
			if( bVScroll || (HTHSCROLL == nHitTest) )
			{
				// Set the window bytes to indicate that the scrollbar is in ACTION.
				data.nHitTest = nHitTest;
				
				ReleaseCapture();
				// Let normal scrollbar action takeover from now on.
				PostMessage(WM_NCLBUTTONDOWN, nHitTest, MAKELPARAM(pt.x, pt.y));
			}
			else
			{
				GetWindowRect(rect);
				if( !PtInRect(rect, pt) )
					ReleaseCapture();
			}
		}
	}
	CVComboListBoxBase::OnLButtonDown(nFlags, point);
}

void CVComboListBox::OnLButtonUp( UINT nFlags, CPoint point )
{
	if ( GetBuddyComboBox() )
	{
		VCOMBOTRACE("======= CVComboListBox::OnLButtonUp()\n");
		int		nCurSel;
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		
		// No mouse capture logic needed for the VLBS_SIMPLE style
		// virtual listbox. Just send the VLBN_SELCHANGE notification,
		// if necessary.
		if( SIMPLE == data.listboxType )
		{
			nCurSel = DefWindowProc(LB_GETCURSEL, 0, 0);
			if( (LB_ERR != nCurSel) && (data.nCurSel != nCurSel) )
			{
				// Set the selection.
				data.nCurSel = nCurSel;
				
				// Send a VLBN_SELCHANGE notification to the parent.
				CommandNotify(VLBN_SELCHANGE);
			}
		}
		else
		{
			// Check if the point is on the client rectangle. If it is,
			// & there is a valid selection, release the mouse capture & 
			// let the window get hidden. 
			CRect	rect;
			GetClientRect(rect);
			if( PtInRect(rect, point) && 
				(CB_ERR != (nCurSel = DefWindowProc(LB_GETCURSEL, 0, 0))) )
			{
				if( nCurSel != data.nCurSel )
				{
					// Set the selection.
					data.nCurSel = nCurSel;
					
					// Send a VLBN_SELCHANGE notification to the parent.
					CommandNotify(VLBN_SELCHANGE);
				}
				ReleaseCapture();
			}
		}
	}
	CVComboListBoxBase::OnLButtonUp(nFlags, point);
}

void CVComboListBox::OnMouseMove( UINT nFlags, CPoint point )
{
	if ( GetBuddyComboBox() )
	{
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		// No need to track mouse movement when the listbox has the
		// VLBS_SIMPLE style.
		if( SIMPLE != data.listboxType )
		{
			CRect	rect;
			// Keep track of the mouse movement & change the selected item.		
			GetClientRect(rect);
			if( PtInRect(rect, point) )
			{
				int nSel = (int)SendMessage(LB_ITEMFROMPOINT, 0, MAKELPARAM(point.x, point.y));
				if( (nSel != CB_ERR) && 
					(nSel != DefWindowProc(LB_GETCURSEL, 0, 0)) )
				{
					// Change the selection iff the point (left, bottom) is 
					// in the window rect.
					
					RECT	rcItem;
					POINT	ptLeftBottom;
					
					GetItemRect(nSel, &rcItem);
					ptLeftBottom.x = rcItem.left;
					ptLeftBottom.y = rcItem.bottom;
					ClientToScreen(&ptLeftBottom);
					GetWindowRect(&rect);
					
					if( PtInRect(&rect, ptLeftBottom) )
						DefWindowProc(LB_SETCURSEL, nSel, 0);
				}
			}
		}
	}
	CVComboListBoxBase::OnMouseMove(nFlags, point);
}

void CVComboListBox::OnCaptureChanged( CWnd* pWnd )
{
	VCOMBOTRACE("======= CVComboListBox::OnCaptureChanged()\n");
	if ( GetBuddyComboBox() )
	{
		// Close the list box if the scrollbar is not in ACTION & the window is visible.
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		if( (HTHSCROLL != data.nHitTest) && (HTVSCROLL != data.nHitTest) && (SIMPLE != data.listboxType) )
		{
			if( (IsWindowVisible()) )
			{
				ShowWindow(SW_HIDE);
			}
			
			// Send the VLBN_CLOSEUP notification to the combobox.
			CommandNotify(VLBN_CLOSEUP);
		}
	}
	CVComboListBoxBase::OnCaptureChanged(pWnd);
}

void CVComboListBox::OnShowWindow( BOOL bShow, UINT nStatus )
{
	VCOMBOTRACE("======= CVComboListBox::OnShowWindow(%d, %d)\n", bShow, nStatus);
	CVComboListBoxBase::OnShowWindow(bShow, nStatus);

	if ( GetBuddyComboBox() )
	{
		// Set the mouse capture when the window is shown.
		// The listbox shall remain visible only so long as the mouse
		// capture is ON or while the scrollbar is in ACTION.
		// SetCapture logic not needed for VLBS_SIMPLE style.
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		if( bShow  && (SIMPLE != data.listboxType) )
			SetCapture();
		else
		{
			if( ::GetCapture() == m_hWnd ) 
				ReleaseCapture();
		}
	}
}

void CVComboListBox::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	VCOMBOTRACE("======= CVComboListBox::OnVScroll()\n");
	if ( GetBuddyComboBox() )
	{
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		switch( nSBCode )
		{
		case SB_ENDSCROLL:
			if( (HTVSCROLL == data.nHitTest) )
			{
				// When the scrollbar goes out of ACTION, we need to 
				// gain mouse capture again. Remember to record that the scrollbar
				// is NOT in ACTION.
				data.nHitTest = 0;
				SetCapture();
			}
			break;
		}
	}
	CVComboListBoxBase::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CVComboListBox::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	VCOMBOTRACE("======= CVComboListBox::OnHScroll()\n");
	if ( GetBuddyComboBox() )
	{
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		switch( nSBCode )
		{
		case SB_ENDSCROLL:
			if( (HTHSCROLL == data.nHitTest) )
			{
				// When the scrollbar goes out of ACTION, we need to 
				// gain mouse capture again. Remember to record that the scrollbar
				// is NOT in ACTION.
				data.nHitTest = 0;
				SetCapture();
			}
			break;
		}
	}
	CVComboListBoxBase::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CVComboListBox::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	VCOMBOTRACE("======= CVComboListBox::OnKeyDown()\n");
	CVComboListBoxBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if ( GetBuddyComboBox() )
	{
		int nCurSel;
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		
		nCurSel = DefWindowProc(LB_GETCURSEL, 0, 0);
		if( (VK_RETURN == nChar) || (nCurSel != data.nCurSel) )
		{
			// Set the selection.
			data.nCurSel = nCurSel;
			
			// Send a VLBN_SELCHANGE notification to the parent.
			CommandNotify(VLBN_SELCHANGE);
		}
	}
}

void CVComboListBox::OnSetFocus( CWnd* pOldWnd )
{
	VCOMBOTRACE("======= CVComboListBox::OnSetFocus()\n");
	if ( GetBuddyComboBox() )
	{
		HWND hWndCombo		= GetBuddyComboBox()->m_hWnd;
		HWND hWndLoseFocus	= pOldWnd ? pOldWnd->m_hWnd : NULL;
		// If we got focus from a window other than that belonging
		// to the control, send the combobox a VLBN_SETFOCUS notification.
		if( (hWndCombo != hWndLoseFocus) && (::GetParent(hWndLoseFocus) != hWndCombo) )
		{
			CommandNotify(VLBN_SETFOCUS);
		}
		
		// The moment we get focus, set the focus to the combobox.
		GetBuddyComboBox()->SetFocus();
	}

	CVComboListBoxBase::OnSetFocus(pOldWnd);
}

LRESULT CVComboListBox::OnGetCurSel( WPARAM wParam, LPARAM lParam )
{
	VCOMBOTRACE("======= CVComboListBox::OnGetCurSel()\n");
	if ( GetBuddyComboBox() )
	{
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		return data.nCurSel;
	}
	return DefWindowProc(LB_GETCURSEL, wParam, lParam);
}

LRESULT CVComboListBox::OnSetCurSel( WPARAM wParam, LPARAM lParam )
{
	int& nNewSel = (int&)wParam;
	VCOMBOTRACE("======= CVComboListBox::OnSetCurSel(%d)\n", nNewSel);
	if ( GetBuddyComboBox() )
	{
		VIRTUALCOMBOLISTBOX_DATA& data = GetVirtualComboListBoxData();
		data.nCurSel = (nNewSel < LB_ERR) ? LB_ERR : nNewSel;
	}
	return DefWindowProc(LB_SETCURSEL, wParam, lParam);
}

CString CVComboListBox::GetItemText( UINT nItem )
{
	if ( GetBuddyComboBox() )
	{
		CVComboBox* pVComboBox = STATIC_DOWNCAST(CVComboBox, GetBuddyComboBox());
		return pVComboBox->GetItemText(nItem);
	}
	return CVComboListBoxBase::GetItemText(nItem);
}

// Notes.

/**************************** Note 1 ************************************/
// The three windows in the control - the combobox, the listbox &
// the editbox share a notion of focus as far as sending VCBN_SETFOCUS
// and VCBN_KILLFOCUS to the combo's parent is concerned.
//
// In the VCBS_DROPDOWNLIST style combobox, only the combobox can 
// get the focus, and it sends the notifications while handling 
// WM_SETFOCUS & WM_KILLFOCUS.
//
// In the VCBS_DROPDOWN style combobox, the combobox & the editbox can
// get focus, and the listbox does not. The moment it gets focus, the 
// combobox sets focus to the edit control. The focus is set to the
// edit control when it does not have the focus, and we click on 
// the combobox. The edit control itself can get focus by just 
// clicking the mouse on it. The VCBN_SETFOCUS and VCBN_KILLFOCUS 
// notifications are sent when the combobox receives the  
// EN_SETFOCUS & EN_KILLFOCUS notifications from the editbox.
// 
// The VCBS_SIMPLE is, by far, the most difficult to handle. Here all the three 
// windows can get focus. But the only window which can have focus
// is the editbox. The combobox sets focus to it the moment it gets
// focus. Clicking on the combobox also sets focus to the editbox,
// if the editbox currently does not have focus.But the listbox itself can 
// get focus by clicking on it. We promptly set focus back to the combobox,
// which, in turn, sets it back to the edit control. However, each time 
// it gets focus, the edit control sends EN_SETFOCUS to the combobox, but we
// need to send the VCBN_SETFOCUS only if the whole control gets focus
// from a different window. To overcome this problem, the listbox sends
// the combobox a VLBN_SETFOCUS when it gets focus from a different window, and the 
// combobox sends the VCBN_SETFOCUS to its parent in response to this notification.
// The editbox sends the combobox a VEN_SETFOCUS when it gets focus from a different
// window, and in response, the combobox sends the VCBN_SETFOCUS notification.
// The edit control sends the VEN_KILLFOCUS to the combobox when it loses
// focus to a different window, and in response, the combobox sends the 
// VCBN_KILLFOCUS notification. The combobox itself can get focus from
// a different window. Here again we need to send the VCBN_SETFOCUS notification.

IMPLEMENT_DYNCREATE(CVComboBox, CVComboBoxBase)

CVComboBox::CVComboBox()
	: m_bHighlighted(FALSE)
	, m_bPushed(FALSE)
	, m_bBtnHilight(FALSE)
	, m_nLockDrawCount(0)
	, m_bMouseEventsTracked(FALSE)
	, m_bDrawHotItem(TRUE)
{
	VERIFY( RegisterWindowClass() );
}

CVComboBox::~CVComboBox()
{
	
}

BEGIN_MESSAGE_MAP(CVComboBox, CVComboBoxBase)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_ENABLE()
	ON_WM_MEASUREITEM()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CANCELMODE()
	ON_MESSAGE(WM_NOTIFYFORMAT, OnNotifyFormat)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_MESSAGE(VCBM_GETCOUNT, OnGetCount)
	ON_MESSAGE(VCBM_GETCURSEL, OnGetCurSel)
	ON_MESSAGE(VCBM_GETDROPPEDSTATE, OnGetDroppedState)
	ON_MESSAGE(VCBM_GETEDITSEL, OnGetEditSel)
	ON_MESSAGE(VCBM_GETTOPINDEX, OnGetTopIndex)
	ON_MESSAGE(VCBM_LIMITTEXT, OnLimitText)
	ON_MESSAGE(VCBM_SETCOUNT, OnSetCount)
	ON_MESSAGE(VCBM_SETCURSEL, OnSetCurSel)
	ON_MESSAGE(VCBM_SETEDITSEL, OnSetEditSel)
	ON_MESSAGE(VCBM_SETTOPINDEX, OnSetTopIndex)
	ON_MESSAGE(VCBM_SHOWDROPDOWN, OnShowDropDown)

	ON_MESSAGE(WM_CDTOOLTIPCTRL_NOTIFY, OnCustomToolTipNotify)
END_MESSAGE_MAP()

BOOL CVComboBox::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	return CVComboBoxBase::Create(WC_VCOMBOBOX, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CVComboBox::CreateFromCtrl( CWnd* pParent, int nID, DWORD dwStyle /*= VCBS_DEFAULT_DROPDOWN*/ )
{
	if (!pParent || !pParent->GetSafeHwnd())
		return FALSE;
	CWnd *pCtrl = pParent->GetDlgItem(nID);
	if (!pCtrl)
		return FALSE;
	
	CRect controlRect;
	pCtrl->GetWindowRect(controlRect);
	pParent->ScreenToClient(controlRect);
	pCtrl->DestroyWindow();
	
	BOOL bRet = Create(dwStyle, controlRect, pParent, nID);
	
	return bRet;
}

BOOL CVComboBox::RegisterWindowClass()
{
	WNDCLASSEX	wc;
	BOOL		bRet = TRUE; 
	HINSTANCE	hInst = (HINSTANCE)::GetModuleHandle(NULL);
	if( !GetClassInfoEx(hInst, WC_VCOMBOBOX, &wc) )
	{
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		
		wc.cbSize			= sizeof(WNDCLASSEX);
		wc.style			= CS_PARENTDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS;
		wc.lpfnWndProc		= ::DefWindowProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= sizeof(PVIRTUALCOMBOBOX_DATA);
		wc.hInstance		= hInst;
		wc.hIcon			= NULL;
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName		= NULL;
		wc.lpszClassName	= WC_VCOMBOBOX; 
		wc.hIconSm			= NULL;
		
		bRet = (RegisterClassEx(&wc) != 0);
	}
	return bRet ;
}

int CVComboBox::GetItemCount()
{
	return (int)SendMessage(VCBM_GETCOUNT);
}

void CVComboBox::SetItemCount( int nCount )
{
	SendMessage(VCBM_SETCOUNT, (WPARAM)nCount);
}

void CVComboBox::SetDroppedVisibleItemCount( int nCount, BOOL bRepaint /*= TRUE*/ )
{
	if (GetComboListBox().GetSafeHwnd())
	{
		VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
		if ( SIMPLE != data.comboType )
		{
			int nItemHeight = GetComboListBox().GetItemHeight(0);
			if ( COMBOBOX_DROPPED_ALL_ITEMS_VISIBLE == nCount )
			{
				nCount = GetItemCount();
			}
			int nTotalHeight = nCount * nItemHeight;
			
			CRect rtClient, rtWnd;
			GetComboListBox().GetClientRect(rtClient);
			GetComboListBox().GetWindowRect(rtWnd);
			
			int nEdgeHeight = rtWnd.Height() - rtClient.Height();
			nTotalHeight += nEdgeHeight;
			AutoResizeListBox(nTotalHeight, bRepaint);
		}
	}
}

void CVComboBox::PreSubclassWindow()
{
	CVComboBoxBase::PreSubclassWindow();
	
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	if (pThreadState->m_pWndInit == NULL)
	{
		VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
		CRect rect;
		GetWindowRect(rect);
		data.nHeight	= rect.Height();
		VERIFY( OnInitCombo() );
	}
}

int CVComboBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CVComboBoxBase::OnCreate(lpCreateStruct) == -1 )
		return -1;
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	data.nHeight	= lpCreateStruct->cy;
	BOOL bInit		= OnInitCombo();
	return bInit ? 0 : -1;
}

void CVComboBox::OnDestroy()
{
	OnDestroyCombo();
	CVComboBoxBase::OnDestroy();
}

BOOL CVComboBox::OnInitCombo()
{
	DWORD					dwLBType;
	DWORD					dwLBExStyle;
	COMBOTYPE				comboType;
	DWORD					dwEditStyle;
	DWORD					dwLBStyle;
	// Check the combobox type & set it. VCBS_DROPDOWN is the default.
	DWORD					dwComboStyle = GetStyle();
	switch( dwComboStyle & 0x000f )
	{
	case VCBS_SIMPLE:
		
		comboType = SIMPLE;
		break;
		
	case VCBS_DROPDOWNLIST:
		
		comboType = DROPDOWNLIST;
		break;
		
	default:
		
		comboType = DROPDOWN;
		break;
	}

	ASSERT( !GetComboEdit().m_hWnd );		// not created at this time.
	ASSERT( !GetComboListBox().m_hWnd );	// not created at this time.

	GetComboEdit().SetBuddyComboBox(this);
	GetComboListBox().SetBuddyComboBox(this);

	// Create the edit control, if we have DROPDOWN style or 
	// SIMPLE style.
	if( (SIMPLE == comboType) || (DROPDOWN == comboType) )
	{
		dwEditStyle = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_WANTRETURN;
		if( dwComboStyle & VCBS_AUTOHSCROLL )
			dwEditStyle |= ES_AUTOHSCROLL;
		if( dwComboStyle & VCBS_OEMCONVERT )
			dwEditStyle |= ES_OEMCONVERT;
		if( dwComboStyle & VCBS_UPPERCASE )
			dwEditStyle |= ES_UPPERCASE;
		if( dwComboStyle & VCBS_LOWERCASE )
			dwEditStyle |= ES_LOWERCASE;
		if( dwComboStyle & WS_DISABLED )
			dwEditStyle |= WS_DISABLED;

		CRect rect(BORDER_WIDTH, BORDER_HEIGHT, BORDER_WIDTH, BORDER_HEIGHT);	// we will set the width & height later.
		BOOL bEditCreated = GetComboEdit().CreateEx(0, WC_VCOMBOEDITBOX, NULL, dwEditStyle, rect, this, EDITCTRL_ID);
		if ( !bEditCreated )
			return FALSE;
	}

	// Create the listbox.
	dwLBStyle = WS_CHILD | WS_VSCROLL | LBS_OWNERDRAWFIXED | LBS_NODATA | LBS_COMBOBOX | //WS_HSCROLL | 
				LBS_NOINTEGRALHEIGHT | LBS_NOTIFY;

	switch( comboType )
	{
	case SIMPLE:
		
		dwLBType = VLBS_SIMPLE;
		dwLBExStyle = WS_EX_CLIENTEDGE;
		if( dwComboStyle & WS_DISABLED )
			dwLBStyle |= WS_DISABLED;
		break;
		
	case DROPDOWN:
		
		dwLBType = VLBS_DROPDOWN;
		dwLBExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
		dwLBStyle |= WS_BORDER;
		break;
		
	case DROPDOWNLIST:
		
		dwLBType = VLBS_DROPDOWNLIST;
		dwLBExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
		dwLBStyle |= WS_BORDER;
		break;
	}
	
	if( dwComboStyle & VCBS_DISABLENOSCROLL )
		dwLBStyle |= LBS_DISABLENOSCROLL;

	CRect rect(0, 0, 0, 0);	// We will set the rectangle later.
	BOOL bLBoxCreated = GetComboListBox().CreateEx(dwLBExStyle, WC_VCOMBOLISTBOX, NULL, dwLBStyle, rect, this, LISTCTRL_ID, (LPVOID)&dwLBType);
	if ( !bLBoxCreated )
		return FALSE;
	
	// Now change the parent of the listbox to the desktop.
	if( SIMPLE != comboType )
		GetComboListBox().SetParent( GetDesktopWindow() );

	// Our initial format for the notifications to the
	// parent is based on the type of our window.
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	data.comboType			= comboType;
	data.bSendDispChange	= TRUE;
	data.nTopIndex			= 0;

	CWnd* pParent = GetParent();
	ASSERT(pParent);
	data.hFont				= (HFONT)pParent->SendMessage(WM_GETFONT);
	OnSetFont( (WPARAM)data.hFont, FALSE);

	if( SIMPLE == comboType )
		GetComboListBox().ShowWindow(SW_SHOWNORMAL);

	// Position the edit & listbox
	GetClientRect(rect);
	WINDOWPOS wndPos = {0};
	wndPos.cx = rect.Width();
	wndPos.cy = rect.Height();
	OnWindowPosChanging(&wndPos);

	return TRUE;
}

void CVComboBox::OnDestroyCombo()
{
	VCOMBOTRACE("======= CVComboBox::OnDestroyCombo()\n");
}

BOOL CVComboBox::OnEraseBkgnd( CDC* pDC )
{
	//return CVComboBoxBase::OnEraseBkgnd(pDC);
// 	CRect rect;
// 	GetClientRect(rect);
// 	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_WINDOW));
 	return TRUE;
}

void CVComboBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = &dc;

	CRect		rc;
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	// Draw the border rect.
	if( SIMPLE == data.comboType )
	{
		GetComboEdit().GetWindowRect(rc);
		ScreenToClient(rc);
		rc.left -= BORDER_WIDTH;
		rc.top -= BORDER_HEIGHT;
		rc.right += BORDER_WIDTH;
		rc.bottom += BORDER_HEIGHT;
		
		OnFillBackground(pDC, rc);
		OnDrawBorder(pDC, rc);
		return;
	}
	
	GetClientRect(rc);

	CRect rcBtn = GetButtonDrawRect();
	
	// Draw the text if there is no edit control.
	BOOL bDrawText		= DROPDOWNLIST == data.comboType;
#ifdef CUSTOMDRAW_GRADIENT
	if ( !bDrawText )
#endif // CUSTOMDRAW_GRADIENT
	{
		OnFillBackground(pDC, rc);
		OnDrawBorder(pDC, rc);
		
		CSimpleMemDC memDC(*pDC, rcBtn, OUTPUTTYPE_TRANSPARENTBLT);
		CDC* pDrawDC = &memDC.GetDC();
		memDC.InitTransparentBackground();
		
		OnDrawButton(pDrawDC, rcBtn);
	}
	if( bDrawText )
	{
		CRect	rcClip;
		SetRect(&rcClip, BORDER_WIDTH, BORDER_HEIGHT,
			rcBtn.left - BORDER_WIDTH, rc.bottom - BORDER_HEIGHT);
	
		OnDrawItem(pDC, rcClip);
	}
}

void CVComboBox::OnFillBackground( CDC* pDC, const CRect& rect )
{
	pDC->FillSolidRect(rect, RGB(255,255,255));
}

void CVComboBox::OnDrawBorder( CDC* pDC, CRect& rect )
{
#ifdef CUSTOMDRAW_GRADIENT
	COLORREF clrBordder;
	if ( IsWindowEnabled() )
	{
		clrBordder = m_bHighlighted ? RGB_COMBO_BORDER_HILIGHT : RGB_COMBO_BORDER_NORMAL;
	}
	else
		clrBordder = ::GetSysColor(COLOR_GRAYTEXT);
	
	CBrush brush(clrBordder);
	pDC->FrameRect(rect, &brush);
#else
	pDC->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);
#endif // CUSTOMDRAW_GRADIENT
}

CRect CVComboBox::GetButtonDrawRect()
{
	CRect rc;
	GetClientRect(rc);
#ifdef CUSTOMDRAW_GRADIENT
	rc.left = rc.right - SCROLLBUTTON_WIDTH - 1;
#else
	rc.left += rc.right - BORDER_WIDTH - SCROLLBUTTON_WIDTH + 1;
	rc.right -= BORDER_WIDTH - 1;
	rc.top  += BORDER_HEIGHT - 1;
	rc.bottom -= BORDER_HEIGHT - 1;
#endif // CUSTOMDRAW_GRADIENT
	return rc;
}

void CVComboBox::OnDrawButton( CDC* pDC, CRect& rc )
{
#ifdef CUSTOMDRAW_GRADIENT
	CCustomDrawHelper drawer;
	drawer.m_bWndEnabled		= !!IsWindowEnabled();
	drawer.m_bWndHasFocus		= ::GetFocus() == GetSafeHwnd();
	drawer.m_bFocusItem			= false;
	drawer.m_bIsHotItem			= m_bBtnHilight ? true : false;
	drawer.m_bIsPressed			= m_bPushed ? true : false;
	drawer.m_hWnd				= m_hWnd;
	drawer.DrawThemeBackground(pDC, rc, CDTBT_COMBODOWNBUTTON);
#else
	int	nStatusFlag = IsWindowEnabled() ? 0 : DFCS_INACTIVE;
	pDC->DrawFrameControl(&rc, DFC_SCROLL, DFCS_SCROLLDOWN | nStatusFlag);
#endif // CUSTOMDRAW_GRADIENT
}

void CVComboBox::OnDrawItem( CDC* pDC, CRect& rcClip)
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	CFont* pFont = CFont::FromHandle(data.hFont);
	CFont* pOldFont = pDC->SelectObject(pFont);

	ASSERT( DROPDOWNLIST == data.comboType );
	
#ifdef CUSTOMDRAW_GRADIENT
	CCustomDrawHelper drawer;
	drawer.m_bWndEnabled		= !!IsWindowEnabled();
	drawer.m_bWndHasFocus		= ::GetFocus() == GetSafeHwnd();
	drawer.m_bFocusItem			= m_bHighlighted ? true : false;
	drawer.m_bIsHotItem			= m_bHighlighted ? true : false;
	drawer.m_bIsPressed			= m_bPushed ? true : false;
	drawer.m_hWnd				= m_hWnd;
	CRect rectClient;
	GetClientRect(rectClient);
	drawer.DrawThemeBackground(pDC, rectClient, CDTBT_COMBOBUTTON);

	DrawDropDownArrow(pDC->GetSafeHdc(), GetButtonDrawRect());

	if( !IsWindowEnabled() )
	{
		CBrush br(GetSysColor(COLOR_BTNFACE));
		pDC->SetBkColor(GetSysColor(COLOR_BTNFACE));
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->FillRect(rcClip, &br);
	}
#else
	if( !IsWindowEnabled() )
	{
		CBrush br(GetSysColor(COLOR_BTNFACE));
		pDC->SetBkColor(GetSysColor(COLOR_BTNFACE));
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->FillRect(rcClip, &br);
	}
	else if( ::GetFocus() == m_hWnd )
	{
		CBrush br(GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->FillRect(rcClip, &br);
		
		//pDC->DrawFocusRect(rcClip);
		
		//rcClip.top++;
		//rcClip.left++;
		//rcClip.bottom--;
		//rcClip.right--;
	}
	else
	{
		CBrush br(GetSysColor(COLOR_WINDOW));
		pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pDC->FillRect(rcClip, &br);
	}
#endif // CUSTOMDRAW_GRADIENT
	
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);

	const UINT nDTFormat = DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS;

	int iItemSelected = GetCurSel();
	if(iItemSelected >= 0)
	{
		CString strText = GetItemText(iItemSelected);

		if ( GetTextDrawer() )
			GetTextDrawer()->Draw(pDC, strText, rcClip, nDTFormat);
		else
			pDC->DrawText(strText, rcClip, nDTFormat);
	}
		
	pDC->SetBkMode(nOldBkMode);
	
	pDC->SelectObject(pOldFont);
}

void CVComboBox::OnMouseMove( UINT nFlags, CPoint point )
{
	if ( m_bDrawHotItem )
	{
		if ( !m_bMouseEventsTracked )
		{
			m_bMouseEventsTracked = TRUE;
			
			TRACKMOUSEEVENT trackmouseevent;
			trackmouseevent.cbSize		= sizeof(trackmouseevent);
			trackmouseevent.dwFlags		= TME_LEAVE;
			trackmouseevent.hwndTrack	= GetSafeHwnd();
			trackmouseevent.dwHoverTime	= HOVER_DEFAULT;
			//::AFXTrackMouse(&trackmouseevent);
			_TrackMouseEvent(&trackmouseevent);
		}

	/*	BOOL bRedraw	= FALSE;
		
		CRect rcBtn = GetButtonDrawRect();
		
		CRect rectClient;
		GetClientRect(rectClient);
		ASSERT( rectClient.PtInRect(point) );
		if ( !m_bPushed )
		{
			if (!m_bHighlighted)
			{
				m_bHighlighted = TRUE;
				bRedraw = TRUE;
			}

			BOOL bInBtn = rcBtn.PtInRect(point);

			if ( bInBtn && !m_bBtnHilight )
			{
				m_bBtnHilight = TRUE;
				bRedraw = TRUE;
			}
		}
		
		if (bRedraw)
		{
			Invalidate();
			UpdateWindow();
		}*/
	}
	CVComboBoxBase::OnMouseMove(nFlags, point);
}

LRESULT CVComboBox::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
	LRESULT result = Default();
	m_bMouseEventsTracked = FALSE;
	
	/*if ( m_bDrawHotItem && (m_bHighlighted || m_bBtnHilight) )
	{
		CRect rc;
		GetClientRect(rc);
		const MSG* pMsg = GetCurrentMessage();
		CPoint pt = pMsg->pt;
		m_bHighlighted = rc.PtInRect(pt);
		m_bBtnHilight = GetButtonDrawRect().PtInRect(pt);
		Invalidate();
		UpdateWindow();
	}*/
	
	return result;
}

int CVComboBox::GetCurSel()
{
	return (int)SendMessage(VCBM_GETCURSEL);
}

int CVComboBox::SetCurSel( int nSelect )
{
	int nSel = (int)SendMessage(VCBM_SETCURSEL, (WPARAM)nSelect);
	
	return nSel;
}

int CVComboBox::GetTopIndex()
{
	return (int)SendMessage(VCBM_GETTOPINDEX);
}

int CVComboBox::SetTopIndex( int nIndex )
{
	return (int)SendMessage(VCBM_SETTOPINDEX, (WPARAM)nIndex);
}

void CVComboBox::SetHorizontalExtent( UINT nExtent )
{
	GetComboListBox().SetHorizontalExtent(nExtent);
}

UINT CVComboBox::GetHorizontalExtent()
{
	return GetComboListBox().GetHorizontalExtent();;
}

int CVComboBox::SetDroppedWidth( UINT nWidth )
{
	// TODO
	return 0;
}

int CVComboBox::GetDroppedWidth()
{
	// TODO
	return 0;
}

DWORD CVComboBox::GetEditSel()
{
	return (DWORD)SendMessage(VCBM_GETEDITSEL);
}

BOOL CVComboBox::SetEditSel( int nStartChar, int nEndChar )
{
	return (BOOL)SendMessage(VCBM_SETEDITSEL, (WPARAM)nStartChar, (LPARAM)nEndChar);
}

DWORD CVComboBox::GetItemData( int nIndex )
{
	return GetComboListBox().GetItemData(nIndex);
}

int CVComboBox::SetItemData( int nIndex, DWORD dwItemData )
{
	return GetComboListBox().SetItemData(nIndex, dwItemData);
}

void* CVComboBox::GetItemDataPtr( int nIndex )
{
	return GetComboListBox().GetItemDataPtr(nIndex);
}

int CVComboBox::SetItemHeight( UINT cyItemHeight )
{
	return GetComboListBox().SetItemHeight(0, cyItemHeight);
}

int CVComboBox::GetItemHeight()
{
	return GetComboListBox().GetItemHeight(0);
}

int CVComboBox::SetExtendedUI( BOOL bExtended /*= TRUE*/ )
{
	// TODO
	return 0;
}

BOOL CVComboBox::GetExtendedUI()
{
	return FALSE;
}

BOOL CVComboBox::GetDroppedState()
{
	return GetComboListBox().IsWindowVisible();
}

BOOL CVComboBox::GetDroppedControlRect( LPRECT lprect )
{
	return SendMessage(VCBM_GETDROPPEDCONTROLRECT, 0, (LPARAM)lprect);
}

void CVComboBox::ShowDropDown( BOOL bShowIt /*= TRUE*/ )
{
	SendMessage(VCBM_SHOWDROPDOWN, (WPARAM)bShowIt);
}

LRESULT CVComboBox::OnGetFont( WPARAM wParam, LPARAM lParam )
{
	VCOMBOTRACE("======= CVComboBox::OnGetFont()\n");
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	return (LRESULT)data.hFont;
}

LRESULT CVComboBox::OnSetFont( WPARAM wParam, LPARAM lParam )
{
	VCOMBOTRACE("======= CVComboBox::OnSetFont()\n");
	HFONT& hFont = (HFONT&)wParam;
	BOOL& bRedraw = (BOOL&)lParam;

	CRect		rcClient;
	
	// When the font is set, recompute the combobox size, & set the 
	// font for the edit control & the listbox.
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	GetWindowRect(rcClient);
	GetParent()->ScreenToClient(rcClient);
	
	if( (DROPDOWN == data.comboType)  || (SIMPLE == data.comboType) )
		GetComboEdit().SendMessage(WM_SETFONT, wParam, lParam);
	
	GetComboListBox().SendMessage(WM_SETFONT, wParam, lParam);
	
	// Set the height of each item in the listbox.
	HDC hDC = ::GetDC(m_hWnd);
	HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);
	TEXTMETRIC tm = {0};
	GetTextMetrics(hDC, &tm);
	SelectObject(hDC, hFontOld);
	::ReleaseDC(m_hWnd, hDC);
	
	GetComboListBox().SetItemHeight(0, tm.tmHeight + 4);
	
	// Always resize so that we specify the total height of the combobox.
	MoveWindow(rcClient.left, rcClient.top, rcClient.Width(), data.nHeight, TRUE);

	DefWindowProc(WM_SETFONT, wParam, lParam);

	return 0;
}

void CVComboBox::OnLButtonDown( UINT nFlags, CPoint point )
{
	VCOMBOTRACE("======= CVComboBox::OnLButtonDown()\n");

	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();

	CRect rcHitTest = GetDropDownHittestRect();
	BOOL bShowLB = FALSE;
	
	switch( data.comboType )
	{
	case SIMPLE:
		
		if( ::GetFocus() != GetComboEdit().m_hWnd )
			SetFocus();
		break;
		
	case DROPDOWN:
		if( ::GetFocus() != GetComboEdit().m_hWnd )
			SetFocus();
		
		// Clicking on the arrow button should show/hide the listbox.
		if ( rcHitTest.PtInRect(point) )
		{
			// Show / hide the listbox.
			bShowLB = !GetComboListBox().IsWindowVisible();
			ShowListBox( bShowLB );
			if ( !bShowLB )
				CommandNotify(VCBN_SELENDCANCEL);
		}
		break;
		
	case DROPDOWNLIST:
		if( ::GetFocus() != m_hWnd )
			SetFocus();
		
		// Clicking anywhere on the combobox should show/hide the listbox.
		if ( rcHitTest.PtInRect(point) )
		{
			// Show / hide the listbox.
			bShowLB = !GetComboListBox().IsWindowVisible();
			ShowListBox( bShowLB );
			if ( !bShowLB )
				CommandNotify(VCBN_SELENDCANCEL);
		}
		break;
	}

	/*if ( data.comboType != SIMPLE && bShowLB )
	{
		m_bPushed		= TRUE;
		m_bHighlighted	= TRUE;
		m_bBtnHilight	= TRUE;
		
		Invalidate();
		UpdateWindow();
	}*/

	//CVComboBoxBase::OnLButtonDown(nFlags, point);
}

void CVComboBox::OnLButtonUp( UINT nFlags, CPoint point )
{
	//BOOL bClicked	= m_bPushed && m_bHighlighted;
	//
	//m_bPushed		= FALSE;
	//CRect rcCLient;
	//GetClientRect(rcCLient);
	//m_bHighlighted	= rcCLient.PtInRect(point);
	//
	//HWND hWnd = GetSafeHwnd();
	//
	//if (!::IsWindow(hWnd))
	//{
	//	// The button was destroyed after send message
	//	return;
	//}
	//	
	//RedrawWindow();
	//
	////CVComboBoxBase::OnLButtonUp(nFlags, point);
	//
	//if (!::IsWindow(hWnd))
	//{
	//	// The button was destroyed after the OnLButtonUp handler
	//	return;
	//}
}

BOOL CVComboBox::OnCommand( WPARAM wParam, LPARAM lParam )
{
	int nID			= LOWORD(wParam);
	UINT codeNotify	= HIWORD(wParam);
	HWND hWndCtrl	= (HWND)lParam;
	VCOMBOTRACE("======= CVComboBox::OnCommand(%d, %d)\n", nID, codeNotify);
	switch( nID )
	{
	case EDITCTRL_ID:
		HandleEditNotifications(codeNotify);
		break;
		
	case LISTCTRL_ID:	
		HandleListBoxNotifications(codeNotify);
		break;
		
	default:
		break;
	}
	return TRUE;
}

void CVComboBox::OnSetFocus( CWnd* pOldWnd )
{
	VCOMBOTRACE("======= CVComboBox::OnSetFocus()\n");
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	HWND hWndLoseFocus = pOldWnd ? pOldWnd->m_hWnd : NULL;
	
	switch( data.comboType )
	{
	case SIMPLE:
		// Set focus to the edit control.
		GetComboEdit().SetFocus();
		
		// If we got focus from a different window, send a VCBN_SETFOCUS notification.
		if( (hWndLoseFocus != GetComboListBox().m_hWnd) && (hWndLoseFocus != GetComboEdit().m_hWnd) )
			CommandNotify(VCBN_SETFOCUS);
		break;
		
	case DROPDOWN:		
		// Set focus to the edit control.
		GetComboEdit().SetFocus();
		break;
		
	case DROPDOWNLIST:
		// Send the VCBN_SETFOCUS notification.
		CommandNotify(VCBN_SETFOCUS);
		
		// Redraw
		InvalidateRect(NULL, TRUE);
		break;
	}
}

void CVComboBox::OnKillFocus( CWnd* pNewWnd )
{
	VCOMBOTRACE("======= CVComboBox::OnKillFocus()\n");
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	switch( data.comboType )
	{
	case SIMPLE:
		break;		
	case DROPDOWN:
		break;		
	case DROPDOWNLIST:
		// Hide the listbox, if necessary.
		ShowListBox(FALSE);
		CommandNotify(VCBN_SELENDCANCEL);
		
		// Send the VCBN_KILLFOCUS notification.
		CommandNotify(VCBN_KILLFOCUS);
		
		InvalidateRect(NULL, TRUE);
		break;
	}
	
	m_bHighlighted	= FALSE;
	
	Invalidate();
	UpdateWindow();
}

void CVComboBox::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	VCOMBOTRACE("======= CVComboBox::OnKeyDown()\n");
	int& nVirtKey = (int&)nChar;
	LONG lKeyData = MAKELPARAM(nRepCnt, nFlags);
	int	nCurSel;
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	switch( nChar )
	{
	case VK_F4:
		// Show/hide the listbox.
		if( SIMPLE != data.comboType )
		{
			BOOL bShowLB = !GetComboListBox().IsWindowVisible();
			ShowListBox( bShowLB );
			if ( !bShowLB )
				CommandNotify(VCBN_SELENDCANCEL);
		}
		break;
		
	case VK_ESCAPE:
		// Check the last valid selection & reset the selection.
		nCurSel = GetCurSel();
		SetCurSel(nCurSel);
		ModifyText();
		
		// Hide the listbox.
		ShowListBox(FALSE);
		CommandNotify(VCBN_SELENDCANCEL);
		break;
		
	case VK_RETURN:
		GetComboListBox().SendMessage(WM_KEYDOWN, nVirtKey, lKeyData);
		
		// Hide the listbox.
		ShowListBox(FALSE);
		break;
		
	case VK_DOWN:
	case VK_UP:
		if( DROPDOWNLIST == data.comboType )
		{
			GetComboListBox().SendMessage(WM_KEYDOWN, nVirtKey, lKeyData);
			break;
		}
		
		nCurSel = GetCurSel();
		if( data.nTopIndex != nCurSel )
		{
			SetCurSel(data.nTopIndex);
			ModifyText();
			CommandNotify(VCBN_SELCHANGE);
			break;
		}
		else
		{
			GetComboListBox().SendMessage(WM_KEYDOWN, nVirtKey, lKeyData);
			nCurSel = GetCurSel();
			data.nTopIndex = nCurSel;
			VCOMBOTRACE("======= CVComboBox::update 1 TopIndex(%d)\n", nCurSel);
		}
		break;
		
	case VK_NEXT:
	case VK_PRIOR:
		
		if( DROPDOWNLIST == data.comboType )
		{
			GetComboListBox().SendMessage(WM_KEYDOWN, nVirtKey, lKeyData);
			break;
		}
		
		SetCurSel(data.nTopIndex);
		GetComboListBox().SendMessage(WM_KEYDOWN, nVirtKey, lKeyData);
		nCurSel = GetCurSel();
		data.nTopIndex = nCurSel;
		VCOMBOTRACE("======= CVComboBox::update 2 TopIndex(%d)\n", nCurSel);
		break;
		
	default:
		
		break;
	}
}

void CVComboBox::OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	VCOMBOTRACE("======= CVComboBox::OnSysKeyDown()\n");
	switch( nChar )
	{
	case VK_DOWN:
	case VK_UP:
		{
			LONG lKeyData = MAKELPARAM(nRepCnt, nFlags);
			SendMessage(WM_KEYDOWN, VK_F4, lKeyData);
		}
		break;
	}
}

UINT CVComboBox::OnGetDlgCode()
{
	UINT uRet = 0;
	// We need the messages if the listbox is visible.
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();	
	
	switch( data.comboType )
	{
	case SIMPLE:		
		break;
		
	case DROPDOWN:
		if( GetComboListBox().IsWindowVisible() )
			uRet = DLGC_WANTMESSAGE;
		break;
		
	case DROPDOWNLIST:	
		{
			const MSG* pCurMsg = GetCurrentMessage();
			if( pCurMsg != NULL )
			{
				PMSG pMsg = (PMSG)pCurMsg->lParam;
				if ( pMsg != NULL )
				{
					if( (WM_KEYDOWN == pMsg->message) && ((VK_ESCAPE == pMsg->wParam) || (VK_RETURN == pMsg->wParam)) 
						&& GetComboListBox().IsWindowVisible() )
						uRet = DLGC_WANTMESSAGE;
					else
						uRet = DLGC_WANTARROWS | DLGC_WANTCHARS;
				}
			}
		}
		break;
	}
	
	return uRet;
}

void CVComboBox::OnWindowPosChanging( WINDOWPOS* pWndPos )
{
	// Set the combobox height based on the current font.
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();

	HDC			hDC;
	TEXTMETRIC	tm;
	HFONT		hFontOld;
	int			nHeight;
	int			nListHeight;
	int			nItemHeight;
	int			nVisible;
	CRect		rcItem;
	int			nBorderHt = GetSystemMetrics((SIMPLE == data.comboType) ? SM_CYEDGE : SM_CYBORDER);
	
	hDC = ::GetDC(m_hWnd);
	hFontOld = (HFONT)SelectObject(hDC, data.hFont);
	GetTextMetrics(hDC, &tm);
	SelectObject(hDC, hFontOld);
	::ReleaseDC(m_hWnd, hDC);
	
	// The height of our combobox is determined by the font. 
	nHeight = tm.tmHeight + 2 + 2 * BORDER_WIDTH;
	
	// Resize the edit control.
	if( SIMPLE == data.comboType )
	{
		// There is no down-arrow button.
		GetComboEdit().MoveWindow(
			BORDER_WIDTH, 
			BORDER_HEIGHT, 
			pWndPos->cx - 2 * BORDER_WIDTH,
			nHeight - 2 * BORDER_WIDTH, 
			FALSE);
	}
	else if( DROPDOWN == data.comboType )
	{
		// The down-arrow button is present. The edit control is smaller.
		GetComboEdit().MoveWindow(
			BORDER_WIDTH, 
			BORDER_HEIGHT, 
			pWndPos->cx - (2 * BORDER_WIDTH + SCROLLBUTTON_WIDTH),
			nHeight - 2 * BORDER_WIDTH, 
			FALSE);
	}
	
	// Resize the listbox.
	
	// Change the width & height of the listbox.
	// Ensure that no item is partially visible, by adjusting the
	// height accordingly.
	
	if( LB_ERR != GetComboListBox().GetItemRect(0, rcItem) )
		nItemHeight = rcItem.bottom - rcItem.top;
	else
		nItemHeight = tm.tmHeight + 4;
	
	nListHeight = pWndPos->cy - nHeight - 2 * nBorderHt;
	nVisible = (int)(((float)nListHeight/nItemHeight) + 0.5);
	nListHeight = nVisible * nItemHeight + 2 * nBorderHt;
	
	
	// Save the combobox height.
	data.nHeight = nListHeight + nHeight;
	
	if( (DROPDOWN == data.comboType) || (DROPDOWNLIST == data.comboType) )
	{
		// First hide the listbox.
		ShowListBox(FALSE);
		CommandNotify(VCBN_SELENDCANCEL);
		GetComboListBox().MoveWindow(0, 0, pWndPos->cx, nListHeight, FALSE);
		
		// Set the button rectangle.
		/*
		SetRect(
			&data.rcButton, 
			pWndPos->cx - (BORDER_WIDTH + SCROLLBUTTON_WIDTH),
			BORDER_HEIGHT,
			pWndPos->cx - BORDER_WIDTH,
			nHeight - BORDER_HEIGHT
			);
		*/
		
		pWndPos->cy = nHeight;
	}
	else	// SIMPLE type.
	{
		pWndPos->cy = data.nHeight;
		GetComboListBox().MoveWindow(0, nHeight, pWndPos->cx, nListHeight, FALSE);
	}
}

void CVComboBox::OnEnable( BOOL bEnable )
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	// Hide the listbox.
	if( (DROPDOWN == data.comboType) || (DROPDOWNLIST == data.comboType) )
	{
		ShowListBox(FALSE);
		CommandNotify(VCBN_SELENDCANCEL);
	}
	else
	{
		// Disable/enable listbox.
		GetComboListBox().EnableWindow(bEnable);
	}
	
	if (DROPDOWNLIST != data.comboType )
	{
		// Enable/disable the edit box.
		GetComboEdit().EnableWindow(bEnable);
	}
	
	// Redraw.
	InvalidateRect(NULL, TRUE);
}

LRESULT CVComboBox::OnNotifyFormat( WPARAM wParam, LPARAM lParam )
{
	HWND hWndFrom	= (HWND)wParam;
	int nCommand	= (int)lParam;
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	// TODO
	/*
	if( NF_QUERY == nCommand )
	{
		return data.bUnicode ? NFR_UNICODE : NFR_ANSI;
	}
	else if( NF_REQUERY == nCommand )
	{
		// The parent is asking us to query it again.
		int	iFormat = SendMessage(data.hWndParent, WM_NOTIFYFORMAT, (WPARAM)hWnd, NF_QUERY);
		
		// Set the status of our notifications, based on the above return
		// value.
		data.bUnicode = (NFR_UNICODE == iFormat);
	}
	*/
	
	return 0;
}

LRESULT CVComboBox::OnGetText( WPARAM wParam, LPARAM lParam )
{
	int cchTextMax = (int)wParam;
	int nResult;
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	if( (SIMPLE == data.comboType) || (DROPDOWN == data.comboType) )
	{
		nResult = GetComboEdit().GetWindowText((LPTSTR)lParam, cchTextMax);
	}
	else
	{
		nResult = DefWindowProc(WM_GETTEXT, wParam, lParam);
	}
	
	return nResult;
}

LRESULT CVComboBox::OnGetTextLength( WPARAM wParam, LPARAM lParam )
{
	int nResult;
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	if( (SIMPLE == data.comboType) || (DROPDOWN == data.comboType) )
	{
		nResult = ::GetWindowTextLength(GetComboEdit());
	}
	else
	{
		nResult = DefWindowProc(WM_GETTEXTLENGTH, 0, 0);
	}
	
	return nResult;
}

LRESULT CVComboBox::OnGetCount( WPARAM wParam, LPARAM lParam )
{
	int	nCount = GetComboListBox().GetCount();
	return (LB_ERR == nCount) ? VCB_ERR : nCount;
}

LRESULT CVComboBox::OnGetCurSel( WPARAM wParam, LPARAM lParam )
{
	int nCurSel = GetComboListBox().GetCurSel();
	
#ifdef _DEBUG
	int nSel = DefWindowProc(LB_GETCURSEL, 0, 0);
#endif // _DEBUG

	return (LB_ERR == nCurSel) ? VCB_ERR : nCurSel;
}

LRESULT CVComboBox::OnGetDroppedState( WPARAM wParam, LPARAM lParam )
{
	return GetComboListBox().IsWindowVisible();
}

LRESULT CVComboBox::OnGetDroppedControlRect( WPARAM wParam, LPARAM lParam )
{
	LPRECT pRect = (LPRECT)lParam;
	return ::GetWindowRect(GetComboListBox().GetSafeHwnd(), pRect);
}

LRESULT CVComboBox::OnGetEditSel( WPARAM wParam, LPARAM lParam )
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	DWORD* pdwStart = (DWORD*)wParam;
	DWORD* pdwEnd	= (DWORD*)lParam;
	
	if( DROPDOWNLIST == data.comboType )
	{
		if( pdwStart )
			*pdwStart = 0;
		if( pdwEnd )
			*pdwEnd = 0;
		return -1;
	}
	
	return GetComboEdit().SendMessage(EM_GETSEL, wParam, lParam);
}

LRESULT CVComboBox::OnGetTopIndex( WPARAM wParam, LPARAM lParam )
{
	int nItem = GetComboListBox().GetTopIndex();
	return (LB_ERR == nItem) ? VCB_ERR : nItem;
}

LRESULT CVComboBox::OnLimitText( WPARAM wParam, LPARAM lParam )
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	if ( data.comboType != DROPDOWNLIST )
	{
		GetComboEdit().LimitText(wParam);
	}
	return TRUE;
}

LRESULT CVComboBox::OnSetText( WPARAM wParam, LPARAM lParam )
{
	int nResult;
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	if( (SIMPLE == data.comboType) || (DROPDOWN == data.comboType) )
	{
		nResult = ::SetWindowText(GetComboEdit().m_hWnd, (LPCTSTR)lParam);
	}
	else
	{
		nResult = DefWindowProc(WM_SETTEXT, wParam, lParam);
	}
	
	return nResult;
}

LRESULT CVComboBox::OnSetCount( WPARAM wParam, LPARAM lParam )
{
#ifdef _DEBUG
	int nCurCount = GetComboListBox().GetItemCount();
	if ( nCurCount >= (int)wParam )
	{
		//return 0;
	}
#endif // _DEBUG
	return (LB_ERR == GetComboListBox().SetItemCount(wParam)) ? VCB_ERR : 0;
}

LRESULT CVComboBox::OnSetCurSel( WPARAM wParam, LPARAM lParam )
{
	int nSel = GetComboListBox().SetCurSel((int)wParam);
	// Change the combo/edit text.
	ModifyText();
	return (LB_ERR == nSel)? VCB_ERR : nSel;
}

LRESULT CVComboBox::OnSetEditSel( WPARAM wParam, LPARAM lParam )
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	if( DROPDOWNLIST == data.comboType )
		return VCB_ERR;
	int ichStart	= LOWORD(lParam);
	int ichEnd		= HIWORD(lParam);
	GetComboEdit().SetSel(ichStart, ichEnd);
	return TRUE;
}

LRESULT CVComboBox::OnSetTopIndex( WPARAM wParam, LPARAM lParam )
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	data.nTopIndex = (int)wParam;
	return (LB_ERR == GetComboListBox().SetTopIndex(wParam)) ? VCB_ERR : 0;
}

LRESULT CVComboBox::OnShowDropDown( WPARAM wParam, LPARAM lParam )
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	BOOL& bShowIt = (BOOL&)wParam;
	
	if( SIMPLE != data.comboType )
	{
		ShowListBox(bShowIt);
		if ( !bShowIt )
			CommandNotify(VCBN_SELENDCANCEL);
	}
	return TRUE;
}

CString CVComboBox::GetItemText( UINT nItem )
{
	if (GetComboListBox().GetSafeHwnd())
	{
		CString strText;
		GetComboListBox().GetText(nItem, strText);
		return strText;
	}
	return _T("");
}

int CVComboBox::GetItemIconIndex( UINT nItem )
{
	if (GetComboListBox().GetSafeHwnd())
	{
		return GetComboListBox().GetItemIconIndex(nItem);
	}
	return -1;
}

void CVComboBox::PreAddToolTipForItem( int nItem )
{
	// nothing to do
}

void CVComboBox::PreShowToolTipForItem( int nItem )
{
	// nothing to do
}

void CVComboBox::PrePopToolTip()
{
	// nothing to do
}

void CVComboBox::SetTextOfItem( int nIndex )
{
	if ( nIndex >= 0 )
	{
		SetCurSel(nIndex);
		ModifyText();
		CommandNotify(VCBN_EDITCHANGE);
	}
}

LRESULT CVComboBox::OnCustomToolTipNotify( WPARAM wParam, LPARAM lParam )
{
	switch (wParam)
	{
	case CDTOOLTIP_ONBEFORE_SHOW:
		{
			int nItem = (int)GetCustomDrawToolTips().GetDisplayWParam();
			PreShowToolTipForItem(nItem);
		}
		break;
	case CDTOOLTIP_ONBEFORE_POP:
		PrePopToolTip();
		break;
	}
	return 0;
}

void CVComboBox::AutoResizeListBox( UINT nListHeight /*= 0*/, BOOL bRepaint /*= FALSE*/ )
{
	CRect	rcListBox;
	CRect	rcScreen;
	
	SystemParametersInfo(SPI_GETWORKAREA, 0, (void *)&rcScreen, 0);

	if ( 0 == nListHeight )
	{
		GetComboListBox().GetWindowRect(rcListBox);
		nListHeight = rcListBox.Height();
	}
	
	CRect	rcCombo;
	GetWindowRect(rcCombo);

	rcListBox.left		= rcCombo.left;
	rcListBox.right		= rcCombo.right;

	// First let's try to put the listbox underneath the combo
	rcListBox.top		= rcCombo.bottom + 1;
	rcListBox.bottom	= rcListBox.top + nListHeight;
	
	// Now check to see if this will make the bottom portion of listbox display outside the screen
	// then to consider placing the listbox above the combo
	if( (rcListBox.top + nListHeight) > (UINT)rcScreen.bottom )
	{
		if ( rcCombo.top < (rcScreen.Height() / 2) )
		{
			rcListBox.bottom = rcScreen.bottom;
		}
		else
		{
			// If we comes here, then we can put the listbox above the combo
			rcListBox.bottom	= rcCombo.top - 1;
			rcListBox.top		= rcListBox.bottom - nListHeight;
			
			if( rcListBox.top < 0 )
			{
				rcListBox.top = 0;
			}
		}
	}
	
	GetComboListBox().MoveWindow(rcListBox, bRepaint);
}

BOOL CVComboBox::IsListBoxShowAbove()
{
	CRect rect;
	GetComboListBox().GetWindowRect(rect);
	ScreenToClient(rect);
	return rect.top < 0;
}

BOOL CVComboBox::ShowListBox( BOOL bShow /*= TRUE*/ )
{
	//VCOMBOTRACE("======= CVComboBox::ShowListBox(%d)\n", bShow);
	if( bShow )
	{
		AutoResizeListBox();

		// Send the VCBN_DROPDOWN notification to the parent.
		CommandNotify(VCBN_DROPDOWN);

#ifdef ANIMATE_LISTBOX_SHOWUP
		::OAnimateWindow( GetComboListBox(), 200, AW_SLIDE|AW_ACTIVATE|(IsListBoxShowAbove() ? AW_VER_NEGATIVE : AW_VER_POSITIVE) );
		// I have to send this message to the listbox because otherwise it won't capture the mouse event.
		GetComboListBox().PostMessage(WM_SHOWWINDOW, bShow);
#else		
		GetComboListBox().ShowWindow(SW_SHOWNORMAL);
#endif // ANIMATE_LISTBOX_SHOWUP
	}
	else
	{
		VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData(); 
		// Hiding the listbox will cause capture change, which will lead the listbox sends the VLBN_CLOSEUP notification 
		// to this combobox, so before this we need to reset this flag to prevent redundant notification.
		data.bNotifySelEnd = FALSE;
		GetComboListBox().ShowWindow(SW_HIDE);
		data.bNotifySelEnd = TRUE;
	}

	m_bBtnHilight	= bShow;
	m_bPushed		= bShow;
	Invalidate();
	UpdateWindow();
	return TRUE;
}

void CVComboBox::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	if (GetComboListBox().GetSafeHwnd())
	{
		GetComboListBox().MeasureItem(lpMeasureItemStruct);
	}
}

void CVComboBox::CommandNotify( int nCode )
{
	::CommandNotify(GetParent()->m_hWnd, nCode, m_hWnd);
}

void CVComboBox::HandleEditNotifications( UINT codeNotify )
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	switch( codeNotify )
	{
	case EN_SETFOCUS:
		// Select the entire text in the edit control.
		GetComboEdit().SetSel(0, -1);
		
		// Send the parent the VCBN_SETFOCUS notification.
		if( DROPDOWN == data.comboType )
			CommandNotify(VCBN_SETFOCUS);
		break;
		
	case EN_KILLFOCUS:
		// The edit control has lost focus. Hide the listbox, if necessary.
		if( DROPDOWN == data.comboType ) 
		{
			ShowListBox(FALSE);
			CommandNotify(VCBN_SELENDCANCEL);
		}
		
		// Hide the entire selection.
		GetComboEdit().SetSel(0, 0);
		
		// Send a VCBN_KILLFOCUS notification.
		if( DROPDOWN == data.comboType )
			CommandNotify(VCBN_KILLFOCUS);
		break;
		
	case EN_CHANGE:
		// Send the VCBN_DISPCHANGE notification.
		HandleDispChange();
		
		// Send the VCBN_EDITCHANGE notification.
		CommandNotify(VCBN_EDITCHANGE);
		break;
		
	case EN_UPDATE:
		// Send the VCBN_EDITUPDATE notification.
		CommandNotify(VCBN_EDITUPDATE);
		break;
		
	case VEN_SETFOCUS:
		// Send a VCBN_SETFOCUS notification.
		if( SIMPLE == data.comboType )
			CommandNotify(VCBN_SETFOCUS);
		break;
		
	case VEN_KILLFOCUS:
		// Send a VCBN_KILLFOCUS notification.
		if( SIMPLE == data.comboType )
			CommandNotify(VCBN_KILLFOCUS);
		break;
		
	default:
		
		break;
	}
}

void CVComboBox::HandleDispChange()
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	if ( !data.bSendDispChange )
		return;
	/*
	NMHDR nmh;
	nmh.hwndFrom	= m_hWnd;
	nmh.idFrom		= GetDlgCtrlID();
	nmh.code		= VCBN_DISPCHANGE;
	GetParent()->SendMessage(WM_NOTIFY, nmh.idFrom, (LPARAM)&nmh);
	*/
	CommandNotify(VCBN_DISPCHANGE);
}

void CVComboBox::HandleListBoxNotifications( UINT codeNotify )
{
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	
	switch( codeNotify )
	{
	case VLBN_SELCHANGE:
		{
			ModifyText();
			
			// MSDN state:
			// In a combo box with the CBS_SIMPLE style, the CBN_SELENDOK notification message is sent 
			// immediately before every CBN_SELCHANGE notification message. 

			if ( SIMPLE == data.comboType )
			{
				CommandNotify(VCBN_SELENDOK);
			}

			// Send the VCBN_SELCHANGE notification.
			CommandNotify(VCBN_SELCHANGE);
		}
		break;
		
	case VLBN_CLOSEUP:
		{
			if ( data.bNotifySelEnd )
			{
				int nCurSel = GetCurSel();
				CommandNotify( nCurSel >= 0 ? VCBN_SELENDOK : VCBN_SELENDCANCEL);
			}

			//DefWindowProc(LB_SETCURSEL, (WPARAM)nCurSel, 0);

			// Notify the parent that the listbox has closed.
			CommandNotify(VCBN_CLOSEUP);
			
			data.nTopIndex = GetCurSel();
			SetTopIndex(data.nTopIndex);

			CRect rcClient;
			GetClientRect(rcClient);

			CRect rcBtn = GetDropDownHittestRect();
			const MSG* pMsg = GetCurrentMessage();
			CPoint pt = pMsg->pt;
			ScreenToClient(&pt);
			m_bBtnHilight	= rcBtn.PtInRect(pt);
			m_bPushed		= FALSE;
			m_bHighlighted	= rcClient.PtInRect(pt);
			Invalidate();
			UpdateWindow();
		}
		break;
		
	case VLBN_SETFOCUS:
		// Send the VCBN_SETFOCUS notification to the parent.
		if( SIMPLE == data.comboType )
			CommandNotify(VCBN_SETFOCUS);
		break;
		
	case LBN_DBLCLK:
		// This notification comes only for the VCBN_SIMPLE style
		// combobox. Simply forward this to the parent.
		CommandNotify(VCBN_DBLCLK);
		break;
		
	default:
		break;
	}
}

void CVComboBox::OnMeasureItem( int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	MeasureItem(lpMeasureItemStruct);
}

BOOL CVComboBox::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	if (GetComboListBox().GetSafeHwnd())
	{
		if( GetComboListBox().IsWindowVisible() )
		{
			int nTopIndex = GetTopIndex();
			if ( zDelta < 0 )
			{
				nTopIndex += 3;
				if ( nTopIndex > GetItemCount() )
				{
					nTopIndex = GetItemCount();
				}
			}
			else
			{
				nTopIndex -= 3;
				if ( nTopIndex < 0 )
				{
					nTopIndex = 0;
				}
			}
			SetTopIndex(nTopIndex);
		}
		else
		{
			int nCurSel = GetCurSel();
			BOOL bUpdateSel = FALSE;
			if ( zDelta < 0 )
			{
				if ( nCurSel < GetItemCount() )
				{
					++nCurSel;
					bUpdateSel = TRUE;
				}
			}
			else
			{
				if ( nCurSel > 0 )
				{
					--nCurSel;
					bUpdateSel = TRUE;
				}
			}
			if ( bUpdateSel )
			{
				SetCurSel(nCurSel);
				ModifyText();
			}
		}
	}
	return CVComboBoxBase::OnMouseWheel(nFlags, zDelta, pt);
}

void CVComboBox::ModifyText()
{
	int		nCurSel = GetCurSel();
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
		
	if( nCurSel != LB_ERR )	// Get the text for the combobox / editbox.
	{
		CString strText = GetItemText(nCurSel);
		
		if( (DROPDOWN == data.comboType) || (SIMPLE == data.comboType) )
		{
			// We do not want to send a VCBN_DISPCHANGE notification when 
			// we set the text ourselves.
			data.bSendDispChange = FALSE;
			GetComboEdit().SetWindowText(strText);
			data.bSendDispChange = TRUE;
			GetComboEdit().SetSel(0, -1);
		}
		else
		{
			SetWindowText(strText);
			InvalidateRect(NULL, TRUE);
		}
	}
	else  // Current selection == LB_ERR
	{
		// Make the window text blank.
		TCHAR	strText[1] = {0};
		
		if( (DROPDOWN == data.comboType) || (SIMPLE == data.comboType) )
		{
			GetComboEdit().SetWindowText(strText);
			GetComboEdit().SetSel(0, -1);
		}
		else
		{
			SetWindowText(strText);
			InvalidateRect(NULL, TRUE);
		}
	}
	CommandNotify(VCBN_EDITCHANGE);
}

CRect CVComboBox::GetDropDownHittestRect()
{
	CRect rcDropDown(0,0,0,0);
	VIRTUALCOMBOBOX_DATA& data = GetVirtualComboData();
	if ( DROPDOWN == data.comboType )
	{
		rcDropDown = GetButtonDrawRect();
	}
	else if ( DROPDOWNLIST == data.comboType )
	{
		GetClientRect(rcDropDown);
	}
	return rcDropDown;
}
