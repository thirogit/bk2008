#ifndef __HERDBOOK_H__
#define __HERDBOOK_H__

#include "../../printing/CowDrwDoc.h"
#include "../../DataTypes.h"
#include "../CustomPrintDocOptDlg.h"
#include "../PrintDocsCfg.h"

class CHerdBook : public CCowDrwDoc
{
public:

	enum CFG_ITEM_INDEX
	{
		CFG_PAGENUM = CPrintCfg::CFG_PRINT_OPTION_MAX,
		CFG_SERIALNUM
	};

	CHerdBook(); 	
	int GetPrintPages() const;	
	void PrintPage(int page);
	int LoadInputData(CPtrArray &docInputData);
	
protected:	
	virtual int GetCowsOnPage() const = 0;
	virtual void DrawCowRow(int iRow,int iSerialNum,const Cow* pCow) = 0;
	void PrintEAN(const CString& sEAN,const CString& sLabelFmt);
	virtual void PrintHerdNo(const CString& sHerdNo,const CString& sLabelFmt);
	CString FormatHent(const Hent* pHent);
	
private:
	CCowArray m_cows;
	int m_PageNum;
	int m_SerialNum;


};


class CHerdBookOptDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CHerdBookOptDlg)
public:
	CHerdBookOptDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_HERDBOOKOPTDLG };
	virtual UINT GetTemplateID() { return IDD; };
	virtual void Save();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
private:
	int m_nPageNum;
	int m_nSerialNum;	
	

	
	
};



#endif
