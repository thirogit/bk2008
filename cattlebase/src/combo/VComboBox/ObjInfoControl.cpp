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

#include "StdAfx.h"
#include "ObjInfoControl.h"

/*----------------------------------------------------------------------------*/
/* IIconMapper
/*----------------------------------------------------------------------------*/
IIconMapper::IIconMapper(int cx /*= 16*/, int cy /*= 16*/)
{
	UINT nImageListFlags = ILC_COLOR32|ILC_MASK;
	m_ImageList.Create(cx, cy, nImageListFlags, 0, 8);
}

IIconMapper::~IIconMapper()
{
	;
}

/*----------------------------------------------------------------------------*/
/* CAsciiProgressGenerator
/*----------------------------------------------------------------------------*/
CAsciiProgressGenerator::CAsciiProgressGenerator(DWORD dwMinTimeLimit /*= 600*/)
	: m_nProgress(0)
	, m_dwMinTimeLimit(dwMinTimeLimit)
{
	m_dwCurrentTime = GetTickCount();
}

CString CAsciiProgressGenerator::GetProressAscii()
{
	const TCHAR* szProgress[] = {
		_T("--"), _T("\\"), _T("|"), _T("/")
	};
	UINT nProgress = m_nProgress;
	DWORD dwCurrentTime = GetTickCount();
	if ( GetTickCount() - m_dwCurrentTime > m_dwMinTimeLimit )
	{
		++m_nProgress;
		m_dwCurrentTime = dwCurrentTime;
	}
	return szProgress[nProgress % 4];
}

/*----------------------------------------------------------------------------*/
/* CObjInfoListCtrl
/*----------------------------------------------------------------------------*/

#ifndef LVS_EX_INFOTIP
	#define LVS_EX_INFOTIP          0x00000400 // listview does InfoTips for you
#endif // LVS_EX_INFOTIP

IMPLEMENT_DYNCREATE(CObjInfoListCtrl, CObjInfoListCtrlBase)

CObjInfoListCtrl::CObjInfoListCtrl()
	: m_pObjInfoMatcher(NULL)
	, m_pIconMapper(NULL)
#ifdef UPDATE_OBJINFO_VIA_TIMER
	, m_nTimer(0)
	, m_bUpdateListByTimer(TRUE)
#endif // UPDATE_OBJINFO_VIA_TIMER
{
	
}

CObjInfoListCtrl::~CObjInfoListCtrl()
{
	;
}

BEGIN_MESSAGE_MAP(CObjInfoListCtrl, CObjInfoListCtrlBase)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetObjListDispInfo)
	ON_NOTIFY_REFLECT(LVN_ODFINDITEM, OnOdFindObjListItem)
#ifdef UPDATE_OBJINFO_VIA_TIMER
	ON_WM_TIMER()
#endif // UPDATE_OBJINFO_VIA_TIMER
	ON_MESSAGE(WM_OBJINFO_THREADEVENT, OnObjInfoThreadEvent)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CObjInfoListCtrl::SetObjInfoMatcher( CObjInfoMatcher* pMatcher, BOOL bSubscribeThreadEvent /*= TRUE*/ )
{
	m_pObjInfoMatcher = pMatcher;
	if (m_pObjInfoMatcher && bSubscribeThreadEvent)
	{
		// to bind the thread event.
		Subscribe(m_pObjInfoMatcher->GetObjInfoHolder());
	}
}

void CObjInfoListCtrl::OnGetObjListDispInfo( NMHDR* pNMHDR, LRESULT* pResult )
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
	ASSERT(pDispInfo);
	
	if ( !GetObjInfoMatcher() )
	{
		ASSERT(0);	// how come this happen?
		return;
	}
	
	LVITEM& item = pDispInfo->item;
	
	if ( item.mask & LVIF_TEXT )
	{
		//_tcsncpy(item.pszText, const_cast<LPTSTR>(GetObjInfoMatcher()->GetObjInfoTextInMatches(item.iItem, item.iSubItem)), item.cchTextMax);
		item.pszText = const_cast<LPTSTR>(GetObjInfoMatcher()->GetObjInfoTextInMatches(item.iItem, item.iSubItem));
	}
	
	if ( item.mask & LVIF_IMAGE )
	{
		item.iImage = GetItemIconIndex(item.iItem);
	}
	
	*pResult = 0;
}

void CObjInfoListCtrl::OnOdFindObjListItem( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMLVFINDITEM pFindItem = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);
	
	LVFINDINFO& lvfi = pFindItem->lvfi;
	
	ASSERT( !(lvfi.flags & LVFI_PARAM) );

	*pResult = -1;	// no match

	if ( !GetObjInfoMatcher() )
	{
		ASSERT(0);	// how come this happen?
		return;
	}
	
	if ( lvfi.flags & LVFI_STRING && GetFindObjInfoListColumnIndex() >= 0 )
	{
		const int nSearchStrLen = _tcslen(lvfi.psz);
		
		int nStartPos = pFindItem->iStart;
		if ( nStartPos >= GetItemCount() )
		{
			nStartPos = 0;
		}
		
		int nCurrentPos = nStartPos;
		do 
		{
			LPCTSTR lpcszItem = const_cast<LPCTSTR>(GetObjInfoMatcher()->GetObjInfoTextInMatches(nCurrentPos, GetFindObjInfoListColumnIndex()));
			if ( _tcsnicmp(lvfi.psz, lpcszItem, nSearchStrLen) == 0 )
			{
				*pResult = nCurrentPos;
				return;
			}
			++nCurrentPos;
			if ( nCurrentPos >= GetItemCount() )
				nCurrentPos = 0;
		} while (nCurrentPos != nStartPos);
	}
}

