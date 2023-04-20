#if !defined(__WAITDIALOG_H__)
#define __WAITDIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WaitDlgThread.h"
#include "../taskprogress/ProgressIFace.h"

class CWait : public ITaskProgres
{
public:
	CWait();           
	void SetWaitText(const CString& waitText);
	void ShowCancelButton();
	void Close();
	void Show();
	virtual ~CWait();

	virtual void SetTask(UINT taskID,int steps);
	virtual bool OnStep(int step);
	virtual void OnTaskError(UINT errorCode);

private:
	CWaitDlgThread*		m_Thread;
	bool			    m_bShowCancelButton;	
	CString				m_Text;
	
};

#endif 
