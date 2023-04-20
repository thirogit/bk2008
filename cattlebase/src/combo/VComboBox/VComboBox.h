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

#if !defined(AFX_VCOMBOBOX_H__53E3E9B6_A479_4A66_A7BA_FDBEBD193B6C__INCLUDED_)
#define AFX_VCOMBOBOX_H__53E3E9B6_A479_4A66_A7BA_FDBEBD193B6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VComboBox.h : header file
//

#include "CustomDrawControl.h"
#include "CustomDrawCommon.h"

/*----------------------------------------------------------------------------*/
/* Global control
/*----------------------------------------------------------------------------*/

#define ANIMATE_LISTBOX_SHOWUP

/*----------------------------------------------------------------------------*/
/* CVComboBox, MFC wrapper class for virtual combo box
/* http://support.microsoft.com/kb/197690
/*----------------------------------------------------------------------------*/
#define WC_VCOMBOBOXA           "Virtual_ComboBox32"
#define WC_VCOMBOBOXW           L"Virtual_ComboBox32"

// #define WC_VCOMBOBOXA           "ComboBox"
// #define WC_VCOMBOBOXW           L"ComboBox"

#ifdef UNICODE
	#define WC_VCOMBOBOX            WC_VCOMBOBOXW
#else
	#define WC_VCOMBOBOX            WC_VCOMBOBOXA
#endif	// UNICODE

// The virtual combobox styles. 
// Description of the styles is given below:
// 
// VCBS_SIMPLE:			The virtual combobox has a list box and an editbox.
//						The list box is displayed at all times, and the current
//						selection is displayed in the edit control.
// VCBS_DROPDOWN:		Similar to VCBS_SIMPLE, except that the list box is not 
//						displayed unless the user selects an icon next to the edit control.
// VCBS_DROPDOWNLIST:	Similar to VCBS_DROPDOWN, except that the edit control is 
//						replaced by a static-text item that displays the current selection 
//						in the list box.
// VCBS_AUTOHSCROLL:	Automatically scrolls the text in the edit control to the right 
//						when the user types a character at the end of the line. If this 
//						style is not set, only text that fits within the rectangular 
//						boundary is allowed. Valid only for the styles, VCBS_SIMPLE &
//						VCBS_DROPDOWN.
// VCBS_OEMCONVERT:		Text entered in the virtual combo-box edit control is converted from the 
//						ANSI character set to the OEM character set and then back to ANSI. 
//						This ensures proper character conversion when the application calls 
//						the AnsiToOem Windows function to convert an ANSI string in the virtual combo 
//						box to OEM characters. This style is most useful for combo boxes that 
//						contain filenames and applies only to combo boxes created with the 
//						VCBS_SIMPLE or VCBS_DROPDOWN styles.
// VCBS_DISABLENOSCROLL:The list box shows a disabled vertical scroll bar when the list box does 
//						not contain enough items to scroll. Without this style, the scroll bar 
//						is hidden when the list box does not contain enough items.
// VCBS_UPPERCASE:		All characters are displayed in uppercase.
// VCBS_LOWERCASE:		All characters are displayed in lowercase.

#define	VCBS_SIMPLE				0x0001L
#define	VCBS_DROPDOWN			0x0002L
#define VCBS_DROPDOWNLIST		0x0003L
#define VCBS_AUTOHSCROLL		0x0010L
#define VCBS_OEMCONVERT			0x0020L
#define VCBS_DISABLENOSCROLL	0x0040L
#define VCBS_UPPERCASE			0x0100L
#define VCBS_LOWERCASE			0x0200L

#define VCBS_DEFAULT				(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | \
										WS_CLIPCHILDREN | VCBS_AUTOHSCROLL)

#define VCBS_DEFAULT_SIMPLE			(VCBS_DEFAULT | VCBS_SIMPLE)
#define VCBS_DEFAULT_DROPDOWN		(VCBS_DEFAULT | VCBS_DROPDOWN)
#define VCBS_DEFAULT_DROPDOWNLIST	(VCBS_DEFAULT | VCBS_DROPDOWNLIST)

/*
#define CBS_SIMPLE            0x0001L
#define CBS_DROPDOWN          0x0002L
#define CBS_DROPDOWNLIST      0x0003L
#define CBS_OWNERDRAWFIXED    0x0010L
#define CBS_OWNERDRAWVARIABLE 0x0020L
#define CBS_AUTOHSCROLL       0x0040L
#define CBS_OEMCONVERT        0x0080L
#define CBS_SORT              0x0100L
#define CBS_HASSTRINGS        0x0200L
#define CBS_NOINTEGRALHEIGHT  0x0400L
#define CBS_DISABLENOSCROLL   0x0800L
*/

// 
// The virtual combobox notification codes. These are sent as part of
// WM_COMMAND message.
// A description of the notification codes is given below.
//
// VCBN_CLOSEUP:	Sent when the listbox of a virtual combobox has been closed. 
//					Sent only for types VCBS_DROPDOWN & VCBS_DROPDOWNLIST.
// VCBN_DBLCLK:		Sent when the user double clicks an item in the listbox of 
//					a virtual combobox. Occurs only for a virtual combobox of type, 
//					VCBS_SIMPLE.
// VCBN_DROPDOWN:	Sent when the listbox of a virtual combobox is about to be 
//					made visible. Sent only for types VCBS_DROPDOWN & VCBS_DROPDOWNLIST.
// VCBN_EDITCHANGE:	Sent after the user has taken an action that may have altered 
//					the text in the edit control portion of a combo box, after the 
//					contents have been updated on the screen. Sent only for types 
//					VCBS_SIMPLE and VCBS_DROPDOWN.
// VCBN_EDITUPDATE: Sent after the user has taken an action that may have altered
//					the text in the edit control portion of a combo box, but before
//					the contents have been updated on the screen. Sent only for types
//					VCBS_SIMPLE and VCBS_DROPDOWN.
// VCBN_KILLFOCUS:  Sent when a virtual combobox loses keyboard focus.
// VCBN_SELCHANGE:	Sent when the user changes the current selection in the listbox
//					of a virtual combobox.
// VCBN_SETFOCUS:	Sent when a virtual combobox receives keyboard focus.

#define VCBN_ERRSPACE			(-1)
#define VCBN_SELCHANGE			1		
#define VCBN_DBLCLK				2
#define VCBN_SETFOCUS			3
#define VCBN_KILLFOCUS			4
#define VCBN_EDITCHANGE			5
#define VCBN_EDITUPDATE			6
#define VCBN_DROPDOWN			7
#define VCBN_CLOSEUP			8
#define VCBN_SELENDOK			9
#define VCBN_SELENDCANCEL		10

#define VCBN_DISPCHANGE			11


// Notification codes. These are sent as part of WM_NOTIFY message.
#define		VCBN_FIRST			1

// Sent to receive the display information for an item about to be
// displayed. 
#define		VCBN_GETDISPINFO	(VCBN_FIRST + 1)

// This notification is sent when the edit control contents have changed. This 
// allows the user of the control to specify the item number that needs to 
// move up to the top of the listbox. This notification is sent only for the
// virtual combo boxes with styles, VCBS_SIMPLE and VCBS_DROPDOWN.
//#define		VCBN_DISPCHANGE		(VCBN_FIRST + 2)

// Virtual combobox return values.
#define		VCB_OKAY		0
#define		VCB_ERR			(-1)


// Virtual combobox class-specific messages.
#define		VCBM_FIRST			(WM_USER + 1)

// Purpose:	Get the number of items in a virtual combobox.
// wParam:	Not used, must be zero.
// lParam:	Not used, must be zero.
// Return:	The number of items in the combobox, or 
//			VCB_ERR if an error occurs.
#define		VCBM_GETCOUNT				(VCBM_FIRST + 0)

// Purpose:	Get the zero-based index of the currently selected item in the 
//			listbox of a virtual combobox.
// wParam:	Not used, must be zero.
// lParam:	Not used, must be zero.
// Return:	The zero-based index of the currently selected item.
//			VCB_ERR when no item is selected.
#define		VCBM_GETCURSEL				(VCBM_FIRST + 1)

// Purpose:	Get the screen coordinates of the drop-down listbox
//			of a virtual combobox.
// wParam:	Not used, must be zero.
// lParam:	RECT * prc - Pointer to the RECT structure that will receive the coordinates.
// Return:	TRUE if the message succeeds, FALSE if the message fails.
#define		VCBM_GETDROPPEDCONTROLRECT	(VCBM_FIRST + 2)

// Purpose:	Determine whether the listbox of a virtual combobox is dropped down.
// wParam:	Not used, must be zero.
// lParam:	Not used, must be zero.
// Return:	TRUE if the listbox is visible, FALSE otherwise.
#define		VCBM_GETDROPPEDSTATE		(VCBM_FIRST + 3)

// Purpose:	Get the starting & ending character positions of the 
//			current selection in the edit control of a virtual combobox.
// wParam:	DWORD* pdwStart - Pointer to a 32-bit value that receives
//			the starting position of the selection. Can be NULL.
// lParam:	DWORD* pdwEnd - Pointer to a 32-bit value that receives
//			the ending position of the selection. Can be NULL.
// Return:	The	low-order word has the starting position of the selection, 
//			and the high-order word has the position of the character after 
//			the last selected character.
#define		VCBM_GETEDITSEL				(VCBM_FIRST + 4)

// Purpose:	Get the zero-based index of the first visible item in the 
//			listbox portion of a virtual combobox. Initially, the item
//			with index zero is at the top of the listbox, but if the 
//			listbox contents have been scrolled, another item may
//			be at the top.
// wParam:	Not used, must be zero.
// lParam:	Not used, must be zero.
// Return:	If the message is successful, the index of the first visible item in
//			the listbox portion of the virtual combobox, VCB_ERR if the
//			message fails.
#define		VCBM_GETTOPINDEX			(VCBM_FIRST + 5)

// Purpose:	Limit the length of the text that the user may type into
//			the edit control of a virtual combobox.
// wParam:	int cchMax - the maximum number of characters the user can
//			enter. If this parameter is zero, the text length is
//			0x7ffffffe characters.
// lParam:	Not used, must be zero.
// Return:	Always TRUE.
// Comments:If the virtual combobox does not have the VCBS_AUTOHSCROLL
//			style, setting the text limit to be larger than the size of the
//			edit control has no effect. The default limit is 30,000 characters. 
#define		VCBM_LIMITTEXT				(VCBM_FIRST + 6)

// Purpose:	Set the new count of items in a virtual combobox.
// wParam:	int cItems - The new count of items in the virtual combobox.
// lParam:	Not used, must be zero.
// Return:	Zero if the message succeeds, VCB_ERR otherwise.
#define		VCBM_SETCOUNT				(VCBM_FIRST + 7)

// Purpose:	Set the current selected item in the combobox. If necessary,
//			the list scrolls the item into view. The text in the 
//			edit control of the combobox changes, and the previous
//			selection is removed.
// wParam:	int index - The zero-based index of the item. If the index is
//			-1, any current selection is removed and the edit control is
//			cleared.
// lParam:	Not used, must be zero.
// Return:	The index of the item selected if the message is successful.
//			If the index is greater than the number of items, or if
//			index is set to -1, the return value is VCB_ERR, and the
//			selection is cleared.
#define		VCBM_SETCURSEL				(VCBM_FIRST + 8)

// Purpose:	Set the selected characters in the edit control of a virtual 
//			combobox. 
// wParam:	Not used, must be zero.
// lParam:	MAKELPARAM(ichStart, ichEnd) -
//			ichStart -	The low-order word specifies the starting position.
//						If this value is -1, any selection is removed.
//			ichEnd -	The high-order word specifies the ending position.
//						If this value is -1, all text from the starting
//						position to the last character in the edit control
//						is selected.
// Return:	TRUE if the message succeeds. It is VCB_ERR if sent to
//			a virtual combobox of type VCBS_DROPDOWNLIST.
// Comments: The positions are zero-based.
#define		VCBM_SETEDITSEL				(VCBM_FIRST + 9)

// Purpose:	Make a particular item visible in the listbox of a virtual
//			combobox. The list box contents are scrolled so that either
//			the specified item appears at the top of the list box or the
//			maximum scroll range has been reached.
// wParam:	Zero-based index of the item.
// lParam:	Not used, must be zero.
// Return:	Zero if the message succeeds, VCB_ERR otherwise.
#define		VCBM_SETTOPINDEX			(VCBM_FIRST + 10)

