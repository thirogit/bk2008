#include "stdafx.h"
#include "SyncRejectedDlg.h"

// CSyncRejectedDlg dialog

IMPLEMENT_DYNAMIC(CSyncRejectedDlg, CPropertyPage)
CSyncRejectedDlg::CSyncRejectedDlg(UINT nCaptionId)
	: CPropertyPage(CSyncRejectedDlg::IDD, nCaptionId)
{	
	m_CaptionId = nCaptionId;
}
CSyncRejectedDlg::~CSyncRejectedDlg()
{
}

void CSyncRejectedDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	
}

/*
LRESULT CSyncRejectedDlg::OnRefreshRejected(WPARAM wParam,LPARAM lParam)
{
	DoRefresh();
	return (LRESULT)0;
}*/


BEGIN_MESSAGE_MAP(CSyncRejectedDlg, CPropertyPage)

	ON_NOTIFY(NM_CLICK,IDC_REJECTEDLIST,OnListClick)
	
END_MESSAGE_MAP()


UINT CSyncRejectedDlg::GetCaptionId()
{
	return m_CaptionId;
}


BOOL CSyncRejectedDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	return TRUE;
}
void CSyncRejectedDlg::OnListClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE plvItemActive = (LPNMITEMACTIVATE)pNMHDR;
    
	if(plvItemActive->iItem >= 0)
	{
		m_rejectedText =  FormatRejectedReasonsText(plvItemActive->iItem);
		GetParent()->GetParent()->SendMessage(WM_SETREJECTTEXT,(WPARAM)(LPCSTR)m_rejectedText);
	}
	*pResult = 0;
}

CString CSyncRejectedDlg::GetRejectedReasonsText()
{
	return m_rejectedText;
}