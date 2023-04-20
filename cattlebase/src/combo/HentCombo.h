#ifndef __HENTCOMBO_H__
#define __HENTCOMBO_H__


#include "../DataTypes.h"
#include "ImageComboBox.h"
#include "PtrFlushArray.h"
#include "../dlgs/FastFindHent.h"
#include "../dlgs/NewHentDlg.h"

class CHentCombo : public CImageComboBox
{
public:
	CHentCombo(bool bIclMissingItem = false);
	virtual ~CHentCombo();
	static void NotifyReload();


	
	virtual void ReloadCombo(Hent *curSel = NULL);
	Hent* GetSelectedHent();
	bool SelectHent(const CString& farmNo);

	bool GetShowOnlyCompanies();
	bool GetShowOnlyIndividuals();

	void SetShowOnlyCompanies(bool bOnlyCompanies);
	void SetShowOnlyIndividuals(bool bOnlyIndividuals);

protected:
	bool m_bIclMissingItem;
	CImageList m_imlHentIcons;
	CMenu m_hentComboMenu;

	int FindHentInArray(const CString& farmNo,CHentArray &hentArray);
	int FindHentInCombo(const CString& farmNo);

	bool m_bOnlyCompanies;
	bool m_bOnlyIndividuals;

	void CreateImageList();
	void PreSubclassWindow();
	void OnAddHent();
	void OnFindHent();

	static PtrFlushArray<CHentCombo> m_hentCombos;


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	
};





//class CFindHentCombo : public CBaseFindCombo<CHentCombo> 
//{
//public:
//	CFindHentCombo(bool bWithEmptyHentItem = true) 
//	{
//		m_bIclMissingItem = bWithEmptyHentItem;			
//	}
//	virtual void InitFindCombo()
//	{
//		ReloadCombo(/*NULL*/);
//		m_nNeutralItem  = AddItem(NEUTRAL_STR,-1);
//		SetCurSel(m_nNeutralItem);
//	}
//
//};


#endif