// Purpose:	Show/hide the listbox of a virtual combobox of type
//			VCBS_DROPDOWN or VCBS_DROPDOWNLIST.
// wParam:	BOOL bShow - TRUE to show the listbox, FALSE to hide it.
// lParam:	Not used, must be zero.
// Return:	TRUE always.
// Comments: This message has no effect on a VCBS_SIMPLE type virtual
//			 combobox.
#define		VCBM_SHOWDROPDOWN			(VCBM_FIRST + 11)

/*----------------------------------------------------------------------------*/
/* CVComboEdit
/*----------------------------------------------------------------------------*/
#ifndef CVComboEditBase
	#define CVComboEditBase		CEdit
#endif

class CVComboBox;

class CVComboEdit : public CVComboEditBase
{
public:
	CVComboEdit();
	virtual ~CVComboEdit();
protected:
	int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	void			OnDestroy();

	void			OnSetFocus(CWnd* pOldWnd);
	void			OnKillFocus(CWnd* pNewWnd);
	void			OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void			OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void			OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	UINT			OnGetDlgCode();
	void			OnLButtonDown(UINT nFlags, CPoint point);
protected:
	void			SetBuddyComboBox(CVComboBox* pVComboBox);
	void			CommandNotify( int nCode );

	BOOL			PreTranslateMessage(MSG* pMsg);
protected:
	CVComboBox*		m_pVComboBox;
	friend class CVComboBox;
protected:
	DECLARE_MESSAGE_MAP()
};

/*----------------------------------------------------------------------------*/
/* CVComboListBox
/*----------------------------------------------------------------------------*/

#ifndef CVComboListBoxBase
	#define CVComboListBoxBase	CComboListBox
#endif // CObjInfoListBoxBase

class CVComboListBox : public CVComboListBoxBase
{
	DECLARE_DYNCREATE(CVComboListBox)
public:
	CVComboListBox();
	virtual ~CVComboListBox();
public:
	virtual CString GetItemText(UINT nItem);
public:
	// The virtual listbox types.
	typedef enum { SIMPLE, DROPDOWN, DROPDOWNLIST } LISTBOXTYPE;

	// Structure that contains the information for the listbox.
	typedef struct tagVIRTUALCOMBOLISTBOX_DATA
	{
		tagVIRTUALCOMBOLISTBOX_DATA()
			: nHitTest(0)		// We are only bothered about HTVSCROLL & HTHSCROLL.
			, nCurSel(LB_ERR)	// No selection currently.
			, listboxType(DROPDOWN)
		{
			;
		}
		//HWND		hWndCombo;		// The handle to the combobox.
		int			nHitTest;		// The hittest status for the mouse.
		int			nCurSel;		// The current selected item.
		LISTBOXTYPE	listboxType;	// The virtual listbox type.
	} VIRTUALCOMBOLISTBOX_DATA, * PVIRTUALCOMBOLISTBOX_DATA;

	VIRTUALCOMBOLISTBOX_DATA&	GetVirtualComboListBoxData()			{ return m_data; }
protected:
	BOOL	PreCreateWindow(CREATESTRUCT& cs);
	void	OnLButtonDown(UINT nFlags, CPoint point);
	void	OnLButtonUp(UINT nFlags, CPoint point);
	void	OnMouseMove(UINT nFlags, CPoint point);
	void	OnCaptureChanged(CWnd* pWnd);
	void	OnShowWindow(BOOL bShow, UINT nStatus);
	void	OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void	OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnSetFocus(CWnd* pOldWnd);

	LRESULT	OnGetCurSel(WPARAM wParam, LPARAM lParam);
	LRESULT	OnSetCurSel(WPARAM wParam, LPARAM lParam);
protected:
	void	CommandNotify( int nCode );
protected:
	VIRTUALCOMBOLISTBOX_DATA	m_data;
protected:
	DECLARE_MESSAGE_MAP()
};

/*----------------------------------------------------------------------------*/
/* CVComboBox
/*----------------------------------------------------------------------------*/
#ifndef CVComboBoxBase
	#define CVComboBoxBase		CWnd
#endif

class CVComboBox : public CVComboBoxBase
{
	DECLARE_DYNCREATE(CVComboBox)
public:
	CVComboBox();
	virtual ~CVComboBox();
public:
	virtual BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL	CreateFromCtrl(CWnd* pParent, int nID, DWORD dwStyle = VCBS_DEFAULT_DROPDOWN);

