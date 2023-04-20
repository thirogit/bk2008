#ifndef __DEZYNFEKCJA_H__
#define __DEZYNFEKCJA_H__

#include "../printing/CowDrwDoc.h"
#include "CustomPrintDocOptDlg.h"
#include "PrintDocsCfg.h"

class CDezynfDocOptDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CDezynfDocOptDlg)
public:
	CDezynfDocOptDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_DEZYNFDOCOPT };
	virtual UINT GetTemplateID() { return IDD; };
	virtual void Save();
private:
	BOOL m_bDontFillPlateNo;
	CString m_agentName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);		
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};


class CDezynfekcjaDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(CDezynfekcjaDoc)
public:
	CDezynfekcjaDoc();
	int GetPrintPages() const;
	void PrintPage(int page);
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);

	enum CFG_ITEM_INDEX
	{
		CFG_DONTFILLPLATENO = CPrintCfg::CFG_PRINT_OPTION_MAX,
		CFG_AGENTNAME 		
	};
private:
	CDocDataArray m_docs;
	int m_nPages;
	BOOL m_bDontFillPlateNo;
	CString m_agentName;
};

#endif
