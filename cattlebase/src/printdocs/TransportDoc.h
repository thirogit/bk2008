#ifndef __TRANSPORTDOC_H__
#define __TRANSPORTDOC_H__

#include "CustomPrintDocOptDlg.h"
#include "../printing/CowDrwDoc.h"
#include "../DataTypes.h"
#include "PrintDocsCfg.h"

#define TRANSPORTDOCPAGE_1_SIZE 33
#define TRANSPORTDOCPAGE_2_SIZE 44


enum TRANSPORTDOC_OPTIONS_INDEX
{
	TRANSPORTDOC_DONOTFILLCARIER = CPrintCfg::CFG_PRINT_OPTION_MAX
};

class CTransportDocOptDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CTransportDocOptDlg)
public:
	CTransportDocOptDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_TRANSPORTDOCOPTDLG };
	virtual UINT GetTemplateID() { return IDD; };
	virtual void Save();
private:
	BOOL m_bDoNotFillCarrier;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};


class CTransportDoc : public CCowDrwDoc
{
public:
	CTransportDoc();
	
	int GetPrintPages() const;
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
	void PrintPage(int page);
protected:
	CString FormatHent(const Hent* hent);
	CString FormatFirstOwner(const Hent* hent);
	virtual void PrintRecord(Cow *pCow,int iRow,int iIndex);
		
	virtual const Hent* GetTransportCompany() = 0;
	virtual const Hent* GetDestination() = 0;

	BOOL m_bDontFillCarrier;
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


class CPzTransportDoc : public CTransportDoc
{
	DECLARE_DYNCREATE(CPzTransportDoc)
public:
	CPzTransportDoc();
protected:
	
	virtual const Hent* GetTransportCompany();
	virtual const Hent* GetDestination();
};

class CWzTransportDoc : public CTransportDoc
{
	DECLARE_DYNCREATE(CWzTransportDoc)
public:
	CWzTransportDoc();
protected:
	
	virtual const Hent* GetTransportCompany();
	virtual const Hent* GetDestination();
};


#endif
