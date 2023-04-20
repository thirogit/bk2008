#include "stdafx.h"
#include "AgentDlg.h"
#include "../datalimits.h"

CAgentDlg::CAgentDlg(CWnd* pParent/* = NULL*/) : 
	CDialog(CAgentDlg::IDD,pParent)
{
}



void CAgentDlg::InitDialogControls()
{
	
}

void CAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_AGENTCODE,m_AgentCode);
	DDX_Text(pDX,IDC_AGENTNAME,m_AgentName);
	DDX_Text(pDX,IDC_AGENTPLATENO,m_AgentPlateNo);
}

BOOL CAgentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText((LPCSTR)GetWindowCaption());
	SendDlgItemMessage(IDC_AGENTCODE,EM_LIMITTEXT,MAXAGENTCODE);
	SendDlgItemMessage(IDC_AGENTNAME,EM_LIMITTEXT,MAXAGENTNAME);
	SendDlgItemMessage(IDC_AGENTPLATENO,EM_LIMITTEXT,MAXAGENTPLATENO);

	InitDialogControls();
	return TRUE;
}

bool CAgentDlg::ValidateFields()
{
	if(m_AgentName.GetLength() == 0)
	{
		AfxMessageBox(IDS_EMPTYAGENTNAME);
		GetDlgItem(IDC_AGENTNAME)->SetFocus();
		return false;
	}

	if(m_AgentCode.GetLength() == 0)
	{
		AfxMessageBox(IDS_EMPTYAGENTCODE);
		GetDlgItem(IDC_AGENTCODE)->SetFocus();
		return false;
	}

	return true;
}

void CAgentDlg::OnOK()
{
	UpdateData();

	if(!ValidateFields())
		return;
	
	if(CommitAgent())
		EndDialog(IDOK);

}
