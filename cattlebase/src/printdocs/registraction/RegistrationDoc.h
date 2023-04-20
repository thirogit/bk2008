#ifndef __REGISTRATIONDOC_H__
#define __REGISTRATIONDOC_H__

#include "../../DataTypes.h"
#include "../../printing/CowDrwDoc.h"
#include "../PrintDocsCfg.h"

#include <boost/shared_ptr.hpp>


class RegistrationRecord
{
public:
	RegistrationRecord();

	void SetCows(CowArrayPtr cows);
	int GetCowCount();
	CowArrayPtr GetCows();

private:
	CowArrayPtr m_Cows;
};

class RegistrationDoc : public CCowDrwDoc
{
protected:
	RegistrationDoc();
public:

	virtual void PreparePage(int nPage);
	virtual int Load() ;
	virtual void FillEANs(int iStart,int iEnd,int iEANLblStart);
	virtual void FillOneEAN(const CString &strEAN,LPCSTR EANLblRowFmt);
	virtual void FillCowRecord(Cow* pCow,int iRow);
	virtual int GetPrintPages() const = 0;
	virtual ~RegistrationDoc();
	
protected:
	void FillBottomOfPage(RegistrationRecord *pRecord);
	void FillHeaderOfPage();
	void PutPrintDate(const CDateTime& printDt);
	void PutDate(const CDateTime& dt,LPCSTR prefix,int row);

	void PutChar(const CString& sLabel,char c);
	
	virtual void ComputeIndexes() = 0;
	
	virtual int GetNumberOfRecordsPerSheet() = 0;
	virtual int LoadInputData(CPtrArray &inputElements);
	
	int m_nSheets;
	
	PtrFlushArray<RegistrationRecord> m_records;
	
	CCowArray m_currCows;	
	RegistrationRecord *m_pCurrentRecord;
	int m_currStartIdx;
	int m_currEndIdx;
	int m_currDocPage;	
};


#endif
