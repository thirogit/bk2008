// DateSpan1.cpp : implementation file
//

#include "stdafx.h"

#include "DateSpan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateSpan

#define DestroyControl(ctrl) \
	if (ctrl != NULL) { ctrl->DestroyWindow(); delete ctrl; ctrl = NULL; }

//enum { ID_LABEL  = 1, ID_DATEEDIT1, ID_SEPARATOR, ID_DATEEDIT2 };


enum { ID_LABEL = 0x987, ID_DATEEDIT1, ID_SEPLABEL, ID_DATEEDIT2, ID_COMBO };


CDateSpan::CDateSpan() :
CStatic(),
//m_pLabel(NULL), 
m_pDate1(NULL), 
m_pSeparator(NULL), 
m_pDate2(NULL),
m_pCombo(NULL),
m_StrLabel("Od/Do: "),
m_StrSeparator("-"),
m_StrBetween("Miêdzy"),
m_StrExcept("Oprócz"),
m_StrBefore("Przed"),
m_StrAfter("Po")
{

	
}

CDateSpan::~CDateSpan()
{

//DestroyControl(m_pLabel);
DestroyControl(m_pDate1);
DestroyControl(m_pSeparator);
DestroyControl(m_pDate2);
DestroyControl(m_pCombo);
}


BEGIN_MESSAGE_MAP(CDateSpan, CStatic)
	//{{AFX_MSG_MAP(CDateSpan)
		ON_CBN_SELCHANGE(ID_COMBO, OnSpanCombo)
		ON_WM_SETFOCUS()
		ON_WM_KILLFOCUS()
		ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateSpan message handlers
BOOL CDateSpan::Create(LPCSTR lpszLabel,LPCSTR lpszSeparator,const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	VERIFY(lpszLabel && lpszSeparator);

	m_StrLabel = lpszLabel;
	m_StrSeparator = lpszSeparator;
	return Create("",WS_CHILD,rect,pParentWnd,nID);
	
}


BOOL CDateSpan::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID/*=0xffff*/)
{
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
	
}

BOOL CDateSpan::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
  if (lpszWindowName == NULL)     lpszWindowName = "";
  BOOL bOK = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

InitControls();
PosControls();
  return bOK;
}

/*void CDateSpan::SetLabelText(LPCTSTR lpszString)
{
  // sets the text of the static (label) control
  if (lpszString != NULL)
  {
	  m_StrLabel = lpszString;
	  m_pLabel->SetWindowText(lpszString);
	  PosControls();
	  
  }
}*/



BOOL CDateSpan::InitControls()
{

		CWnd* pParent = GetParent();
  if (pParent != NULL)
    SetFont(pParent->GetFont());


  m_pCombo = new CComboBox;
    if (m_pCombo == NULL || !m_pCombo->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST  | WS_VSCROLL, CRect(0, 0, 0, 0), this, ID_COMBO))
      return FALSE;
  
  m_pCombo->SetFont(GetFont());
  m_pCombo->EnableWindow(TRUE);
  m_pCombo->ShowWindow(SW_SHOWNOACTIVATE);
   // m_pCombo->SendMessage(WM_KILLFOCUS,0,0);

  int item;
  item = m_pCombo->AddString(m_StrBetween);
  m_pCombo->SetItemData(item,SPAN_BETWEEN);


  item = m_pCombo->AddString(m_StrExcept);
  m_pCombo->SetItemData(item,SPAN_EXCEPT);

  item = m_pCombo->AddString(m_StrBefore);
  m_pCombo->SetItemData(item,SPAN_BEFORE);

  item = m_pCombo->AddString(m_StrAfter);
  m_pCombo->SetItemData(item,SPAN_AFTER);

  m_pCombo->SetCurSel(0);

   m_pDate1 = new CDateTimeCtrl;
    if (m_pDate1 == NULL || !m_pDate1->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT | DTS_RIGHTALIGN, CRect(0, 0, 0, 0), this, ID_DATEEDIT1))
      return FALSE;
  
  m_pDate1->SetFont(GetFont());
  m_pDate1->EnableWindow(TRUE);
  m_pDate1->ShowWindow(SW_SHOWNOACTIVATE);

  
 // m_pDate1->SendMessage(WM_KILLFOCUS,0,0);


  m_pSeparator = new CStatic;
    if (m_pSeparator == NULL || !m_pSeparator->Create((LPCSTR)m_StrSeparator, WS_CHILD |  WS_VISIBLE |  SS_NOPREFIX , CRect(0, 0, 0, 0), this, ID_SEPLABEL))
      return FALSE;
  
  m_pSeparator->SetFont(GetFont());
  m_pSeparator->EnableWindow(TRUE);

  m_pSeparator->ShowWindow(SW_SHOWNOACTIVATE);




 m_pDate2 = new CDateTimeCtrl;
    if (m_pDate2 == NULL || !m_pDate2->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT | DTS_RIGHTALIGN, CRect(0, 0, 0, 0), this, ID_DATEEDIT2))
      return FALSE;
  
  m_pDate2->SetFont(GetFont());
  m_pDate2->EnableWindow(TRUE);
  m_pDate2->ShowWindow(SW_SHOWNOACTIVATE);
   // m_pDate2->SendMessage(WM_KILLFOCUS,0,0);



  

  


