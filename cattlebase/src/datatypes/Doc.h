#ifndef __DOC_H__
#define __DOC_H__

#include "RawData.h"
#include "Hent.h"
#include "CowDataArray.h"
#include "Cow.h"
#include "Reason.h"
#include "AgentArray.h"
#include "DataMemberPtr.h"

#define NULL_DOCID (0)

class VATInvoice;

typedef DataMember<VATInvoice*,VATInvoice*> VATInvoiceDataMember;
typedef DataMember<Reason*,Reason*> ReasonDataMember;
typedef DataMember<Agent*,Agent*> AgentDataMember;


class Doc : public RawData
{
public:

	Doc(int docId);
	virtual ~Doc();
	void CopyFrom(const Doc &copy);
	
	int	GetId() const;
	void ResetId();
	CString GetDocNoStr() const;
	bool IsNew() const;

	Hent* GetHent() const;
	void SetHent(Hent* pHent);
	void ChangeHent(Hent* pHent);

	CDateTime GetDocDate() const;
	void SetDocDate(const CDateTime& docDt);
	void ChangeDocDate(const CDateTime& docDt);

	CDateTime GetLoadDate() const;
	void SetLoadDate(const CDateTime& loadDt);
	void ChangeLoadDate(const CDateTime& loadDt);
		
	CString GetExtras() const;
	void SetExtras(const CString& sExtras);
	void ChangeExtras(const CString& sExtras);

	CString GetPlateNo() const;
	void SetPlateNo(const CString& plateNo);
	void ChangePlateNo(const CString& plateNo);
	
	Reason* GetReason() const;
	void SetReason(Reason* pReason);
	void ChangeReason(Reason* pReason);

	Time GetLoadStartTime() const;
	void SetLoadStartTime(const Time& loadStrTm);
	void ChangeLoadStartTime(const Time& loadStrTm);
	
	Time GetLoadEndTime() const;
	void SetLoadEndTime(const Time& loadEndTm);
	void ChangeLoadEndTime(const Time& loadEndTm);

	Agent* GetAgent() const;
	void SetAgent(Agent* pAgent);
	void ChangeAgent(Agent* pAgent);
	
	VATInvoice* GetInvoice() const;
	void SetInvoice(VATInvoice* pInvoice);
	void AssignToInvoice(VATInvoice* pInvoice);
	void DeleteInvoice();
	bool HasInvoice() const;

	UINT GetCowCount() const;
	CowArrayPtr GetCows();

	virtual bool InsertThisDoc(bool bCommit = true);
	virtual bool UpdateThisDoc(bool bCommit = true);
	virtual bool DeleteThisDoc(bool bCommit = true);

	bool CanDeleteThisDoc() const;
	bool CanEditThisDoc() const;
	virtual bool CanMakeVATInvoiceOnThisDoc() const;

	void CommitChanges();
	void RollbackChanges();
	bool IsUncommited();
protected:
	virtual CString GetDocPrefix() const = 0;
	virtual CString GetTableName() const = 0;

protected:
	int						docid;
	
	HentDataMember			hent;
	DateTimeDataMember		docdate;
	DateTimeDataMember		loaddate;
	StringDataMember		extras;
	StringDataMember		plateno;
	ReasonDataMember		reason;
	VATInvoiceDataMember	invoice;
	TimeDataMember			loadstart;
	TimeDataMember			loadend;
	AgentDataMember			agent;

	CCowArray				cows;

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;

};

#endif