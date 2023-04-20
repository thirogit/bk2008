#include "stdafx.h"
#include "ComboBoxImg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SEARCH_EXPIRE_TIME 3000

///////////////////////////////////////////////////////////////////////////////
// CComboBoxImg

IMPLEMENT_DYNAMIC(CComboBoxImg, CComboBoxEx)

BEGIN_MESSAGE_MAP(CComboBoxImg, CComboBoxEx)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,OnCBKillFocus)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CComboBoxImg::CComboBoxImg() : m_LastKeyPressTick(0)
{
}

CComboBoxImg::~CComboBoxImg()
{
}

int CComboBoxImg::AddItem(LPCTSTR pszText, int iImage)
{
	COMBOBOXEXITEM cbi = {0};
	cbi.mask = CBEIF_TEXT;
	cbi.iItem = -1;
	cbi.pszText = (LPTSTR)pszText;
	if (iImage != -1)
	{
		cbi.mask |= CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
		cbi.iImage = iImage;
		cbi.iSelectedImage = iImage;
	}
	return InsertItem(&cbi);
}


void CComboBoxImg::UpdateHorzExtent(int iIconWidth)
{	
	int iItemCount = GetCount();
	if (iItemCount > 0)
	{
		CDC *pDC = GetDC();
		if (pDC != NULL)
		{
			// *** To get *ACCURATE* results from 'GetOutputTextExtent' one *MUST*
			// *** explicitly set the font!
			CFont *pOldFont = pDC->SelectObject(GetFont());

			CString strItem;
			int iMaxWidth = 0;
			for (int i = 0; i < iItemCount; i++)
			{
				GetLBText(i, strItem);
				int iItemWidth = pDC->GetOutputTextExtent(strItem, strItem.GetLength()).cx;
				if (iItemWidth > iMaxWidth)
					iMaxWidth = iItemWidth;
			}
			
			pDC->SelectObject(pOldFont);
			ReleaseDC(pDC);

			// Depending on the string (lot of "M" or lot of "i") sometime the
			// width is just a few pixels too small!
			iMaxWidth += 4;
			if (iIconWidth)
				iMaxWidth += 2 + iIconWidth + 2;
			SetHorizontalExtent(iMaxWidth);
			if (GetDroppedWidth() < iMaxWidth)
				SetDroppedWidth(iMaxWidth);
		}
	}
	else
		SetHorizontalExtent(0);
}

void CComboBoxImg::OnCBKillFocus()
{
	m_itemSearchBuffer.Empty();
	DisplayToolTip(false);	
}

void CComboBoxImg::OnCloseup() 
{
	m_itemSearchBuffer.Empty();
	DisplayToolTip(false);

}


BOOL CComboBoxImg::PreTranslateMessage(MSG* pMsg)
{
	// there seems to be no way that we get the WM_CHARTOITEM for this control
	ASSERT( pMsg->message != WM_CHARTOITEM );

	if (pMsg->message == WM_KEYDOWN)
	{
		UINT uChar = MapVirtualKey(pMsg->wParam, 2);
		if((GetTickCount() - m_LastKeyPressTick) > SEARCH_EXPIRE_TIME)
		{				
			m_itemSearchBuffer.Empty();				
		}
		m_LastKeyPressTick = GetTickCount();

		if(pMsg->wParam == VK_BACK && m_itemSearchBuffer.GetLength() > 0)
		{
			m_itemSearchBuffer.Delete(m_itemSearchBuffer.GetLength()-1);	
			SelectFirstStringIfStartsWith(m_itemSearchBuffer);
			return TRUE;
		}
		
		
		if (isgraph(uChar))
		{
			UINT uTheChar        = (UINT)(_TUCHAR)_totlower((_TINT)((UINT)uChar));	
			CString serachBuffer(m_itemSearchBuffer);
			serachBuffer.AppendChar(uChar);
			if(SelectFirstStringIfStartsWith(serachBuffer))
				m_itemSearchBuffer.AppendChar(uChar);

			return TRUE;
		}

		
		
		
		
		
		

			
			
		
	}
	return CComboBoxEx::PreTranslateMessage(pMsg);
}

bool CComboBoxImg::SelectFirstStringIfStartsWith(const CString& startStr)
{
		SetToolTipText((LPCSTR)startStr);
		DisplayToolTip(true);

		int iCount = GetCount();
		for (int i = 0; i < iCount; i++)
		{
			CString strItem;
			GetLBText(i, strItem);
			if (strItem.IsEmpty())	continue;

			if(strItem.GetLength() > startStr.GetLength())
			{					
				strItem.Delete(startStr.GetLength(),
					strItem.GetLength()-startStr.GetLength());
			}				
			
			if (!startStr.CompareNoCase(strItem))
			{
				SetCurSel(i);				
				return true;
			}
		}
		return false;
}