return TRUE;
}

void CDateSpan::PosControls(void)
{

	const int GAP = 3;
	const int DATEW = 80;
	const int COMBOW = 60;
	const int COMBOH = 92;

  CRect rc;
  GetClientRect(rc);
  int cx = rc.Width();
  int cy = rc.Height();
  int posx,posy;
  CDC * pDC;
  CSize size;

/*  pDC = m_pLabel->GetDC();
  size = pDC->GetTextExtent(m_StrLabel);*/

/*  posy = (cy - size.cy) /2;
  posx = 0;
  m_pLabel->MoveWindow(posx, posy,size.cx , size.cy);
  m_pLabel->ReleaseDC(pDC);
  
  
  posx += size.cx + GAP;*/

  posx = 0;
  posy = rc.Width()/2;


  m_pCombo->MoveWindow(posx,(cy-22)/2,COMBOW,COMBOH);

  posx += COMBOW + GAP;

  
  /*pDC = m_pDate1->GetDC();
  
  TEXTMETRIC tm;

  pDC->GetTextMetrics(&tm);
  tm.tmHeight;
  release not detachpDC->Detach();*/

  m_pDate1->MoveWindow(posx,(cy-22)/2,DATEW,22);
  posx += DATEW + GAP;

  

  pDC = m_pSeparator->GetDC();
  size = pDC->GetTextExtent(m_StrSeparator);

  posy = (cy - size.cy)/2;
  m_pSeparator->MoveWindow(posx,posy,size.cx,size.cy);
  m_pSeparator->ReleaseDC(pDC);
  posx += size.cx+GAP;

  m_pDate2->MoveWindow(posx,(cy-22)/2,DATEW,22);


}

void CDateSpan::OnSpanCombo()
{

	int item = m_pCombo->GetCurSel();

	if(item >= 0)
	{
		switch(m_pCombo->GetItemData(item))
		{
		case SPAN_AFTER:
		case SPAN_BEFORE:
			m_pDate2->EnableWindow(FALSE);
			break;

		case SPAN_BETWEEN:
		case SPAN_EXCEPT:
			m_pDate2->EnableWindow(TRUE);
			break;
		}
	}
}



TIMESPAN CDateSpan::GetSpan(CDateTime &t1, CDateTime &t2)
{
	CTime time1,time2;
	TIMESPAN spantype = (TIMESPAN)m_pCombo->GetItemData(m_pCombo->GetCurSel());
	m_pDate1->GetTime(time1);
	t1.SetYearMonthDay(time1.GetYear(),time1.GetMonth(),time1.GetDay());
	if(spantype != SPAN_BEFORE && spantype != SPAN_AFTER)
	{
		m_pDate2->GetTime(time2);
		t2.SetYearMonthDay(time2.GetYear(),time2.GetMonth(),time2.GetDay());		
	}
	else t2 = t1; 
	
	return spantype;
}


void CDateSpan::PreSubclassWindow()
{

	CStatic::PreSubclassWindow();

	ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP);
	InitControls();
	PosControls();
}


void CDateSpan::OnSetFocus(CWnd* pOldWnd) 
{
	m_pCombo->SetFocus();
	CStatic::OnSetFocus(pOldWnd);

}

void CDateSpan::OnKillFocus(CWnd* pNewWnd)
{
  CStatic::OnKillFocus(pNewWnd);
 
  m_pCombo->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
  m_pDate1->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
  m_pDate2->SendMessage(WM_KILLFOCUS,(WPARAM)pNewWnd,(LPARAM)0);
}

void CDateSpan::OnEnable(BOOL bEnable) 
{
  CStatic::OnEnable(bEnable);


m_pDate1->EnableWindow(bEnable);
m_pSeparator->EnableWindow(bEnable);
m_pDate2->EnableWindow(bEnable);
m_pCombo->EnableWindow(bEnable);


 SendMessage(WM_SETREDRAW, TRUE, 0);
 RedrawWindow( NULL, NULL,          RDW_INVALIDATE | RDW_ALLCHILDREN);



}