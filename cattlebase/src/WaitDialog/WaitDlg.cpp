// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WaitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWaitDlg::CWaitDlg(CWnd* pParent/* = NULL*/)	: CDialog(CWaitDlg::IDD, pParent)
{	
   m_bEnableAbortButton = false;
   m_bAborted = false;
}

void CWaitDlg::SetWaitText(const CString& waitText)
{
	m_Text = waitText;
}
void CWaitDlg::EnableAbortButton(bool bEnable)
{
	m_bEnableAbortButton = true;
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TASKPROGRESS,m_Progress);
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)

	ON_MESSAGE(WM_PROGRESS_SETTASK,OnSetTask)
	ON_MESSAGE(WM_PROGRESS_ONSTEP,OnStep)
	ON_MESSAGE(WM_PROGRESS_ONERROR,OnError)
	ON_BN_CLICKED(IDC_ABORTBTN,OnAbort)

END_MESSAGE_MAP()


BOOL CWaitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   if (m_Text.GetLength() > 0)
      GetDlgItem(IDC_BUSY_TEXT)->SetWindowText(m_Text);

   GetDlgItem(IDC_ABORTBTN)->EnableWindow(m_bEnableAbortButton);
	
   return TRUE;  
}

LRESULT CWaitDlg::OnSetTask(WPARAM wParam,LPARAM lParam)
{
	SetDlgItemText(IDC_BUSY_TEXT,(LPCSTR)wParam);
	m_Progress.SetRange(0,(short)lParam);
	m_Progress.SetPos(0);

	return (LRESULT)0;
}
LRESULT CWaitDlg::OnError(WPARAM wParam,LPARAM lParam)
{
	return (LRESULT)(m_bAborted ? 1 : 0);
}
LRESULT CWaitDlg::OnStep(WPARAM wParam,LPARAM lParam)
{
	m_Progress.SetPos(wParam);
	return (LRESULT)0;
}

void CWaitDlg::OnAbort()
{
	m_bAborted = true;
}


