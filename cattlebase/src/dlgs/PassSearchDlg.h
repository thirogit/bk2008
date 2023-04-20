#ifndef __PASSSEACHDLG_H__
#define __PASSSEACHDLG_H__

#include "../DataTypes.h"
#include "../ctrls/CowListCtrl.h"
#include "../ctrls/XColorStatic.h"
#include "../combo/OutGrpCombo.h"
#include "../barcodescanner/BCScannerDrv.h"
#include "../accessor/CowAccessor.h"

typedef CMap<Cow*,Cow*,bool,bool> CFoundPassMarkMap;

class GroupComparator : public CCowArray::SortArrayComparator,private DataComparator<Cow>
{
public:
	GroupComparator(CowAccessor* pGroupAccessor,bool bAsc);
	virtual COMPARE_RESULT Compare(Cow *DataLeft,Cow *DataRight);
protected:
	CowAccessor* m_pGroupAccessor;
private:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,Cow* DataLeft,Cow* DataRight);
};

class CSearchPassListCtrl : public CCowListCtrl
{
public:
	CSearchPassListCtrl(CCowArray &cowArray,CFoundPassMarkMap &markMap,CowAccessor* pGroupAccessor);
protected:
	CowAccessor* m_pGroupAccessor;
	CFoundPassMarkMap &m_markMap;
	static const GRIDHEADERS PassSearchCowHeader[];
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	virtual void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
};



// CPassSearchDlg dialog

class CPassSearchDlg : public CDialog, public IBarcodeReceiver 
{
	DECLARE_DYNAMIC(CPassSearchDlg)

public:
	CPassSearchDlg(CCowArray &searchArray,CowAccessor* pGroupAccessor,CWnd* pParent = NULL);   // standard constructor
	virtual ~CPassSearchDlg();

// Dialog Data
	enum { IDD = IDD_PASSSEARCHDLG };

	
protected:

	BOOL OnInitDialog();

protected:
	CCowArray &m_searchArray;
	CSearchPassListCtrl m_cowlist;
	
	CXColorStatic m_lastSearchStatus;
	CXColorStatic m_barcode;

	CowAccessor* m_pGroupAccessor;
	CFoundPassMarkMap m_markMap;
	int m_cowsLeft;
	CFont m_lastResultFont;
	COutGrpCombo m_grpCombo;
	BOOL m_bActiveGroupCheck;
	
	void UpdateCowsLeft();

	enum PASSSEARCH_RESULT
	{
		FOUND,
		NOTFOUND,
		WAITING,
		NOTINGRP
	};
	void UpdateLastResult(PASSSEARCH_RESULT lastResult);
	void FindPassport(const CString& sBarcode);
	virtual void OnBarcode(const CString& sBarcode);
	NullInt GetCowGroup(Cow* pCow);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	
	afx_msg void OnActiveGrpCheck();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif