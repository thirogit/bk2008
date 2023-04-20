#include "stdafx.h"
#include "TipEdit.h"
#include "global_arrays.h"


BEGIN_MESSAGE_MAP(CTipEdit,COXMaskedEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP();

CTipEdit::CTipEdit(LPCSTR pszMask/*= ""*/) : COXMaskedEdit(pszMask),pAbortWnd(NULL)
{	
}


void CTipEdit::SetAbortWnd(CWnd *pWnd)
{
	pAbortWnd = pWnd;
}

/*
TTI_NONE No icon.
TTI_INFO Info icon.
TTI_WARNING Warning icon
TTI_ERROR Error Icon
TTI_INFO_LARGE Large error Icon
TTI_WARNING_LARGE Large error Icon
TTI_ERROR_LARGE Large error Icon
*/

void CTipEdit::CreateToolTip (CWnd *pParent, const TCHAR *pszText,
	const int iIconType/* = TTI_NONE*/, const TCHAR *pszTitle/* = _T("")*/)
{
	TOOLINFO ti;
	DWORD dwStyle = TTS_BALLOON;

	m_ToolTip.Create (pParent, dwStyle);
	m_ToolTip.FillInToolInfo (ti, this, 0);
	ti.uFlags |= TTF_TRACK | TTF_CENTERTIP | TTF_ABSOLUTE;
	ti.lpszText = (LPTSTR) pszText;
#if _MSC_VER >= 1310
	m_ToolTip.SetTitle (iIconType, pszTitle);
#endif
	m_ToolTip.SendMessage (TTM_ADDTOOL, 0, reinterpret_cast<LPARAM> (&ti));
}

void CTipEdit::SetToolTipText (const TCHAR *pszText)
{
	m_ToolTip.UpdateTipText (pszText, this);
}

void CTipEdit::SetToolTipTitle (const int iIconType, const TCHAR *pszTitle)
{
#if _MSC_VER >= 1310
	m_ToolTip.SetTitle (iIconType, pszTitle);
#endif
}

void CTipEdit::DisplayToolTip (const bool bDisplay)
{
	if (m_ToolTip.m_hWnd)
	{
		CToolInfo ti;

		m_ToolTip.GetToolInfo (ti, this, 0);

		if (bDisplay)
		{
			RECT ClientRect;
			int iHeight = 0;
			int iOffset = 0;

			GetRect (&ClientRect);
			iHeight = ClientRect.bottom - ClientRect.top;
			iOffset = (ClientRect.right - ClientRect.left) / 2;
			::GetWindowRect (m_hWnd, &ti.rect);
			ti.rect.top += iHeight;
			ti.rect.bottom += iHeight;
			ti.rect.left += iOffset;
			ti.rect.right += iOffset;

			m_ToolTip.SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (ti.rect.left,
				ti.rect.top));
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, TRUE,
				reinterpret_cast<LPARAM> (&ti));
		}
		else
		{
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, FALSE,
				reinterpret_cast<LPARAM> (&ti));
		}
	}
}

void CTipEdit::PrepareToolTip(CWnd *pParent)
{
	CreateToolTip(pParent,"Tooltip text",2,"Tooltip");
}


void CTipEdit::PreSubclassWindow()
{
	COXMaskedEdit::PreSubclassWindow();
	PrepareToolTip(GetParent());
}

void CTipEdit::OnKillFocus (CWnd *pNewWnd)
{
	DisplayToolTip (FALSE);
	// Don't allow cursor to disappear...
	if ((pNewWnd && pNewWnd->m_hWnd != m_ToolTip.m_hWnd))
	{
		if(pAbortWnd && pNewWnd->m_hWnd == pAbortWnd->m_hWnd) return;

		COXMaskedEdit::OnKillFocus (pNewWnd);
	}
}

void CTipEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	DisplayToolTip(FALSE);
	COXMaskedEdit::OnKeyDown(nChar,nRepCnt,nFlags);

}
int CTipEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(COXMaskedEdit::OnCreate(lpCreateStruct) == -1) return -1;

	PrepareToolTip(GetParent());
	return 0;
}
void CTipEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	DisplayToolTip(FALSE);
	COXMaskedEdit::OnContextMenu(pWnd,point);
}
