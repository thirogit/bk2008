#include "stdafx.h"
#include "NewAgentDlg.h"
#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNewAgentDlg::CNewAgentDlg(CWnd* pParent/* = NULL*/) : CAgentDlg(pParent),m_NewAgent(NULL)
{
}

CString CNewAgentDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_NEWAGENT);
}

Agent *CNewAgentDlg::GetNewAgent()
{
	return m_NewAgent;
}

bool CNewAgentDlg::CommitAgent()
{
	Agent *newAgent = new Agent(NULL_AGENTID);
	newAgent->SetAgentCode(m_AgentCode);
	newAgent->SetAgentName(m_AgentName);
	newAgent->SetPlateNo(m_AgentPlateNo);
	m_NewAgent = newAgent;
	return true;
}