void CObjInfoListCtrl::OnInitList()
{
	CObjInfoListCtrlBase::OnInitList();
	ASSERT( GetStyle() & LVS_OWNERDATA );	// must be a virtual list-ctrl!

	ASSERT(GetObjInfoMatcher() != NULL);	// how come this happen?!
	ASSERT(m_arrColumnLabels.GetSize() == GetObjInfoHolder()->GetObjInfoColumnCount());

	for (int nCol = 0; nCol < m_arrColumnLabels.GetSize(); ++nCol)
	{
		InsertColumn(nCol, m_arrColumnLabels[nCol]);
	}

	if ( GetIconMapper() )
	{
		SetImageList( &GetIconMapper()->GetImageList(), LVSIL_SMALL );
	}
}

void CObjInfoListCtrl::OnDestroyList()
{
	//TRACE("======= CObjInfoListCtrl::OnDestroyList()\n");
	if (m_pObjInfoMatcher )
	{
		UnSubscribe( m_pObjInfoMatcher->GetObjInfoHolder());
	}
#ifdef UPDATE_OBJINFO_VIA_TIMER
	if (m_nTimer)
	{
		KillTimer(m_nTimer);
		m_nTimer = 0;
	}
#endif // UPDATE_OBJINFO_VIA_TIMER
	CObjInfoListCtrlBase::OnDestroyList();
}

void CObjInfoListCtrl::AutoResizeColumn()
{
	if ( GetObjInfoMatcher() )
	{
		for (UINT nCol = 0; nCol < GetObjInfoHolder()->GetObjInfoColumnCount(); ++nCol)
		{
			SetColumnWidth(nCol, LVSCW_AUTOSIZE);
		}
	}
}

BOOL CObjInfoListCtrl::LoadColumnLabels( UINT nResID )
{
	return GetStringResourceArray(nResID, m_arrColumnLabels);
}

#ifdef UPDATE_OBJINFO_VIA_TIMER
void CObjInfoListCtrl::OnTimer( UINT nIDEvent )
{
	CObjInfoListCtrlBase::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case UPDATE_INFOLIST_TIMER_ID:
		ASSERT(GetObjInfoHolder());
		if ( !GetObjInfoHolder()->IsTaskStopped() )
		{
			//if ( GetObjInfoMatcher()->GetMatchObjInfoCount() > GetItemCount() )
				OnUpdateObjInfoList();
		}
		break;
	default:
		break;
	}
}
#endif // UPDATE_OBJINFO_VIA_TIMER

BOOL CObjInfoListCtrl::RefreshObjInfoList()
{
	if (!GetObjInfoHolder())
	{
		ASSERT(0);
		return FALSE;
	}

	SetItemCountEx(0);

#ifdef RETRIEVE_OBJINFO_IN_NEWTHREAD
	BOOL bRet = GetObjInfoMatcher()->RunInThread();
#else
	OnObjInfoThreadEvent(CThreadedObj::EVENT_ONSTART, 0);
	BOOL bRet = GetObjInfoMatcher()->RetrieveObjInfo();
	OnObjInfoThreadEvent(CThreadedObj::EVENT_ONFINISH, 0);
#endif // RETRIEVE_OBJINFO_IN_NEWTHREAD

	return bRet;
}

void CObjInfoListCtrl::OnUpdateObjInfoList(bool bAutoResizeColumns /*= true*/)
{
	LockSetRedraw(FALSE);
	GetParent()->SendMessage(WM_OBJINFOLIST_UPDATE, (WPARAM)m_hWnd);

	size_t nObjInfoCount = GetObjInfoMatcher()->GetMatchObjInfoCount();
	SetItemCountEx(nObjInfoCount, LVSICF_NOSCROLL);
	
	if ( GetObjInfoHolder() )
	{
		int nSortInfoType = GetObjInfoHolder()->GetSortColumn();
		if ( nSortInfoType >= CMultiColInfoHolder::INFOCOL_FIRST )
		{
			GetObjInfoHolder()->Sort(nSortInfoType, GetObjInfoHolder()->IsSortAscending());
		}
	}
	
	if (bAutoResizeColumns)
	{
		AutoResizeColumn();
	}
	//Invalidate();

	LockSetRedraw(TRUE);
}

BOOL CObjInfoListCtrl::OnSort( int nCol )
{
	if (GetObjInfoHolder())
	{
		LockSetRedraw(FALSE);
		SetItemState(-1, 0, LVIS_SELECTED|LVIS_FOCUSED);
		
		GetObjInfoHolder()->SwitchSort(nCol);
		
		OnUpdateObjInfoList(false);
		//Invalidate();
		
		LockSetRedraw(TRUE);
		SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		return TRUE;
	}
	return FALSE;
}

bool CObjInfoListCtrl::IsSortable() const
{
	if (GetObjInfoMatcher())
	{
		return GetObjInfoMatcher()->IsSortable();
	}
	return false;
}

void CObjInfoListCtrl::OnNotify( ISubject* pSubject, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	ASSERT(pSubject);
	if ( NULL == m_hWnd )
		return;
	if ( CThreadedObj::EVENT_ONSTART == wParam )
	{
		SetItemCountEx(0);
		GetObjInfoMatcher()->Reset();
	}
	// Use PostMessage to avoid deadlock
	PostMessage(WM_OBJINFO_THREADEVENT, wParam, lParam);
}

