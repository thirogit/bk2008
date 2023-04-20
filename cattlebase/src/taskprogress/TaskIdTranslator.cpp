#include "stdafx.h"
#include "TaskIdTranslator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString TaskIdTranslator::TraslateTaskId(UINT taskId)
{
	CString	taskName;
	UINT nResStr = Translate(taskId);

	if(nResStr != 0)
	{
		taskName.LoadString(nResStr);
	}
	else
	{
		taskName.Format(IDS_NOTASKNAME,taskId);	
	}
	return taskName;
}
void TaskIdTranslator::Add(UINT taskId,UINT nResStrId)
{
	m_translateMap.SetAt(taskId,nResStrId);
}

UINT TaskIdTranslator::Translate(UINT taskId)
{
	UINT nResStr;

	if(m_translateMap.Lookup(taskId,nResStr))
	{
		return nResStr;
	}
	return 0;
}
