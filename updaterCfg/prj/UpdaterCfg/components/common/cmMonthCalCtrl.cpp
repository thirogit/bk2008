// cmMonthCalCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "cmMonthCalCtrl.h"
#include "cmDateCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cmMonthCalCtrl

cmMonthCalCtrl::cmMonthCalCtrl()
{
}

cmMonthCalCtrl::~cmMonthCalCtrl()
{
}


BEGIN_MESSAGE_MAP(cmMonthCalCtrl, CMonthCalCtrl)
	//{{AFX_MSG_MAP(cmMonthCalCtrl)
	ON_NOTIFY_REFLECT(MCN_SELECT, OnSelect)
	ON_NOTIFY_REFLECT(MCN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmMonthCalCtrl message handlers

void cmMonthCalCtrl::SetParentCell(cmDateCell* parent)
{
	parentCell_ = parent;
}

void cmMonthCalCtrl::OnSelect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMSELCHANGE* p;
	
	p = (NMSELCHANGE*)pNMHDR;

	ChangeSelection(p->stSelStart);
		
	*pResult = 0;
}

void cmMonthCalCtrl::SetDate(const COleDateTime& date)
{
	date_ = date;
	if (GetSafeHwnd() && (date.m_status == COleDateTime::DateTimeStatus::valid))
		SetCurSel(date_);
}

COleDateTime cmMonthCalCtrl::GetDate() const
{
	return date_;
}

void cmMonthCalCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

BOOL cmMonthCalCtrl::PreTranslateMessage(MSG* pMsg) 
{
	BOOL ret;

	ret = CMonthCalCtrl::PreTranslateMessage(pMsg);
	
	if (pMsg->message == WM_LBUTTONDOWN) {
		MCHITTESTINFO testInfo;
		DWORD hitResult;
		CPoint pt;
		CRect rc;

		pt = pMsg->pt;
		GetWindowRect(rc);
		if (!rc.PtInRect(pt)) {
			parentCell_->KillFocus(true, true);
			ret = TRUE;
		} else {
			ScreenToClient(&pt);
			memset(&testInfo, 0, sizeof(MCHITTESTINFO));
			testInfo.cbSize = sizeof(MCHITTESTINFO);
			testInfo.pt = pt;
			
			hitResult = HitTest(&testInfo);
			
			switch (hitResult) {
				case MCHT_TODAYLINK:
					::GetLocalTime(&testInfo.st);
					ChangeSelection(testInfo.st);	
					ret = TRUE;
					break;

				case MCHT_CALENDARDATE:
					ChangeSelection(testInfo.st);	
					ret = TRUE;
					break;
			}
		}
		
		return ret;
	}
		
	return ret;
}

void cmMonthCalCtrl::ChangeSelection(SYSTEMTIME& t)
{
	date_ = t;
	parentCell_->KillFocus(true, true);
}

void cmMonthCalCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CMonthCalCtrl::OnKillFocus(pNewWnd);
	
	
	
}
