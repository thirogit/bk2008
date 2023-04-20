#ifndef __HEALTHCERTDOC_H__
#define __HEALTHCERTDOC_H__

#include "../printing/CowDrwDoc.h"
#include "global_arrays.h"

#define HEALTHCERTDOCSIZE 40

class CHealthCertDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(CHealthCertDoc)
public:
	CHealthCertDoc();
	int GetPrintPages() const;
	int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
	void PrintPage(int page);
		
private:
	CDocDataArray m_docs;
	Doc *m_currDoc;
	int starti;
	int endi;
	int pageOff;
	int m_nPages;
	int m_currDocPageSize;
	CCowArray m_currDocCows;
};






#endif
