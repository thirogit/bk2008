#include "stdafx.h"
#include "AgentListCtrl.h"
#include "DataTypes.h"
#include "MainCfg.h"
#include "../datalimits.h"
#include "../global_arrays.h"
#include "../datatypes/AgentComparator.h"


const GRIDHEADERS CAgentListCtrl::szColHeaders[] =
{
	{IDS_HEADERSHORT,AGENT_MEMBER_CODE},
	{IDS_HEADERNAME,AGENT_MEMBER_NAME},
	{IDS_HEADERPLATE,AGENT_MEMBER_PLATE},	
	{0,0}
};

BEGIN_MESSAGE_MAP(CAgentListCtrl, CBaseListCtrl)
   END_MESSAGE_MAP()

CAgentListCtrl::CAgentListCtrl() : CBaseListCtrl(szColHeaders,CFGID_AGENTLIST) 
{

}
void CAgentListCtrl::UpdateItemCount()
{
	SetItemCount(ary_agents.GetSize());
}

void CAgentListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	CString sText;
	Agent *pAgent = ary_agents[nItem];
	switch(GetColID(nSubItem))
	{
	case AGENT_MEMBER_CODE:
		sText = pAgent->GetAgentCode();
	break;
	case AGENT_MEMBER_NAME:
		sText = pAgent->GetAgentName();
	break;	
	case AGENT_MEMBER_PLATE:
		sText = pAgent->GetPlateNo();
	break;
	default:
		ASSERT(0);
	}
	strcpy(pszItemText,sText);
}

void CAgentListCtrl::SortByCol(int Column,bool bAsc)
{
	//ary_agents.SortBy(GetColID(Column),bAsc);
}
