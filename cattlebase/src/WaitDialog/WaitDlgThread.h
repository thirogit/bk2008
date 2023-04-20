#if !defined(__WAITDLGTHREAD_H__)
#define __WAITDLGTHREAD_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxmt.h>
#include "WaitDlg.h"

#define TM_EXITTHREAD (WM_APP+1001)

class CWaitDlgThread : public CWinThread
{
	DECLARE_DYNCREATE(CWaitDlgThread)

public:
   CWaitDlgThread();
   virtual ~CWaitDlgThread();
   
   void SetShowCancelButton(bool bShowCancel);
   void SetWaitText(const CString& waitText);
      
protected:
   virtual BOOL InitInstance();

   bool     m_bShowCancelButton;
   CString  m_Text;
   CWaitDlg *m_pWaitDlg;

   void OnExitThread(WPARAM wParam,LPARAM lParam);
   DECLARE_MESSAGE_MAP()
};

#endif 
