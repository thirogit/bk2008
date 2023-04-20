#ifndef __WETBOOKDOC_H__
#define __WETBOOKDOC_H__
#include "../printing/CowDrwDoc.h"
//#include "global_arrays.h"
#include "CustomPrintDocOptDlg.h"
#include "../DataTypes.h"

#define WETBOOKPAGESIZE 10

class CWetBookOptDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CWetBookOptDlg)
public:
	CWetBookOptDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_WETBOOKOPTDLG };
	virtual UINT GetTemplateID() { return IDD; };
	virtual void Save();
private:
	int m_LP;		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	DECLARE_MESSAGE_MAP()
};


class CWetBookDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(CWetBookDoc)
public:
	CWetBookDoc();
	
	int GetPrintPages() const;
	void PrintPage(int page);
	int LoadInputData(CPtrArray &docInputData);	
protected:
	CString FormatHent(const Hent* pHent) const;
	CString FormatLastOwner(const Hent* pHent) const;
	CString FormatCowNo(Cow *pCow);
private:
	CCowArray m_cows;
	int LPnumber;


};

#endif