LRESULT CObjInfoListCtrl::OnObjInfoThreadEvent( WPARAM wParam, LPARAM )
{
	ASSERT( IsWindow(m_hWnd) );
	switch (wParam)
	{
	case CThreadedObj::EVENT_ONSTART:
#ifdef UPDATE_OBJINFO_VIA_TIMER
		if (m_bUpdateListByTimer && 0 == m_nTimer)
		{
			m_nTimer = SetTimer(UPDATE_INFOLIST_TIMER_ID, UPDATE_INFOLIST_TIMER_ELAPSE, NULL);
		}
#endif // UPDATE_OBJINFO_VIA_TIMER
		break;
	case CThreadedObj::EVENT_ONFINISH:
	case CThreadedObj::EVENT_ONFORCESTOP:
		OnUpdateObjInfoList();
#ifdef UPDATE_OBJINFO_VIA_TIMER
		if (m_nTimer > 0)
		{
			KillTimer(m_nTimer);
			m_nTimer = 0;
		}
#endif // UPDATE_OBJINFO_VIA_TIMER
		break;
	default:
		return 0;	// no need to notify the parent for the other events
	}
	// Forward this message to the parent, lParam is not used!
	GetParent()->PostMessage(WM_OBJINFO_THREADEVENT, wParam, (LPARAM)m_hWnd);
	return 1;
}

int CObjInfoListCtrl::GetItemIconIndex( int nItem )
{
	if ( GetIconMapper() )
	{
		ASSERT(GetObjInfoHolder());
		ObjInfoHolderAccessLock(GetObjInfoHolder());
		return GetIconMapper()->GetIconIndex(GetObjInfoMatcher(), (WPARAM)nItem);
	}
	return -1;	// not found!
}

void CObjInfoListCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if ( VK_F5 == nChar )
	{
		RefreshObjInfoList();
		return;
	}
	CObjInfoListCtrlBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*----------------------------------------------------------------------------*/
/* CFileIconMapper
/*----------------------------------------------------------------------------*/
CFileIconMapper::CFileIconMapper(int cx /*= 16*/, int cy /*= 16*/)
	: IIconMapper(cx, cy)
{
}

CFileIconMapper::~CFileIconMapper()
{
	
}

int CFileIconMapper::GetFileIconIndex( const CFileInfoEnumerator::FileInfo* pFileInfo )
{
	LockableObjAccessLock(this);
	if ( pFileInfo )
	{
		LPCTSTR lpcszFileExt = const_cast<TCHAR*>(pFileInfo->sFileInfo[CFileInfoEnumerator::FIT_FILEEXT].c_str());
		LPCTSTR lpcszFilePath = const_cast<TCHAR*>(pFileInfo->sFileInfo[CFileInfoEnumerator::FIT_FILEPATH].c_str());
		return GetFileIconIndex(lpcszFilePath, lpcszFileExt);
	}
	return -1;
}

int CFileIconMapper::GetFileIconIndex( LPCTSTR lpcszFilePath, LPCTSTR lpcszFileExt )
{
	LPCTSTR lpcszMapKey = _tcsicmp(lpcszFileExt, _T("exe")) ? lpcszFileExt : lpcszFilePath;
	
	// lazy creation
	FileIconMap::const_iterator iter = m_FileIconIndexMap.find(lpcszMapKey);
	if ( iter != m_FileIconIndexMap.end() )
	{
		return iter->second;
	}
	else
	{
		HICON hIcon = GetAssociatedFileIcon(lpcszFilePath);
		if ( hIcon )
		{
			int nImgIndex = m_ImageList.Add(hIcon);
			m_FileIconIndexMap[lpcszMapKey] = nImgIndex;
			return nImgIndex;
		}
	}
	return -1;
}

