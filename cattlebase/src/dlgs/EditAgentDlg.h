#ifndef __EDITAGENTDLG_H__
#define __EDITAGENTDLG_H__

#include "AgentDlg.h"
#include "../datatypes/Agent.h"

class CEditAgentDlg : public CAgentDlg
{
public:
	CEditAgentDlg(Agent *pAgent,CWnd* pParent = NULL);
protected:
	virtual bool CommitAgent();
	virtual CString GetWindowCaption();
	virtual void InitDialogControls();
	
	Agent *m_pAgent;


};

#endif