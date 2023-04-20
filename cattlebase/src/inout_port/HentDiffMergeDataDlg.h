#ifndef __HENTDIFFMERGEDATADLG_H__
#define __HENTDIFFMERGEDATADLG_H__

#include "../datatypes/HentComparator.h"
#include "../textproviders/HentTextProvider.h"
#include "DataDiffMerge.h"
#include "DiffMergeDataDlg.h"

typedef CDataDiffMerge<Hent,HENTDATA_MEMBERS,HentComparator,HentTextProvider> CHentDiffMerge;

class CHentDiffMergeDataDlg : public CDiffMergeDataDlg
{
protected:
	DECLARE_DYNAMIC(CHentDiffMergeDataDlg)
public:
	CHentDiffMergeDataDlg(Hent* pHent1,Hent *pHent2,CWnd* pParent = NULL);  
	virtual ~CHentDiffMergeDataDlg();
	bool AreDiffrent();
private:
	DiffMemberArray m_AllToDiff;
	DiffMemberArray m_hentDiffArray;

protected:
	void OnOK();
};

#endif