int CFileIconMapper::GetFileIconIndex( LPCTSTR lpcszFilePath )
{
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
#ifdef MSVC_NEW_VER
	_tsplitpath_s(lpcszFilePath, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
#else
	_tsplitpath(lpcszFilePath, drive, dir, fname, ext);
#endif // MSVC_NEW_VER
	return GetFileIconIndex(lpcszFilePath, ext);
}

int CFileIconMapper::GetIconIndex( CObjInfoMatcher* pMatcher, WPARAM wParam, LPARAM lParam /*= 0*/ )
{
	ASSERT(pMatcher);
	int nItem = (int)wParam;
	CFileInfoMatcher* pFileInfoMatcher = static_cast<CFileInfoMatcher*>(pMatcher);
	CFileInfoHolder* pFileInfoHolder = static_cast<CFileInfoHolder*>(pMatcher->GetObjInfoHolder());
	
	int nObjInfoIndex = pFileInfoMatcher->GetObjInfoIndexInMatches(nItem);
	
	return GetFileIconIndex( pFileInfoHolder->GetFileInfo(nObjInfoIndex) );
}

/*----------------------------------------------------------------------------*/
/* CFileInfoListCtrl
/*----------------------------------------------------------------------------*/

IMPLEMENT_DYNCREATE(CFileInfoListCtrl, CFileInfoListCtrlBase)

CFileInfoListCtrl::CFileInfoListCtrl()
{
}

CFileInfoListCtrl::~CFileInfoListCtrl()
{
	
}

BEGIN_MESSAGE_MAP(CFileInfoListCtrl, CFileInfoListCtrlBase)
	ON_MESSAGE(WM_OBJINFO_THREADEVENT, OnObjInfoThreadEvent)
END_MESSAGE_MAP()

IIconMapper* CFileInfoListCtrl::GetIconMapper() const
{
	if ( !CFileInfoListCtrlBase::GetIconMapper() )
	{
		static CFileIconMapper s_FileIconMapper;
		return &s_FileIconMapper;
	}
	return CFileInfoListCtrlBase::GetIconMapper();
}

LRESULT CFileInfoListCtrl::OnObjInfoThreadEvent( WPARAM wParam, LPARAM lParam )
{
	LRESULT result = CFileInfoListCtrlBase::OnObjInfoThreadEvent(wParam, lParam);
	if ( wParam == CMultiColInfoHolder::EVENT_SORT_END )
	{
		Invalidate();
		UpdateWindow();
	}
	return result;
}

/*----------------------------------------------------------------------------*/
/* CObjInfoListBox
/*----------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CObjInfoListBox, CObjInfoListBoxBase)

CObjInfoListBox::CObjInfoListBox()
	: m_pObjInfoMatcher(NULL)
	, m_pIconMapper(NULL)
#ifdef UPDATE_OBJINFO_VIA_TIMER
	, m_nTimer(0)
	, m_bUpdateListByTimer(TRUE)
#endif // UPDATE_OBJINFO_VIA_TIMER
{
	
}

CObjInfoListBox::~CObjInfoListBox()
{
	
}

BEGIN_MESSAGE_MAP(CObjInfoListBox, CObjInfoListBoxBase)
	ON_MESSAGE(WM_OBJINFO_THREADEVENT, OnObjInfoThreadEvent)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CObjInfoListBox::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	// Specifies a no-data list box. 
	// Specify this style when the count of items in the list box will exceed one thousand. 
	// A no-data list box must also have the LBS_OWNERDRAWFIXED style, but must not have the LBS_SORT or LBS_HASSTRINGS style. 
	// see http://msdn.microsoft.com/en-us/library/bb775149(VS.85).aspx
	DWORD dwMustHaveStyle		= LBS_NODATA | LBS_OWNERDRAWFIXED;
	DWORD dwMustNotHaveStyle	= LBS_HASSTRINGS | LBS_SORT;
	ASSERT( (dwStyle & dwMustHaveStyle) == dwMustHaveStyle && (dwStyle & dwMustNotHaveStyle) == 0 );

	dwStyle &= ~dwMustNotHaveStyle;
	dwStyle |= dwMustHaveStyle;
	
	BOOL bRet = CObjInfoListBoxBase::Create(dwStyle, rect, pParentWnd, nID);

	ASSERT(GetObjInfoMatcher());
	
	int nItemCount = GetObjInfoMatcher()->GetObjInfoCount();
	SetItemCount(nItemCount);
	//SetDroppedVisibleItemCount(nItemCount > MAX_VISIBLE_COMBOLISTITEM_COUNT ? MAX_VISIBLE_COMBOLISTITEM_COUNT : nItemCount);
	
	return bRet;
}

BOOL CObjInfoListBox::CreateFromCtrl( CWnd* pParent, int nID, DWORD dwStyleAdd /*= 0*/ )
{
	if (!pParent || !pParent->GetSafeHwnd())
		return FALSE;
	CWnd *pCtrl = pParent->GetDlgItem(nID);
	if (!pCtrl)
		return FALSE;
	
	CFont* pFont = pCtrl->GetFont();
	LOGFONT lf = {0};
	if (pFont)
	{
		pFont->GetLogFont(&lf);
	}
	
	UINT style = ::GetWindowLong(pCtrl->GetSafeHwnd(), GWL_STYLE);
	CRect controlRect;
	pCtrl->GetWindowRect(controlRect);
	pParent->ScreenToClient(controlRect);
	pCtrl->DestroyWindow();
	
	BOOL bRet = Create(style | dwStyleAdd, controlRect, pParent, nID);
	
	m_font.CreateFontIndirect(&lf);
	SetFont(&m_font);
	
	return bRet;
}

void CObjInfoListBox::SetObjInfoMatcher( CObjInfoMatcher* pMatcher, BOOL bSubscribeThreadEvent /*= TRUE*/ )
{
	m_pObjInfoMatcher = pMatcher;
	if (m_pObjInfoMatcher && bSubscribeThreadEvent)
	{
		// to bind the thread event.
		Subscribe( m_pObjInfoMatcher->GetObjInfoHolder() );
	}
}

CString CObjInfoListBox::GetItemText( UINT nItem )
{
	if ( GetObjInfoMatcher() )
	{
		WPARAM wParam = GetObjInfoTextInMatchWPARAM(nItem);
		LPARAM lParam = GetObjInfoTextInMatchLPARAM(nItem);
		return GetObjInfoMatcher()->GetObjInfoTextInMatches(wParam, lParam);
	}
	else if ( GetBuddyComboBox() )
	{
		CObjInfoComboBox* pObjInfoComboBox = STATIC_DOWNCAST(CObjInfoComboBox, GetBuddyComboBox());
		return pObjInfoComboBox->GetItemText(nItem);
	}
	return CObjInfoListBoxBase::GetItemText(nItem);
}

void CObjInfoListBox::OnInitListBox()
{
	CObjInfoListBoxBase::OnInitListBox();
	if ( GetIconMapper() )
	{
		SetImageList( &GetIconMapper()->GetImageList() );
	}
}

BOOL CObjInfoListBox::RefreshObjInfoList()
{
	if (!GetObjInfoHolder())
	{
		ASSERT(0);
		return FALSE;
	}

	SetItemCount(0);
	
#ifdef RETRIEVE_OBJINFO_IN_NEWTHREAD
	BOOL bRet = GetObjInfoMatcher()->RunInThread();
#else
	OnObjInfoThreadEvent(CThreadedObj::EVENT_ONSTART, 0);
	BOOL bRet = GetObjInfoMatcher()->RetrieveObjInfo();
	OnObjInfoThreadEvent(CThreadedObj::EVENT_ONFINISH, 0);
#endif // RETRIEVE_OBJINFO_IN_NEWTHREAD
	
	return bRet;
}

