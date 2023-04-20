#ifndef __AGENTTEXTPROVIDER_H__
#define __AGENTTEXTPROVIDER_H__

#include "BaseTextProvider.h"
#include "../datatypes/Agent.h"

class AgentTextProvider : public BaseTextProvider<Agent>
{
public:
	virtual CString GetDataText(Agent *pData,UINT memberID);

};

#endif