	int				GetItemCount();
	void			SetItemCount(int nCount);
	void			SetDroppedVisibleItemCount(int nCount, BOOL bRepaint = TRUE);

	// Attributes
	// for entire combo box
	int GetCurSel();
	int SetCurSel(int nSelect);

	// Win4
	int GetTopIndex();
	int SetTopIndex(int nIndex);

	void SetHorizontalExtent(UINT nExtent);
	UINT GetHorizontalExtent();
	int SetDroppedWidth(UINT nWidth);
	int GetDroppedWidth();
	
	// for edit control
	DWORD GetEditSel();
	BOOL SetEditSel(int nStartChar, int nEndChar);

	// for combobox item
	DWORD GetItemData(int nIndex);
	int SetItemData(int nIndex, DWORD dwItemData);
	void* GetItemDataPtr(int nIndex);

	int SetItemHeight(UINT cyItemHeight);
	int GetItemHeight();
	
	int SetExtendedUI(BOOL bExtended = TRUE);
	BOOL GetExtendedUI();
	
	BOOL GetDroppedControlRect(LPRECT lprect);
	BOOL GetDroppedState();
	
	// Operations
	// for drop-down combo boxes
	void ShowDropDown(BOOL bShowIt = TRUE);
	
	virtual CVComboListBox&		GetVComboListBox()						{ return m_vComboList; }
	virtual CComboListBox&		GetComboListBox()						{ return GetVComboListBox(); }
	virtual CVComboEdit&		GetComboEdit()							{ return m_vComboEdit; }
	
	virtual CString GetItemText(UINT nItem);
	virtual int	GetItemIconIndex(UINT nItem);
	
	virtual CCustomDrawToolTipCtrl& GetCustomDrawToolTips()				{ return GetComboListBox().GetCustomDrawToolTips(); }

	// The virtual combobox types.
	typedef enum { SIMPLE, DROPDOWN, DROPDOWNLIST } COMBOTYPE;
	
	// The structure that contains information for the combobox.
	typedef struct tagVirtualCombobox_Data
	{
		tagVirtualCombobox_Data()
			: hFont(NULL)
			, nHeight(18)
			, comboType(DROPDOWN)
			, bSendDispChange(TRUE)
			, nTopIndex(0)
			, bNotifySelEnd(TRUE)
		{
			//SetRect(&rcButton, 0, 0, 0, 0);
		}
		// 	HWND		hWndLB;		// The handle of the listbox.
		// 	HWND		hWndEdit;	// The handle of the editbox.
		// 	HWND		hWndParent;	// The original parent. All notifications are
		// 	// sent to this window, even if the parent is changed.
		
		//RECT		rcButton;	// The co-ordinates of the arrow button.
		
		HFONT		hFont;		// the combobox font.
		int			nHeight;	// The total combobox height, including the listbox.
		
		COMBOTYPE	comboType;	// The main combobox type.
		
		// 	DWORD		dwStyle;	// The combobox style.
		// 	BOOL		bUnicode;	// Flag to indicate whether to send Unicode	
									// or ANSI notifications.
		BOOL		bSendDispChange;	// Flag to indicate if we need to send a 
										// VCBN_DISPCHANGE notification.
		int			nTopIndex;	// The current top index.
		BOOL		bNotifySelEnd;
	} VIRTUALCOMBOBOX_DATA, * PVIRTUALCOMBOBOX_DATA;

	VIRTUALCOMBOBOX_DATA&	GetVirtualComboData()						{ return m_data; }

	inline void	LockSetRedraw(BOOL bDraw = TRUE)
	{
		if ( (bDraw ? (0 == --m_nLockDrawCount) : (0 == m_nLockDrawCount++)) )
			SetRedraw( bDraw );
	}

	inline void			SetTextDrawer(ITextDrawer* pTextDrawer)			{ GetComboListBox().SetTextDrawer(pTextDrawer); }
	inline ITextDrawer*	GetTextDrawer()									{ return GetComboListBox().GetTextDrawer(); }
protected:
	virtual void	PreAddToolTipForItem(int nItem);
	virtual void	PreShowToolTipForItem(int nItem);
	virtual void	PrePopToolTip();
	