void CObjInfoListBox::OnUpdateObjInfoList()
{
	LockSetRedraw(FALSE);
	GetParent()->SendMessage(WM_OBJINFOLIST_UPDATE, (WPARAM)m_hWnd);
	
	SetItemCount(GetObjInfoMatcher()->GetMatchObjInfoCount());
	
	if ( GetObjInfoHolder() )
	{
		int nSortInfoType = GetObjInfoHolder()->GetSortColumn();
		if ( nSortInfoType >= CMultiColInfoHolder::INFOCOL_FIRST )
		{
			GetObjInfoHolder()->Sort(nSortInfoType, GetObjInfoHolder()->IsSortAscending());
		}
	}
	
	//Invalidate();
	
	LockSetRedraw(TRUE);
}

#ifdef UPDATE_OBJINFO_VIA_TIMER
void CObjInfoListBox::OnTimer( UINT nIDEvent )
{
	CObjInfoListBoxBase::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case UPDATE_INFOLIST_TIMER_ID:
		ASSERT(GetObjInfoHolder());
		if ( !GetObjInfoHolder()->IsTaskStopped() )
		{
			OnUpdateObjInfoList();
		}
		break;
	default:
		break;
	}
}
#endif // UPDATE_OBJINFO_VIA_TIMER

void CObjInfoListBox::OnNotify( ISubject* pSubject, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	ASSERT(pSubject);
	if ( NULL == m_hWnd )
		return;
	if ( CThreadedObj::EVENT_ONSTART == wParam )
	{
		SetItemCount(0);
		GetObjInfoMatcher()->Reset();
	}
	// Use PostMessage to avoid deadlock
	PostMessage(WM_OBJINFO_THREADEVENT, wParam, lParam);
}

LRESULT CObjInfoListBox::OnObjInfoThreadEvent( WPARAM wParam, LPARAM lParam )
{
	ASSERT( IsWindow(m_hWnd) );
	switch (wParam)
	{
	case CThreadedObj::EVENT_ONSTART:
#ifdef UPDATE_OBJINFO_VIA_TIMER
		if (m_bUpdateListByTimer && 0 == m_nTimer)
		{
			m_nTimer = SetTimer(UPDATE_INFOLIST_TIMER_ID, UPDATE_INFOLIST_TIMER_ELAPSE, NULL);
		}
#endif // UPDATE_OBJINFO_VIA_TIMER
		break;
	case CThreadedObj::EVENT_ONFINISH:
	case CThreadedObj::EVENT_ONFORCESTOP:
		OnUpdateObjInfoList();
		AutoFitContentHScroll();
#ifdef UPDATE_OBJINFO_VIA_TIMER
		if (m_nTimer > 0)
		{
			KillTimer(m_nTimer);
			m_nTimer = 0;
		}
#endif // UPDATE_OBJINFO_VIA_TIMER
		break;
	default:
		return 0;	// no need to notify the parent for the other events
	}
	// Forward this message to the parent, lParam is not used!
	GetParent()->PostMessage(WM_OBJINFO_THREADEVENT, wParam, (LPARAM)m_hWnd);
	return 1;
}

void CObjInfoListBox::AutoFitContentHScroll()
{
	if ( NULL == m_hWnd )
		return;
	UINT nItemCount = GetItemCount();
	// Find the longest string in the list box.
	CString str;
	CSize   sz;
	int     dx = 0;
	HDC hDC = ::GetDC(m_hWnd);	// CWnd::GetDC() has memory leak bug: http://support.microsoft.com/kb/819635/
	for (UINT nItem = 0; nItem < nItemCount; ++nItem)
	{
		str = GetItemText(nItem);
		::GetTextExtentPoint32(hDC, str, str.GetLength(), &sz);
		
		if (sz.cx > dx)
			dx = sz.cx;
	}
	::ReleaseDC(m_hWnd, hDC);

	if ( GetImageList() )
	{
		int cx, cy;
		ImageList_GetIconSize(GetImageList()->m_hImageList, &cx, &cy);
		dx += cx + 2;
	}
	// Set the horizontal extent so every character of all strings 
	// can be scrolled to.
	SetHorizontalExtent(dx);
}

void CObjInfoListBox::OnDestroyListBox()
{
	//TRACE("======= CObjInfoListBox::OnDestroyListBox()\n");
	if (m_pObjInfoMatcher )
	{
		UnSubscribe( m_pObjInfoMatcher->GetObjInfoHolder());
	}
#ifdef UPDATE_OBJINFO_VIA_TIMER
	if (m_nTimer)
	{
		KillTimer(m_nTimer);
		m_nTimer = 0;
	}
#endif // UPDATE_OBJINFO_VIA_TIMER
	CObjInfoListBoxBase::OnDestroyListBox();
}

int CObjInfoListBox::GetItemIconIndex( UINT nItem )
{
	if ( GetIconMapper() )
	{
		ASSERT(GetObjInfoHolder());
		ObjInfoHolderAccessLock(GetObjInfoHolder());
		return GetIconMapper()->GetIconIndex(GetObjInfoMatcher(), (WPARAM)nItem);
	}
	return CObjInfoListBoxBase::GetItemIconIndex(nItem);
}

void CObjInfoListBox::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if ( VK_F5 == nChar )
	{
		RefreshObjInfoList();
		return;
	}
	CObjInfoListBoxBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*----------------------------------------------------------------------------*/
