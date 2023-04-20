#ifndef __PASSPRINT_H__
#define __PASSPRINT_H__

#include "../printing/CowDrwDoc.h"
#include "CustomPrintDocOptDlg.h"
#include "../DataTypes.h"

class CPassPrintDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(CPassPrintDoc)
public:
	CPassPrintDoc();
	
	int GetPrintPages() const;
	void PrintPage(int page);
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
private:
	CCowArray m_cows;
	

	int m_nPages;
};

#endif
