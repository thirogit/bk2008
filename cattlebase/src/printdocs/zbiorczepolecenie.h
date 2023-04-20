#ifndef __ZBIORCZEPOLECENIEDOC_H__
#define __ZBIORCZEPOLECENIEDOC_H__

#include "../printing/CowDrwDoc.h"
#include "../DataTypes.h"

#define ZBIORCZEPOLECENIE_PAGESIZE 41 

class CZbiorczePolecenieDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(CZbiorczePolecenieDoc)
public:
	CZbiorczePolecenieDoc(); 
	int GetPrintPages() const;
	void PrintPage(int page);
	virtual int LoadInputData(CPtrArray &docInputData);
	
protected:
	virtual void DrawTextLR(const CString& sLabel,const CString &sText);		
private:
	CInvoiceDataArray invs;
	CInvoiceDataArray m_dataArray;

	bool m_bDiscountFee;
	Money m_transFee;
	
};

#endif
