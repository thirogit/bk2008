#include "stdafx.h"
#include "WarnPayWayCombo.h"
#include "../classes/TempIconLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	

#define ID_PAYWAYCOMBO 1001
#define DEFAULT_BLINK_INTERVAL 600
#define COMBOW  80
#define COMBOH  22
#define COMBODROPH 92

BEGIN_MESSAGE_MAP(CWarnPaywayCombo, CStatic)

		ON_CBN_SELCHANGE(ID_PAYWAYCOMBO, OnPaywayChange)
		ON_WM_SETFOCUS()
		ON_WM_KILLFOCUS()
		ON_WM_ENABLE()
		ON_WM_PAINT()

END_MESSAGE_MAP()


CWarnPaywayCombo::CWarnPaywayCombo() : m_warnBlinkIntervalMili(DEFAULT_BLINK_INTERVAL),m_bWarn(false)
{
	
}

void CWarnPaywayCombo::SetHent(Hent *pHent)
{
	ASSERT(pHent);
	m_pHent = pHent;
}

BOOL CWarnPaywayCombo::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CStatic::Create("",WS_CHILD,rect,pParentWnd,nID);
}

BOOL CWarnPaywayCombo::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	if (lpszWindowName == NULL)     lpszWindowName = "";
	if(CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		InitControls();
		PosControls();
		return TRUE;
	}
	return FALSE;
}


BOOL CWarnPaywayCombo::InitControls()
{
	CWnd* pParent = GetParent();
	if (pParent != NULL)
		SetFont(pParent->GetFont());

	if(!m_payway.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST  | WS_VSCROLL, CRect(0, 0, 0, 0), this, ID_PAYWAYCOMBO))
		return FALSE;

	m_payway.SetFont(GetFont());
	m_payway.EnableWindow(TRUE);
	m_payway.ShowWindow(SW_SHOWNOACTIVATE);

	m_warnBitmap.DeleteImageList();
	m_warnBitmap.Create(16,16,ILC_COLOR8|ILC_MASK,0,1);
	m_warnBitmap.SetBkColor(CLR_NONE);
	m_warnBitmap.Add(CTempIconLoader(IDI_EXCLAMATIONMARK));

	return TRUE;
}

void CWarnPaywayCombo::PosControls(void)
{
	

	CRect rc;
	GetClientRect(rc);
	int cx = rc.Width();
	int cy = rc.Height();
		
	m_payway.MoveWindow(0,(cy-COMBOH)/2,COMBOW,COMBODROPH);
}

void CWarnPaywayCombo::OnPaint()
{
	CStatic::OnPaint();

	if(m_bWarn)
	{
		CClientDC dc(this);
		CRect rc;
		GetClientRect(rc);
		DrawIconEx(dc.GetSafeHdc(),COMBOW+5,(rc.Height()-16)/2,
				m_warnBitmap.ExtractIcon(0),0, 0, 0, NULL, DI_NORMAL);
	}

}

void CWarnPaywayCombo::SetWarn(bool bWarn)
{
	m_bWarn = bWarn;
	Invalidate();
}

void CWarnPaywayCombo::OnPaywayChange()
{
	if(m_pHent != NULL)
	{
		if(GetPayway() == Invoice::PAY_TRANS &&
			m_pHent->GetAccountNo().IsEmpty())
		{
			SetWarn(true);
			return;
		}
	}
	SetWarn(false);
}

void CWarnPaywayCombo::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP);
	InitControls();
	PosControls();
}


void CWarnPaywayCombo::OnSetFocus(CWnd* pOldWnd) 
{
	m_payway.SetFocus();
	CStatic::OnSetFocus(pOldWnd);
}

void CWarnPaywayCombo::OnKillFocus(CWnd* pNewWnd)
{
  CStatic::OnKillFocus(pNewWnd);
  m_payway.SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
  
}

void CWarnPaywayCombo::OnEnable(BOOL bEnable) 
{
	CStatic::OnEnable(bEnable);

	m_payway.EnableWindow(bEnable);

	SendMessage(WM_SETREDRAW, TRUE, 0);
	RedrawWindow( NULL, NULL,          RDW_INVALIDATE | RDW_ALLCHILDREN);
}


void CWarnPaywayCombo::ReloadCombo(Invoice::PAYWAYS curSel/* = InvoiceData::PAY_TRANS*/)
{
	m_payway.ReloadCombo(curSel);
	OnPaywayChange();
}

void CWarnPaywayCombo::SetWarnBlinkInterval(int miliseconds)
{
	if(miliseconds < 0)
		m_warnBlinkIntervalMili = DEFAULT_BLINK_INTERVAL;
	else
		m_warnBlinkIntervalMili = miliseconds; 
}

Invoice::PAYWAYS CWarnPaywayCombo::GetPayway()
{
	return (Invoice::PAYWAYS)m_payway.GetItemData(m_payway.GetCurSel());
}

int CWarnPaywayCombo::GetCurSel()
{
	return m_payway.GetCurSel();
}
	
