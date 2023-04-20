#ifndef __FAKTURARRDOC_H__
#define __FAKTURARRDOC_H__

#include "../../printing/CowDrwDoc.h"
#include "../../DataTypes.h"
#include "../skladnialiczeb.h"
#include "InvoiceDocOptions.h"
#include "../../Mustache/Mustache.h"

class InvoiceItem
{
public:
	enum QTYUNITT
	{
		KG = 0,
		PIECE
	};

	InvoiceItem();
	InvoiceItem(const InvoiceItem& src);
	InvoiceItem& operator=(const InvoiceItem& right);
	void CopyFrom(const InvoiceItem& src);

	void SetDescription(const CString& sDesc);
	CString GetDescription() const;

	void SetQtyWeight(double dWeight);
	void SetQtyPiece(int iPieces);

	NullDouble GetQtyWeight() const;
	NullInt GetQtyPieces() const;
	
	QTYUNITT GetQtyUnit() const;

	void SetTotal(const Money& total);
	Money GetTotal() const;

	Money GetUnitPrice() const;

private:

	double GetQty() const;

	CString m_sDesc;
	int m_qtyPiece;
	double m_qtyWeight;
	QTYUNITT m_qtyUnit;
	Money m_Total;
};


typedef PtrFlushArray<InvoiceItem> InvoiceItems;

class InvoiceItemGroup
{
public:
	InvoiceItemGroup();
	virtual bool Matches(const CowInvoiceEntry *pCow) = 0;
	void AddCow(const CowInvoiceEntry *pCow);

	int GetCowQty() const;
	const NullDouble& GetTotalGroupWeight() const;
	const Money& GetTotalGroupPrice() const;

	

private:
	int m_CowQty;
	NullDouble m_TotalGroupWeight;
	Money   m_TotalGroupPrice;
};


class StockGroup
{
public:
	StockGroup();
	void Reset();
	int qty;
	NullDouble groupWeight;
	Money   groupTotal;
	Stock *stock;
};


class PricePerKegItemGroup : public InvoiceItemGroup
{
public:
	PricePerKegItemGroup(double dGroupPrice);
	virtual bool Matches(const CowInvoiceEntry *pCow);
	double GetGroupPricePerKg() const;
	
private:
	double m_dGroupPrice;	
};

#define PAGE1UPPER 11
#define PAGE1SIZE 25
#define PAGE2UPPER 29
#define PAGE2SIZE 41

class CFakturaDoc : public CCowDrwDoc
{
protected:
	CFakturaDoc();
public:	
	int GetPrintPages() const;	
protected:
	virtual int LoadInputData(CPtrArray &docInputData);
	virtual int GetInvoicePageSize(Invoice *pInv) = 0;
    virtual void PrintBuyer(const Hent *buyer) = 0;
	virtual void PrintSeller(const Hent *seller) = 0;
	virtual void PrintBottom();
	virtual void PrintHeader(bool withDates,bool withHents);
	virtual void PrintInvoiceRecord(int lp,int row,const InvoiceItem *pPrintRecord);
	virtual void PrintRecords(int sindex,int eindex,int row);
	void PrintPage(int page);	
	virtual bool IsInvoiceValidToPrint(Invoice *pInv) = 0;
	void SetLogo();
	CString GetExtrasTemplate() const;
	CString GetProductNumeralName(int iProductNumeral) const;
	CString GetQtyString(const InvoiceItem *pPrintRecord) const;
	CString GetQtyUnitString(const InvoiceItem *pPrintRecord) const;
	MustacheVariable GetProductNameVariable(int qty) const;
	MustacheVariable GetProductQtyVariable(int qty) const;
	MustacheVariable GetCowEANVariable(const Cow* pCow) const;	
	MustacheVariable GetCowSexVariable(const Cow* pCow) const;
	MustacheVariable GetStockVariable(const Stock* pStock) const;
	CString FormatInvoiceEntry(const MustacheVariables& variables);
	const FloatFormat& GetPricePerKgFormat() const;
	void SetPricePerKgFormat(const FloatFormat& fmt);


protected:
	CInvoiceDataArray m_invs2print;
	CInvoiceDataArray invs;
	Invoice *curInv;
	int starti;
	int endi;
	int pageOff;
	int curInvLastPage;
	int curInvPageSize;
	int m_nPages;
	CCowArray cows;

	InvoiceOptions m_invoiceOptions;
	CSkladniaLiczebnikow m_numeralSyntax;
	MustacheContext m_MustacheContext;

	

};

class CFakturaA4 : public CFakturaDoc
{
public:
	void PreparePage(int nPage);
protected:
	int GetInvoicePageSize(Invoice *pInv);
	int _GetInvoicePageSize(int nRecords);
};

class CFakturaRRA4 : public CFakturaA4
{
	DECLARE_DYNCREATE(CFakturaRRA4)
protected:
	 virtual void PrintBuyer(const Hent *buyer);
	 virtual void PrintSeller(const Hent *seller);
	 virtual bool IsInvoiceValidToPrint(Invoice *pInv);
};

class CFakturaRRA4Piece : public CFakturaRRA4
{
	DECLARE_DYNCREATE(CFakturaRRA4Piece)
protected:
	 virtual void PrintRecords(int sindex,int eindex,int row);
};


