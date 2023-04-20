#include "stdafx.h"
#include "EditAgentDlg.h"
#include "../classes/TempStringLoader.h"

CEditAgentDlg::CEditAgentDlg(Agent *pAgent,CWnd* pParent/* = NULL*/) : CAgentDlg(pParent)
{
	ASSERT(pAgent);
	m_pAgent = pAgent;
}

bool CEditAgentDlg::CommitAgent()
{
	m_pAgent->ChangeAgentName(m_AgentName);
	m_pAgent->ChangeAgentCode(m_AgentCode);
	m_pAgent->ChangePlateNo(m_AgentPlateNo);
	return true;
}

CString CEditAgentDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_EDITAGENTCAPTION);
}

void CEditAgentDlg::InitDialogControls()
{
	m_AgentName = m_pAgent->GetAgentName();
	m_AgentCode = m_pAgent->GetAgentCode();
	m_AgentPlateNo = m_pAgent->GetPlateNo();
	UpdateData(FALSE);
}