/* CFileInfoListBox
/*----------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CFileInfoListBox, CFileInfoListBoxBase)

CFileInfoListBox::CFileInfoListBox()
{
}

CFileInfoListBox::~CFileInfoListBox()
{
	
}

BEGIN_MESSAGE_MAP(CFileInfoListBox, CFileInfoListBoxBase)
	ON_MESSAGE(WM_OBJINFO_THREADEVENT, OnObjInfoThreadEvent)
END_MESSAGE_MAP()

IIconMapper* CFileInfoListBox::GetIconMapper() const
{
	if ( !CFileInfoListBoxBase::GetIconMapper() )
	{
		static CFileIconMapper s_FileIconMapper;
		return &s_FileIconMapper;
	}
	return CFileInfoListBoxBase::GetIconMapper();
}

LRESULT CFileInfoListBox::OnObjInfoThreadEvent( WPARAM wParam, LPARAM lParam )
{
	LRESULT result = CFileInfoListBoxBase::OnObjInfoThreadEvent(wParam, lParam);
	if ( wParam == CMultiColInfoHolder::EVENT_SORT_END )
	{
		Invalidate();
		UpdateWindow();
	}
	return result;
}

/*----------------------------------------------------------------------------*/
/* CObjInfoComboBox
/*----------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CObjInfoComboBox, CObjInfoComboBoxBase)

CObjInfoComboBox::CObjInfoComboBox()
	: m_pObjInfoMatcher(NULL)
	, m_pIconMapper(NULL)
	, m_bFilterListOnDropDown(TRUE)
	, m_nTimer(0)
	, m_bUpdateListByTimer(TRUE)
{
	EnableAutoComplete(TRUE);
}

CObjInfoComboBox::~CObjInfoComboBox()
{
	
}

BEGIN_MESSAGE_MAP(CObjInfoComboBox, CObjInfoComboBoxBase)
#ifdef DERIVE_FROM_VCOMBO
	ON_CONTROL_REFLECT(VCBN_DROPDOWN, OnDropDown)
	ON_CONTROL_REFLECT(VCBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT(VCBN_SELENDOK, OnSelEndOK)
	//ON_MESSAGE(VCBM_SETCURSEL, OnSetCurSel)
	ON_CONTROL_REFLECT(VCBN_DISPCHANGE, OnEditDispChange)
#else
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelEndOK)
	ON_MESSAGE(CB_SETCURSEL, OnSetCurSel)
#endif // DERIVE_FROM_VCOMBO

	ON_MESSAGE(WM_OBJINFO_THREADEVENT, OnObjInfoThreadEvent)

#ifdef UPDATE_OBJINFO_VIA_TIMER
	ON_WM_TIMER()
#endif // UPDATE_OBJINFO_VIA_TIMER

	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CObjInfoComboBox::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	BOOL bRet = CObjInfoComboBoxBase::Create(dwStyle, rect, pParentWnd, nID);

	ASSERT(GetObjInfoMatcher());
	
	int nItemCount = GetObjInfoMatcher()->GetObjInfoCount();
	SetItemCount(nItemCount);
	//SetDroppedVisibleItemCount(nItemCount > MAX_VISIBLE_COMBOLISTITEM_COUNT ? MAX_VISIBLE_COMBOLISTITEM_COUNT : nItemCount);
	
	return bRet;
}

void CObjInfoComboBox::SetObjInfoMatcher( CObjInfoMatcher* pMatcher, BOOL bSubscribeThreadEvent /*= TRUE*/ )
{
	m_pObjInfoMatcher = pMatcher;
	if (m_pObjInfoMatcher )
	{
		if ( bSubscribeThreadEvent )
		{
			// to bind the thread event.
			Subscribe( m_pObjInfoMatcher->GetObjInfoHolder());
		}
		if ( GetObjInfoListBox() )
		{
			// We don't want the listbox to have the thread event notify
			GetObjInfoListBox()->SetObjInfoMatcher(pMatcher, FALSE);
		}
	}
}

CString CObjInfoComboBox::GetItemText( UINT nItem )
{
	if ( GetObjInfoMatcher() )
	{
		WPARAM wParam = GetObjInfoTextInMatchWPARAM(nItem);
		LPARAM lParam = GetObjInfoTextInMatchLPARAM(nItem);
		return GetObjInfoMatcher()->GetObjInfoTextInMatches(wParam, lParam);
	}
	return CObjInfoComboBoxBase::GetItemText(nItem);
}

size_t CObjInfoComboBox::GetMatchItemCount( LPCTSTR lpcszPattern, PINT pSelItem /*= NULL*/ )
{
	if ( GetObjInfoMatcher() )
	{
		return GetObjInfoMatcher()->GetMatch(lpcszPattern, (WPARAM)pSelItem);
	}
	return 0;
}

void CObjInfoComboBox::OnNotify( ISubject* pSubject, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/ )
{
	ASSERT(pSubject);
	if ( NULL == m_hWnd )
		return;
	if ( CThreadedObj::EVENT_PRESTART == wParam )
	{
		SetItemCount(0);
		GetObjInfoMatcher()->Reset();
	}
	// Use PostMessage to avoid deadlock
	PostMessage(WM_OBJINFO_THREADEVENT, wParam, lParam);
}

