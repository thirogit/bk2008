#ifndef __COUNTRYEDIT_H__
#define __COUNTRYEDIT_H__

#include "TipEdit.h"
#include "../DataTypes.h"
#include "../datatypes/CountryCodeArray.h"

class AscendingCountryComparator : public CCountryCodeArray::SortArrayComparator, private DataComparator<Country>
{
public:
	AscendingCountryComparator();
	virtual COMPARE_RESULT Compare(Country *DataLeft,Country *DataRight);
private:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,Country* DataLeft,Country* DataRight);
};

#define OTHER_COUNTRY_ISO_NUMBER  (9999)

class CCountryEdit : public CTipEdit
{
public:
	CCountryEdit(LPCTSTR pszMask = "");
protected:
	virtual void OnCountryChange(Country *newCountry) = 0;
	Country* GetCountry(UINT countryISONumber);
	Country* GetCountry(const CString& countryCode2a);
	
	BOOL OnEditCommand(UINT nID);
	virtual void ExpandContextMenu(CMenu* pMenu) = 0;
	virtual BOOL OnExpandedCommand(UINT nCommand) = 0;

	int m_checkedCountry;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	
	DECLARE_MESSAGE_MAP()
};


#endif