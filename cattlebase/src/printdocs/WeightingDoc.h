#ifndef __WEIGHTINGDOC_H__
#define __WEIGHTINGDOC_H__

#include "../printing/CowDrwDoc.h"
#include "../DataTypes.h"

#define WEIGHTINGDOCPAGE_1_SIZE 33
#define WEIGHTINGDOCPAGE_2_SIZE 44

class CWeightingDoc : public CCowDrwDoc
{
public:
	CWeightingDoc();
	
	int GetPrintPages() const;
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
	void PrintPage(int page);
protected:
	CString FormatHent(const Hent* hent);

	virtual void PrintRecord(Cow *pCow,int iRow,int iIndex);
		
	virtual const Hent* GetSource() = 0;
	virtual const Hent* GetDestination() = 0;

	CDocDataArray m_docs;
	Doc *currDoc;
	CCowArray m_currDocCows;
	int starti;
	int endi;
	int pageOff;
	int m_nPages;
	int currDocLastPage;
	int curDocPageSize;
};


class CWeightingInDoc : public CWeightingDoc
{
	DECLARE_DYNCREATE(CWeightingInDoc)
public:
	CWeightingInDoc();
protected:
	
	virtual const Hent* GetSource();
	virtual const Hent* GetDestination();
};

class CWeightingOutDoc : public CWeightingDoc
{
	DECLARE_DYNCREATE(CWeightingOutDoc)
public:
	CWeightingOutDoc();
protected:
	
	virtual const Hent* GetSource();
	virtual const Hent* GetDestination();
};


#endif