BOOL CComboBoxImg::SelectString(LPCTSTR pszText)
{
	// CComboBox::SelectString seems also not to work
	CComboBox* pctrlCB = GetComboBoxCtrl();
	if (pctrlCB != NULL)
	{
		int iCount = pctrlCB->GetCount();
		for (int i = 0; i < iCount; i++)
		{
			CString strItem;
			pctrlCB->GetLBText(i, strItem);
			if (strItem == pszText)
			{
				SetCurSel(i);
				//GetParent()->SendMessage(WM_COMMAND, MAKELONG((WORD)GetWindowLong(m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_hWnd);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CComboBoxImg::CreateToolTip()
{
	m_ToolTip.Create(this,TTS_NOPREFIX | TTS_ALWAYSTIP);

	// initialize toolinfo struct
	memset(&m_ToolInfo, 0, sizeof(m_ToolInfo));
	m_ToolInfo.cbSize = sizeof(m_ToolInfo);
	m_ToolInfo.uFlags = TTF_TRACK | TTF_TRANSPARENT;
	m_ToolInfo.hwnd = m_hWnd;

	m_ToolTip.SetMaxTipWidth(SHRT_MAX);
	m_ToolTip.SendMessage( TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ToolInfo);
	m_ToolTip.SetTipBkColor(::GetSysColor(COLOR_HIGHLIGHT));
	m_ToolTip.SetTipTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

	// reduce top & bottom margins
	CRect rectMargins(0,-1,0,-1);
	m_ToolTip.SetMargin(&rectMargins);

	// set font
	CFont *pFont = GetFont();
	m_ToolTip.SetFont(pFont, FALSE);



}

void CComboBoxImg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rectClient;
	GetClientRect(&rectClient);
	int nComboButtonWidth = ::GetSystemMetrics(SM_CXHTHUMB) + 2;
	rectClient.right = rectClient.right - nComboButtonWidth;

	if (rectClient.PtInRect(point))
	{
		TRACE(_T("in ccombo\n"));
		ClientToScreen(&rectClient);

		CString strText = _T("zawsze dupa");		
		m_ToolInfo.lpszText = (LPTSTR)(LPCTSTR)strText;

		
			rectClient.left += 1;
			rectClient.top += 3;

			COLORREF rgbText = ::GetSysColor(COLOR_WINDOWTEXT);
			COLORREF rgbBackground = ::GetSysColor(COLOR_WINDOW);

			CWnd *pWnd = GetFocus();
			if (pWnd)
			{
				if (pWnd->m_hWnd == m_hWnd)
				{
					rgbText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
					rgbBackground = ::GetSysColor(COLOR_HIGHLIGHT);
				}
			}

			m_ToolTip.SetTipBkColor(rgbBackground);
			m_ToolTip.SetTipTextColor( rgbText);

			m_ToolTip.SendMessage( TTM_UPDATETIPTEXT, 0, (LPARAM) &m_ToolInfo);
			
			
			m_ToolTip.SendMessage( TTM_TRACKPOSITION, 0, 
				(LPARAM) MAKELONG(rectClient.left, rectClient.top));
			m_ToolTip.SendMessage( TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
			TRACE(_T("setting timer\n"));
			//SetTimer(1, 80, NULL);	// set timer for out-of-rect detection
		
	}
	else	// not inside client rect
	{
		m_ToolTip.SendMessage( TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
	}

	CComboBoxEx::OnMouseMove(nFlags, point);
}



int CComboBoxImg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CComboBoxEx::OnCreate(lpCreateStruct) == -1) return -1;
	CreateToolTip();	
	return 0;	
}
void CComboBoxImg::PreSubclassWindow() 
{
	CComboBoxEx::PreSubclassWindow();	
	CreateToolTip();
}

void CComboBoxImg::SetToolTipText (const TCHAR *pszText)
{
	if (m_ToolTip.m_hWnd)
	{
		m_ToolInfo.lpszText = (LPTSTR)(LPCTSTR)pszText;
		m_ToolTip.SendMessage( TTM_UPDATETIPTEXT, 0, (LPARAM) &m_ToolInfo);
	}
			
}

void CComboBoxImg::DisplayToolTip (const bool bDisplay)
{
	if (m_ToolTip.m_hWnd)
	{
		if (bDisplay)
		{
			CRect rectClient;
			GetClientRect(&rectClient);
			ClientToScreen(&rectClient);

			m_ToolTip.SendMessage( TTM_TRACKPOSITION, 0, 
				(LPARAM) MAKELONG(rectClient.left, rectClient.top));
			m_ToolTip.SendMessage( TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);


		}
		else
		{
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, FALSE,
				reinterpret_cast<LPARAM> (&m_ToolInfo));
		}
	}
}