	void			SetTextOfItem(int nIndex);

protected:
	void			CommandNotify( int nCode );
	void			HandleEditNotifications( UINT codeNotify );
	void			HandleListBoxNotifications( UINT codeNotify );
	void			HandleDispChange();

	void			ModifyText();

	void			AutoResizeListBox(UINT nListHeight = 0, BOOL bRepaint = FALSE);
	BOOL			IsListBoxShowAbove();
	virtual BOOL	ShowListBox(BOOL bShow = TRUE);

	virtual void	MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	virtual void	OnFillBackground(CDC* pDC, const CRect& rect);
	virtual void	OnDrawBorder(CDC* pDC, CRect& rect);
	virtual void	OnDrawButton(CDC* pDC, CRect& rect);
	virtual void	OnDrawItem(CDC* pDC, CRect& rect);

	virtual CRect	GetButtonDrawRect();
	CRect			GetDropDownHittestRect();
protected:
	void			PreSubclassWindow();
	int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	void			OnDestroy();
	
	virtual BOOL	OnInitCombo();
	virtual void	OnDestroyCombo();
	
	BOOL			OnEraseBkgnd(CDC* pDC);
	void			OnPaint();
	
	LRESULT			OnGetFont(WPARAM wParam, LPARAM lParam);
	LRESULT			OnSetFont(WPARAM wParam, LPARAM lParam);

	void			OnLButtonDown(UINT nFlags, CPoint point);
	void			OnLButtonUp(UINT nFlags, CPoint point);
	BOOL			OnCommand(WPARAM wParam, LPARAM lParam);
	void			OnSetFocus(CWnd* pOldWnd);
	void			OnKillFocus(CWnd* pNewWnd);
	void			OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void			OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	UINT			OnGetDlgCode();
	void			OnWindowPosChanging(WINDOWPOS* lpwndpos);
	void			OnEnable(BOOL bEnable);
	LRESULT			OnNotifyFormat(WPARAM wParam, LPARAM lParam);
	LRESULT			OnGetText(WPARAM wParam, LPARAM lParam);
	LRESULT			OnGetTextLength(WPARAM wParam, LPARAM lParam);
	LRESULT			OnGetCount(WPARAM wParam, LPARAM lParam);
	LRESULT			OnGetCurSel(WPARAM wParam, LPARAM lParam);
	LRESULT			OnGetDroppedState(WPARAM wParam, LPARAM lParam);
	LRESULT			OnGetDroppedControlRect(WPARAM wParam, LPARAM lParam);
	LRESULT			OnGetEditSel(WPARAM wParam, LPARAM lParam);
	LRESULT			OnGetTopIndex(WPARAM wParam, LPARAM lParam);
	LRESULT			OnLimitText(WPARAM wParam, LPARAM lParam);
	LRESULT			OnSetText(WPARAM wParam, LPARAM lParam);
	LRESULT			OnSetCount(WPARAM wParam, LPARAM lParam);
	LRESULT			OnSetCurSel(WPARAM wParam, LPARAM lParam);
	LRESULT			OnSetEditSel(WPARAM wParam, LPARAM lParam);
	LRESULT			OnSetTopIndex(WPARAM wParam, LPARAM lParam);
	LRESULT			OnShowDropDown(WPARAM wParam, LPARAM lParam);
	void			OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	BOOL			OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	LRESULT			OnCustomToolTipNotify(WPARAM wParam, LPARAM lParam);

	void			OnMouseMove(UINT nFlags, CPoint point);
	LRESULT			OnMouseLeave(WPARAM wParam, LPARAM lParam);
	
	static BOOL		RegisterWindowClass();
protected:
	CVComboListBox			m_vComboList;
	CVComboEdit				m_vComboEdit;
	friend class CVComboEdit;

	VIRTUALCOMBOBOX_DATA	m_data;

	BOOL					m_bHighlighted;
	BOOL					m_bPushed;
	BOOL					m_bBtnHilight;

	BOOL					m_bDrawHotItem;
	BOOL					m_bMouseEventsTracked;

	int						m_nLockDrawCount;
protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCOMBOBOX_H__53E3E9B6_A479_4A66_A7BA_FDBEBD193B6C__INCLUDED_)
