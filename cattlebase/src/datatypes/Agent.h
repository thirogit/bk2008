#ifndef __AGENT_H__
#define __AGENT_H__

#include "DataMember.h"
#include "DataMembersDefs.h"
#define NULL_AGENTID (0)


class Agent
{
public:

	Agent(UINT Id = NULL_AGENTID);
	~Agent();
	virtual void CopyFrom(const Agent& src);	

	UINT GetId() const;

	CString GetAgentCode() const;
	CString GetAgentName() const;
	CString GetPlateNo() const;

	void SetAgentCode(const CString& sAgentCode);
	void SetAgentName(const CString& sAgentName);
	void SetPlateNo(const CString& sPlateNo);

	void ChangeAgentCode(const CString& sAgentCode);
	void ChangeAgentName(const CString& sAgentName);
	void ChangePlateNo(const CString& sPlateNo);

	
	bool InsertThisAgent(bool bCommit = true);
	bool DeleteThisAgent(bool bCommit = true);
	bool UpdateThisAgent(bool bCommit = true);

	void CommitChanges();
	void RollbackChanges();
	bool IsUncommited();
protected:
	UINT agentId;
	StringDataMember agentcode;
	StringDataMember agentname;
	StringDataMember plate;

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;

};




#endif