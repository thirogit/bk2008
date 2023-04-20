#ifndef __NEWAGENTDLG_H__
#define __NEWAGENTDLG_H__

#include "AgentDlg.h"

class CNewAgentDlg : public CAgentDlg
{
public:
	CNewAgentDlg(CWnd* pParent = NULL);
	Agent *GetNewAgent();
protected:
	virtual CString GetWindowCaption();
	virtual bool CommitAgent();
protected:
	Agent* m_NewAgent;
};



#endif