LRESULT CObjInfoComboBox::OnObjInfoThreadEvent( WPARAM wParam, LPARAM lParam )
{
	ASSERT( IsWindow(m_hWnd) );
	switch (wParam)
	{
	case CThreadedObj::EVENT_ONSTART:
#ifdef UPDATE_OBJINFO_VIA_TIMER
		if (m_bUpdateListByTimer && 0 == m_nTimer)
		{
			m_nTimer = SetTimer(UPDATE_INFOLIST_TIMER_ID, UPDATE_INFOLIST_TIMER_ELAPSE, NULL);
		}
#endif // UPDATE_OBJINFO_VIA_TIMER
		break;
	case CThreadedObj::EVENT_ONFINISH:
	case CThreadedObj::EVENT_ONFORCESTOP:
		OnUpdateObjInfoList();
#ifdef UPDATE_OBJINFO_VIA_TIMER
		if (m_nTimer > 0)
		{
			KillTimer(m_nTimer);
			m_nTimer = 0;
		}
#endif // UPDATE_OBJINFO_VIA_TIMER
		break;
	default:
		return 0;	// no need to notify the parent for the other events
	}
	// Forward this message to the parent, lParam is not used!
	GetParent()->PostMessage(WM_OBJINFO_THREADEVENT, wParam, (LPARAM)m_hWnd);
	return 1;
}

BOOL CObjInfoComboBox::OnInitCombo()
{
	BOOL bRet = CObjInfoComboBoxBase::OnInitCombo();
	ASSERT(GetObjInfoListBox());	// you have to subclass the listbox!
	GetObjInfoListBox()->SetBuddyComboBox(this);

	if ( GetIconMapper() )
	{
		//SetImageList( &GetIconMapper()->GetImageList() );
		GetObjInfoListBox()->SetIconMapper( GetIconMapper() );
		GetObjInfoListBox()->SetImageList( &GetIconMapper()->GetImageList() );
	}
	return bRet;
}

void CObjInfoComboBox::OnDestroyCombo()
{
	//TRACE("======= CObjInfoComboBox::OnDestroyCombo()\n");
	if (m_pObjInfoMatcher )
	{
		UnSubscribe( m_pObjInfoMatcher->GetObjInfoHolder());
	}
#ifdef UPDATE_OBJINFO_VIA_TIMER
	if (m_nTimer)
	{
		KillTimer(m_nTimer);
		m_nTimer = 0;
	}
#endif // UPDATE_OBJINFO_VIA_TIMER
	CObjInfoComboBoxBase::OnDestroyCombo();
}

void CObjInfoComboBox::OnDropDown()
{
	if (m_bAutoComplete)
	{
		// We have to do this, otherwise the cursor will somehow disappear
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		if ( m_bFilterListOnDropDown )
		{
			int nCurSel = GetCurSel();
			BOOL bHasSel = nCurSel >= 0;
			if ( !bHasSel )
				nCurSel = 0;

			int nMatchCount = 0;
			int nSel = nCurSel;
			CString strPattern;
			if ( !bHasSel )
			{
				GetWindowText(strPattern);
				nMatchCount = GetMatchItemCount(strPattern, &nSel);
				nCurSel = nSel;
			}
			if ( nMatchCount <= 0 )
			{
				nMatchCount = GetMatchItemCount(NULL, &nSel);
				SetItemCount(nMatchCount);
			}
			//nCurSel = nSel;

			SetDroppedVisibleItemCount(nMatchCount > MAX_VISIBLE_COMBOLISTITEM_COUNT ? MAX_VISIBLE_COMBOLISTITEM_COUNT : nMatchCount);
			SetTopIndex(nCurSel);

			if ( bHasSel )
			{
// 				GetComboListBox().SetCurSel(nCurSel);
// 				SetTextOfItem(nCurSel);
				SetCurSel(nCurSel);
			}
		}
	}
}

LRESULT CObjInfoComboBox::OnSetCurSel( WPARAM nIndex, LPARAM )
{
#ifdef DERIVE_FROM_VCOMBO
	LRESULT result = CObjInfoComboBoxBase::OnSetCurSel(nIndex, 0);
#else
	LRESULT result = Default();
#endif // DERIVE_FROM_VCOMBO
	//TRACE1("======= CObjInfoComboBox::OnSetCurSel(%d)\n", nIndex);
	int nStartChar, nEndChar;
	GetComboEdit().GetSel(nStartChar, nEndChar);

	if (m_bAutoComplete)
	{
#if defined(USE_HOOK_CHANGE_LISTBOX_STYLE) && !defined(DERIVE_FROM_VCOMBO)
		GetComboEdit().SetSel(nStartChar, nEndChar);
#else
		GetComboEdit().SetSel(nEndChar, nEndChar);
#endif // USE_HOOK_CHANGE_LISTBOX_STYLE
	}
	return result;
}


void CObjInfoComboBox::OnEditDispChange()
{
	CString strPattern;
	GetWindowText(strPattern);
	int nCurSel = GetCurSel();
	BOOL bHasSel = nCurSel >= 0;
	if ( !bHasSel )
		nCurSel = 0;
	int nMatchCount = GetMatchItemCount(strPattern, &nCurSel);

	SetItemCount(nMatchCount);
	SetDroppedVisibleItemCount(nMatchCount > MAX_VISIBLE_COMBOLISTITEM_COUNT ? MAX_VISIBLE_COMBOLISTITEM_COUNT : nMatchCount);

	if ( nMatchCount <= 0 )
	{
		return;
	}
	
	if (m_bAutoComplete)
	{
		if ( !GetComboListBox().IsWindowVisible() )
		{
			m_bFilterListOnDropDown = FALSE;
			ShowDropDown(TRUE);
			m_bFilterListOnDropDown = TRUE;
		}
		//GetComboListBox().SetCurSel(nCurSel);
		SetTopIndex(nCurSel);
		if ( bHasSel || !strPattern.IsEmpty() )
		{
			GetComboListBox().SetCurSel(nCurSel);
		}
	}
}

