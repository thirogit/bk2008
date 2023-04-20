#ifndef __REPOPURCHASE_H__
#define __REPOPURCHASE_H__

#include "RepoInvoice.h"
#include "NewPtrFlushArray.h"

class RepoPurchase
{
public:	
	RepoPurchase(UINT Id);
	RepoPurchase(const RepoPurchase& src);
	~RepoPurchase();
	void CopyFrom(const RepoPurchase& src);

	const CString& GetAgentCode() const;	
	const CString& GetPlateNo() const;
	const CDateTime& GetStartDate() const;
	const CDateTime& GetEndDate() const;
	bool  GetWasDownloaded() const;

	UINT GetId() const;
	int GetCowCount() const;
	 
	int GetInvoiceCount() const;
	RepoInvoice* GetInvoice(int iInvoice);
	const RepoInvoice* GetInvoice(int iInvoice) const;
	void AddInvoice(RepoInvoice* pInvoice);

	int GetHentCount() const;
	RepoHent* GetHent(int iHent);
	void AddHent(RepoHent* pHent);

	void SetAgentCode(const CString& sAgentCode);	
	void SetPlateNo(const CString& sPlateNo);
	void SetStartDate(const CDateTime& startDt);
	void SetEndDate(const CDateTime& endDt);
	void SetWasDownloaded(bool bDownloaded);

private:
	CString m_AgentCode;
	CString m_PlateNo;
	CDateTime m_StartDate;
	CDateTime m_EndDate;
	UINT m_PuracheId;	
	bool m_Downloaded;

	NewPtrFlushArray<RepoHent> m_Hents;
	NewPtrFlushArray<RepoInvoice> m_Invoices;	

};
	

#endif