#ifndef __INVOICEDETAILS_H__
#define __INVOICEDETAILS_H__

#include "../../printing/CowDrwDoc.h"
#include "../../DataTypes.h"
#include "../../classes/format/FloatFormat.h"
#include "../PrintDocsCfg.h"

#define SPECFAKTURYPAGE1SIZE 32
#define SPECFAKTURYPAGE2SIZE 37 

class CInvoiceDetailsDoc : public CCowDrwDoc
{
public:
	enum INVOICEDETAILS_OPTIONS_INDEX
	{
		PRICEPERKGDECIMALPT = CPrintCfg::CFG_PRINT_OPTION_MAX,
		PRICEPERKGPRECISION
	};

public:
	CInvoiceDetailsDoc();
	int GetPrintPages() const;
	int LoadInputData(CPtrArray &docInputData);
	
	
protected:
	virtual int GetInvoicePageSize(Invoice *pInv);
	CString GetHentInfoString(const Hent *pHent);
    virtual void PrintBuyer(const Hent *buyer);
	virtual void PrintSeller(const Hent *seller);
	virtual void PrintBottom() = 0;
	virtual void PrintHeader(bool bWithOutHents);
	virtual void PrintRecords(int sindex,int eindex,int row);
	virtual void PrintRecord(int row,int iLP,CowAccessor* pAccessor,Cow *pCow) = 0;
	void PrintPage(int page);	
	void PreparePage(int nPage);
	const FloatFormat& GetPricePerKgFormat() const;
	void SetPricePerKgFormat(const FloatFormat& fmt);
	

protected:
	CInvoiceDataArray m_invs2print;
	CInvoiceDataArray m_invs;
	Invoice *m_curInv;
	int m_starti;
	int m_endi;
	int m_pageOff;
	int m_curInvLastPage;
	int m_curInvPageSize;
	int m_nPages;
	CCowArray m_cows;
	Weight m_totalWeight;
	Weight m_totalSecWeight;
	Money m_totalNetto;
	FloatFormat m_PricePerKgFormat;


	void DrawWeight(const CString& sLabel,const Weight &weight);
	void DrawNullDouble(const CString& sLabel,const NullDouble& nullDouble,const FloatFormat& fmt);


};


#endif
