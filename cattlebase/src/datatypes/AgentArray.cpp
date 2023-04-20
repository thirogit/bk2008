#include "stdafx.h"
#include "AgentArray.h"

Agent* CAgentArray::GetAgentByAgentId(int Id)
{
	Agent *temp;
	for(int i = 0,s = GetSize();i< s;i++)
	{
		temp = (Agent*)GetAt(i); 
		if(temp->GetId() == Id) return temp;
	}
	return NULL;		
}


