#ifndef __PZWZDOC_H__
#define __PZWZDOC_H__

#include "PrintDocsCfg.h"
#include "../printing/CowDrwDoc.h"
#include "../DataTypes.h"
#include "CustomPrintDocOptDlg.h"

#define PZWZDOCPAGE_1_SIZE 40
#define PZWZDOCPAGE_2_SIZE 46

class CPzWzOptDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CPzWzOptDlg)
public:
	CPzWzOptDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_WZPZOPT };
	virtual UINT GetTemplateID() { return IDD; };
	virtual void Save();
protected:
	BOOL m_printExtras;	
	BOOL m_bUseInvValues;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};


class CZDoc : public CCowDrwDoc
{
public:

	enum CFG_ITEM_INDEX
	{
		CFG_PRINTEXTRAS = CPrintCfg::CFG_PRINT_OPTION_MAX,
		CFG_USEINVVALUES
	};

	CZDoc();
	
	int GetPrintPages() const;
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
	void PrintPage(int page);
protected:
	void FormatHentInfo(const Hent* hent,CString &outStr);
	virtual void PrintRecord(Cow *pCow,int iRow,int iIndex);

	virtual Weight GetCowWeight(Cow *pCow) = 0;
	virtual Stock *GetCowStock(Cow *pCow) = 0;
	virtual NullInt GetCowGroup(Cow* pCow) = 0;
	virtual CString GetZDocTitle() = 0;
	virtual const Hent* GetBuyer() = 0;
	virtual const Hent* GetSeller() = 0;

	CDocDataArray m_docs;
	Doc *currDoc;
	CCowArray m_currDocCows;
	int starti;
	int endi;
	int pageOff;
	BOOL printExtras;
	BOOL bUseInvValues;
	int m_nPages;
	Weight currDocTotalWeight;
	int currDocLastPage;
	int curDocPageSize;
};


class CPzDoc : public CZDoc
{
	DECLARE_DYNCREATE(CPzDoc)
public:
	CPzDoc();
protected:
	virtual Weight GetCowWeight(Cow *pCow);
	virtual Stock *GetCowStock(Cow *pCow);
	virtual NullInt GetCowGroup(Cow* pCow);
	virtual CString GetZDocTitle();
	virtual const Hent* GetBuyer();
	virtual const Hent* GetSeller();
};

class CWzDoc : public CZDoc
{
	DECLARE_DYNCREATE(CWzDoc)
public:
	CWzDoc();
protected:
	virtual Weight GetCowWeight(Cow *pCow);
	virtual Stock *GetCowStock(Cow *pCow);
	virtual NullInt GetCowGroup(Cow* pCow);
	virtual CString GetZDocTitle();
	virtual const Hent* GetBuyer();
	virtual const Hent* GetSeller();
};


#endif
