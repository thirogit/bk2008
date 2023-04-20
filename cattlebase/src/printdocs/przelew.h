#ifndef __PRZELEWDOC_H__
#define __PRZELEWDOC_H__

#include "../printing/CowDrwDoc.h"
#include "../DataTypes.h"

#define GRIDROWSIZE 27
#define GRIDTOTALSIZE 12


class CPrzelewDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(CPrzelewDoc)
public:
	CPrzelewDoc(); 	
	int GetPrintPages() const;
	void PrintPage(int page);
	virtual int LoadInputData(CPtrArray &docInputData);
	
protected:
	void FillGridWithText(LPCSTR labelPrefix,const CString &sText);
	void PrintHent(LPCSTR destLabel,const Hent *hent);

	virtual void DrawText(const char *szLabel,CString &strText);
	void PrintAccountNo(LPCSTR labelPrefix,const CString &sAccountNo);
	void PrintTransValue(Money &Value);

	
private:
	CInvoiceDataArray invs;
	CInvoiceDataArray dataArray;
	Money m_transFee;
	BOOL m_bDiscountFee;
	
};


#endif
