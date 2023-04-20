#include "stdafx.h"
#include "Agents.h"
#include "../global_arrays.h"
#include "../classes/TempStringLoader.h"
#include "NewAgentDlg.h"
#include "EditAgentDlg.h"
#include "../CattleSQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAgentsDlg, CDialog)
	ON_BN_CLICKED(IDC_ADDBTN, OnAddAgent)
	ON_BN_CLICKED(IDC_DELBTN, OnDelAgent)
	ON_BN_CLICKED(IDC_EDITBTN, OnEditAgent)
	ON_NOTIFY(NM_DBLCLK , IDC_LIST, OnDblClick)
END_MESSAGE_MAP()


CAgentsDlg::CAgentsDlg(CWnd* pParent/* = NULL*/) : CDialog(CAgentsDlg::IDD,pParent)
{
}

void CAgentsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control (pDX, IDOK  , m_OKBtn ) ;
	DDX_Control (pDX, IDC_LIST  , m_AgentList ) ;
	DDX_Control (pDX, IDC_EDITBTN,m_EditBtn);
	DDX_Control (pDX, IDC_ADDBTN,m_AddBtn);
	DDX_Control (pDX, IDC_DELBTN,m_DeleteBtn);
}


BOOL CAgentsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_OKBtn.SetButtonImage( IDB_STOCKOK);	
	m_AddBtn.SetButtonImage( IDB_STOCKADD);	
	m_DeleteBtn.SetButtonImage( IDB_STOCKDEL);	
	m_EditBtn.SetButtonImage( IDB_STOCKEDIT);	

	SetWindowText(CTempStringLoader(IDS_EDITAGENTS));
	return TRUE;
}


void  CAgentsDlg::OnAddAgent()
{	
	if(!(HAS_PRIVLEDGE(ADDMODOWN)))
	{
		AfxMessageBox(IDS_NOPRIVS);
		return;
	}
	
	CNewAgentDlg newAgentDlg(this);
	if(newAgentDlg.DoModal() == IDOK)
	{
		Agent* pNewAgent = newAgentDlg.GetNewAgent();

		if(pNewAgent->InsertThisAgent(true))
		{
			m_AgentList.Lock();
			ary_agents.Add(pNewAgent);
			m_AgentList.UpdateItemCount();
			m_AgentList.Unlock();			
			return;
		}
		else
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGAGENT);
		}
	}
	
	
}

void  CAgentsDlg::OnDelAgent()
{
	
	int iSelAgent = -1;

	if(!(HAS_PRIVLEDGE(DELFORG) || HAS_PRIVLEDGE(DELOWN) ))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}
	iSelAgent = m_AgentList.GetCurSel();
	if(iSelAgent < 0) return;

	m_AgentList.Lock();
	Agent* pAgent = ary_agents[iSelAgent];
	//if(agentPtr.Count() > 2) //one in array, one here
	//{
	//	AfxMessageBox(IDS_AGENTRECORDISUSED);
	//}
	//else
	//{
		if(pAgent->DeleteThisAgent())
		{
			ary_agents.RemoveAt(iSelAgent);
			m_AgentList.UpdateItemCount();
			delete pAgent;			
		}
		else
		{
			SQLDB.SQLMessageBox(IDS_ERRDELETINGAGENT);
		}
	//}
	
	m_AgentList.Unlock();
	
}


void CAgentsDlg::OnEditAgent()
{
	int iSelAgent = -1;
	Agent* pAgent = NULL;

	if(!HAS_PRIVLEDGE(ADDMODOWN))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}

	iSelAgent = m_AgentList.GetCurSel();
	if(iSelAgent < 0) return;

	pAgent = ary_agents[iSelAgent];
	CEditAgentDlg editAgentDlg(pAgent,this);
	if(editAgentDlg.DoModal() == IDOK)
	{
		if(!pAgent->UpdateThisAgent())
		{
			SQLDB.SQLMessageBox(IDS_ERRUPDATEINGAGENT);
			pAgent->RollbackChanges();
		}
		else
		{
			pAgent->CommitChanges();
			m_AgentList.Invalidate(FALSE);
		}
	}	
	

}
void CAgentsDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnEditAgent();
	*pResult = 0;
}