#ifndef DERIVE_FROM_VCOMBO
void CObjInfoComboBox::OnEditUpdate()
{
	OnEditDispChange();
}
#endif // DERIVE_FROM_VCOMBO

void CObjInfoComboBox::OnSelChange()
{
	//TRACE1("======= CObjInfoComboBox::OnSelChange(%d)\n", GetCurSel());
	Default();
#ifndef DERIVE_FROM_VCOMBO
	int nCurSel = GetCurSel();
	ASSERT(nCurSel >= 0);
	SetTextOfItem(nCurSel);
#endif // DERIVE_FROM_VCOMBO
}

void CObjInfoComboBox::OnSelEndOK()
{
	//TRACE("======= CObjInfoComboBox::OnSelEndOK()\n");
	Default();
#ifndef DERIVE_FROM_VCOMBO
	if (m_bAutoComplete)
	{
		int nCurSel = GetCurSel();
		if (nCurSel >= 0)
		{
			SetTextOfItem(nCurSel);
		}
	}
#endif // DERIVE_FROM_VCOMBO
}

int CObjInfoComboBox::GetItemIconIndex( UINT nItem )
{
	if ( GetIconMapper() )
	{
		ASSERT(GetObjInfoHolder());
		ObjInfoHolderAccessLock(GetObjInfoHolder());
		return GetIconMapper()->GetIconIndex(GetObjInfoMatcher(), (WPARAM)nItem);
	}
	return CObjInfoComboBoxBase::GetItemIconIndex(nItem);
}

BOOL CObjInfoComboBox::RefreshObjInfoList()
{
	if (!GetObjInfoHolder())
	{
		ASSERT(0);
		return FALSE;
	}

	SetItemCount(0);
	
#ifdef RETRIEVE_OBJINFO_IN_NEWTHREAD
	BOOL bRet = GetObjInfoMatcher()->RunInThread();
#else
	OnObjInfoThreadEvent(CThreadedObj::EVENT_ONSTART, 0);
	BOOL bRet = GetObjInfoMatcher()->RetrieveObjInfo();
	OnObjInfoThreadEvent(CThreadedObj::EVENT_ONFINISH, 0);
#endif // RETRIEVE_OBJINFO_IN_NEWTHREAD
	
	return bRet;
}

void CObjInfoComboBox::OnUpdateObjInfoList()
{
	LockSetRedraw(FALSE);
	GetParent()->SendMessage(WM_OBJINFOLIST_UPDATE, (WPARAM)m_hWnd);

	const size_t nObjInfoCount = GetObjInfoMatcher()->GetMatchObjInfoCount();
	SetItemCount(nObjInfoCount);
	if ( GetObjInfoListBox() )
	{
		GetObjInfoListBox()->AutoFitContentHScroll();
	}

	LockSetRedraw(TRUE);
}

void CObjInfoComboBox::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if ( VK_F5 == nChar )
	{
		RefreshObjInfoList();
		return;
	}
	CObjInfoComboBoxBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

#ifdef UPDATE_OBJINFO_VIA_TIMER
void CObjInfoComboBox::OnTimer( UINT nIDEvent )
{
	CObjInfoComboBoxBase::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case UPDATE_INFOLIST_TIMER_ID:
		ASSERT(GetObjInfoHolder());
		if ( !GetObjInfoHolder()->IsTaskStopped() )
		{
			//if ( GetObjInfoMatcher()->GetMatchObjInfoCount() > GetItemCount() )
			OnUpdateObjInfoList();
		}
		break;
	default:
		break;
	}
}
#endif // UPDATE_OBJINFO_VIA_TIMER

void CObjInfoComboBox::SetIconMapper( IIconMapper* pIconMapper )
{
	m_pIconMapper = pIconMapper;
	if ( GetObjInfoListBox() )
	{
		GetObjInfoListBox()->SetIconMapper(pIconMapper);
	}
}

/*----------------------------------------------------------------------------*/
/* CFileInfoComboBox
/*----------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CFileInfoComboBox, CFileInfoComboBoxBase)

CFileInfoComboBox::CFileInfoComboBox()
{
}

CFileInfoComboBox::~CFileInfoComboBox()
{
	
}

BEGIN_MESSAGE_MAP(CFileInfoComboBox, CFileInfoComboBoxBase)
	ON_MESSAGE(WM_OBJINFO_THREADEVENT, OnObjInfoThreadEvent)
END_MESSAGE_MAP()

IIconMapper* CFileInfoComboBox::GetIconMapper() const
{
	if ( !CFileInfoComboBoxBase::GetIconMapper() )
	{
		static CFileIconMapper s_FileIconMapper;
		return &s_FileIconMapper;
	}
	return CFileInfoComboBoxBase::GetIconMapper();
}

LRESULT CFileInfoComboBox::OnObjInfoThreadEvent( WPARAM wParam, LPARAM lParam )
{
	LRESULT result = CFileInfoComboBoxBase::OnObjInfoThreadEvent(wParam, lParam);
	if ( wParam == CMultiColInfoHolder::EVENT_SORT_END )
	{
		if ( GetComboListBox().IsWindowVisible() )
		{
			GetComboListBox().Invalidate();
			GetComboListBox().UpdateWindow();
		}
	}
	return result;
}

CObjInfoListBox* CFileInfoComboBox::GetObjInfoListBox()
{
	return &m_FileInfoListBox;
}