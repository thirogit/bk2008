#include "stdafx.h"
#include "TaskProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTaskProgressDialog::CTaskProgressDialog(const CString& caption,TaskIdTranslator *taskTranslator/* = NULL*/)
{
	if(caption.IsEmpty())
		m_Caption.LoadString(IDS_PROGRESS_CAPTION);
	else
		m_Caption = caption;

    m_bCancel		= FALSE;
    m_nLower		= 0;
    m_nUpper		= 100;
    m_nStep			= 1;
//	m_nCurTaskID	= 0;
    m_bParentDisabled = FALSE;
		
	m_taskTranslator = taskTranslator;
	if(!m_taskTranslator) m_taskTranslator = &m_defaultTranslator;
}

/////////////////////////////////////////////////////////////////////////////
//
CTaskProgressDialog::~CTaskProgressDialog()
{
    if(m_hWnd != NULL)
		DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CTaskProgressDialog::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
//
void CTaskProgressDialog::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
BOOL CTaskProgressDialog::Create(CWnd *pParent)
{
    // Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);    

    return CDialog::Create(CTaskProgressDialog::IDD,pParent);
    //{
      //ReEnableParent();
    //  return FALSE;
    //}

    //return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CTaskProgressDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
   
    DDX_Control(pDX, IDC_PROGDLG_PROGRESS, m_Progress);

}

BEGIN_MESSAGE_MAP(CTaskProgressDialog, CDialog)

	ON_MESSAGE(WM_PROGRESSDLG_SETTASK,OnMessageSetTask)
	ON_MESSAGE(WM_PROGRESSDLG_ONSTEP,OnMessageOnStep)
	ON_MESSAGE(PROGRESSDLG_WM_TASKERROR,OnMessageTaskError)
	ON_WM_SHOWWINDOW()

END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
//
void CTaskProgressDialog::OnCancel()
{
    m_bCancel = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CTaskProgressDialog::SetRange(int nLower,int nUpper)
{
    m_nLower = nLower;
    m_nUpper = nUpper;
    m_Progress.SetRange(nLower,nUpper);
}
  
/////////////////////////////////////////////////////////////////////////////
//
int CTaskProgressDialog::SetPos(int nPos)
{
    PumpMessages();
    int iResult = m_Progress.SetPos(nPos);
    UpdatePercent(nPos);
    return iResult;
}

/////////////////////////////////////////////////////////////////////////////
//
int CTaskProgressDialog::SetStep(int nStep)
{
    m_nStep = nStep; // Store for later use in calculating percentage
    return m_Progress.SetStep(nStep);
}

int CTaskProgressDialog::OffsetPos(int nPos)
{
    PumpMessages();
    int iResult = m_Progress.OffsetPos(nPos);
    UpdatePercent(iResult+nPos);
    return iResult;
}

/////////////////////////////////////////////////////////////////////////////
//
int CTaskProgressDialog::StepIt()
{
    PumpMessages();
    int iResult = m_Progress.StepIt();
    UpdatePercent(iResult+m_nStep);
    return iResult;
}

/////////////////////////////////////////////////////////////////////////////
//
void CTaskProgressDialog::PumpMessages()
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
      }
    }
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CTaskProgressDialog::CheckCancelButton()
{
    // Process all pending messages
    PumpMessages();

    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    BOOL bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}

/////////////////////////////////////////////////////////////////////////////
//
void CTaskProgressDialog::UpdatePercent(int nNewPos)
{
    
    int nPercent;
    
    int nDivisor = m_nUpper - m_nLower;
    

    int nDividend = (nNewPos - m_nLower);
    

	if(nDividend >= 0 && nDivisor > 0)
		nPercent = nDividend * 100 / nDivisor;
	else
		nPercent = 100;

    // Since the Progress Control wraps, we will wrap the percentage
    // along with it. However, don't reset 100% back to 0%
    if(nPercent != 100) nPercent %= 100;

    // Display the percentage
    CString strBuf;
    strBuf.Format(_T("%d%%"),nPercent);

	SetDlgItemText(IDC_PROGDLG_PERCENT,strBuf);

	
}
    
/////////////////////////////////////////////////////////////////////////////
// CTaskProgressDialog message handlers

BOOL CTaskProgressDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();

    m_Progress.SetRange(m_nLower,m_nUpper);
    m_Progress.SetStep(m_nStep);
    m_Progress.SetPos(m_nLower);

    SetWindowText(m_Caption);

	UpdateData(FALSE);
    return TRUE;  
}

void CTaskProgressDialog::SetTaskTranslator(TaskIdTranslator *taskTranslator)
{
	m_taskTranslator = taskTranslator;
	if(!m_taskTranslator) m_taskTranslator = &m_defaultTranslator;
}

void CTaskProgressDialog::SetCurTask(int nID,int nMaxSteps)
{
	CString taskName = m_taskTranslator->TraslateTaskId(nID);
	SetDlgItemText(IDC_TASKNAME,(LPCSTR)taskName);
	SetRange(0,nMaxSteps);
	SetPos(0);
}



void CTaskProgressDialog::SetTask(UINT taskID,int steps)
{
	PostMessage(WM_PROGRESSDLG_SETTASK,taskID,steps);
	PumpMessages();
}
bool CTaskProgressDialog::OnStep(int step)
{
	PostMessage(WM_PROGRESSDLG_ONSTEP,step);
	PumpMessages();
	return m_bCancel == FALSE;
}
void CTaskProgressDialog::OnTaskError(const CString& sError)
{
	SendMessage(PROGRESSDLG_WM_TASKERROR,(WPARAM)(LPCSTR)sError);
	PumpMessages();
}

LRESULT CTaskProgressDialog::OnMessageSetTask(WPARAM wParam,LPARAM lParam)
{
	SetCurTask(wParam,lParam);
	return (LRESULT)0;
}
LRESULT CTaskProgressDialog::OnMessageOnStep(WPARAM wParam,LPARAM lParam)
{

	SetPos(wParam);
	return (LRESULT)0;
}
LRESULT CTaskProgressDialog::OnMessageTaskError(WPARAM wParam,LPARAM lParam)
{
	m_sLastError = (LPCSTR)wParam; 
	return (LRESULT)0;
}

void CTaskProgressDialog::OnShowWindow(BOOL bShow,UINT nStatus)
{

	// m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.
    if(m_pParentWnd != NULL)
    {
      m_pParentWnd->EnableWindow(!bShow);
      m_bParentDisabled = bShow;
    }

}
const CString& CTaskProgressDialog::GetLastError()
{
	return m_sLastError;
}