#define FAKTURARRA5SIZE 3

class CFakturaRRA5 : public CFakturaDoc
{
	DECLARE_DYNCREATE(CFakturaRRA5)
public:
	void PreparePage(int nPage);
protected:
	int GetInvoicePageSize(Invoice *pInv);
	 virtual void PrintBuyer(const Hent *buyer);
	virtual void PrintSeller(const Hent *seller);
	virtual bool IsInvoiceValidToPrint(Invoice *pInv);
};


class CFakturaRRA5Piece : public CFakturaRRA5
{
	DECLARE_DYNCREATE(CFakturaRRA5Piece)
protected:
	virtual void PrintRecords(int sindex,int eindex,int row);
};

class CFakturaVATA4 : public CFakturaA4
{
	DECLARE_DYNCREATE(CFakturaVATA4)
protected:
	virtual void PrintSeller(const Hent *seller);
	virtual void PrintBuyer(const Hent *buyer);
	void PrintHent(LPCSTR prefix,const Hent *hent);
	virtual bool IsInvoiceValidToPrint(Invoice *pInv);
	virtual void PrintHeader(bool withDates,bool withHents);

};


class CFakturaVATGroupA4 : public CFakturaVATA4 
{
public:
	virtual ~CFakturaVATGroupA4();
protected:
	virtual int LoadInputData(CPtrArray &docInputData);
	virtual void PrintRecords(int sindex,int eindex,int row);
	virtual int GetInvoicePageSize(Invoice *pInv);
	void FreePrintMap();
	void PreparePage(int nPage);
	virtual void SetInvRecord(InvoiceItem *pInvRec,const StockGroup &stockGroup) = 0;
protected:
	CMap<Invoice*,Invoice*,InvoiceItems*,InvoiceItems*> printMap;
	
};

class CFakturaVATGroupWeightA4 : public CFakturaVATGroupA4
{
	DECLARE_DYNCREATE(CFakturaVATGroupWeightA4)
protected:
	virtual void SetInvRecord(InvoiceItem *pInvRec,const StockGroup &stockGroup);
};

class CFakturaVATGroupPieceA4 : public CFakturaVATGroupA4
{
	DECLARE_DYNCREATE(CFakturaVATGroupPieceA4)
protected:
	virtual void SetInvRecord(InvoiceItem *pInvRec,const StockGroup &stockGroup);
};



class CFakturaVATCompactA4 : public CFakturaVATA4
{	
public:
	virtual ~CFakturaVATCompactA4();
protected:
	virtual int LoadInputData(CPtrArray &docInputData);
	virtual void PrintRecords(int sindex,int eindex,int row);
	virtual int GetInvoicePageSize(Invoice *pInv);
	virtual void PreparePage(int nPage);
	virtual void SetInvRecord(InvoiceItem *pInvRec,const Invoice *pInv) = 0;
	void FreePrintMap();
protected:
	CMap<Invoice*,Invoice*,InvoiceItems*,InvoiceItems*> printMap;
};


class CFakturaVATCompactWeightA4 : public CFakturaVATCompactA4
{
	DECLARE_DYNCREATE(CFakturaVATCompactWeightA4)
protected:
	virtual void SetInvRecord(InvoiceItem *pInvRec,const Invoice *pInv);
};

class CFakturaVATCompactPieceA4 : public CFakturaVATCompactA4
{
	DECLARE_DYNCREATE(CFakturaVATCompactPieceA4)
protected:
	virtual void SetInvRecord(InvoiceItem *pInvRec,const Invoice *pInv);
};


class CFakturaVATGroup : public CFakturaVATA4 
{
public:
	virtual ~CFakturaVATGroup();
protected:
	virtual int LoadInputData(CPtrArray &docInputData);
	virtual void PrintRecords(int sindex,int eindex,int row);
	virtual int GetInvoicePageSize(Invoice *pInv);
	void FreeItemsMap();
	void PreparePage(int nPage);

	virtual InvoiceItemGroup* CreateItemGroupForEntry(const CowInvoiceEntry *pCow) = 0;
	virtual InvoiceItem* CreateIvoiceItemForGroup(const InvoiceItemGroup* pInvoiceGroup) = 0;
	
protected:
	CMap<Invoice*,Invoice*,InvoiceItems*,InvoiceItems*> m_InvoiceItemsMap;
	
};


class CFakturaVATGroupPricePerKgPieceA4 : public CFakturaVATGroup 
{
	DECLARE_DYNCREATE(CFakturaVATGroupPricePerKgPieceA4)
protected:
	virtual InvoiceItemGroup* CreateItemGroupForEntry(const CowInvoiceEntry *pCow);
	virtual InvoiceItem* CreateIvoiceItemForGroup(const InvoiceItemGroup* pInvoiceGroup);
};

class CFakturaVATGroupPricePerKgWeightA4 : public CFakturaVATGroup 
{
	DECLARE_DYNCREATE(CFakturaVATGroupPricePerKgWeightA4)
protected:
	virtual InvoiceItemGroup* CreateItemGroupForEntry(const CowInvoiceEntry *pCow);
	virtual InvoiceItem* CreateIvoiceItemForGroup(const InvoiceItemGroup* pInvoiceGroup);
};



#endif
