#ifndef __GRPSUMMARYLISTCTRL_H__
#define __GRPSUMMARYLISTCTRL_H__

#include "../datatypes/Cow.h"
#include "../datatypes/CowDataArray.h"
#include "../datatypes/Doc.h"
#include "../accessor/CowAccessor.h"
#include "BaseListCtrl.h"

class CGrpSummaryListCtrl : public CBaseListCtrl
{
public:
	CGrpSummaryListCtrl(CowAccessor* pAccessor,CCowArray &summaryCows);	
	void UpdateSummary();
protected:
	void UpdateItemCount();
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual void SetListCols();
	virtual NullInt GetCowGroup(Cow* pCow);
private:
	GRIDHEADERS szInOutGrpSummaryListCols[1+MAX_INOUT_GRP+1+1]; //empty count, 0..MAX_OUT_GRP = MAX_OUT_GRP+1,  {0,0}
	CCowArray &m_summaryCows;
	CowAccessor* m_pAccessor;
	UINT m_inoutgrpSummaryTable[MAX_INOUT_GRP+1];
	UINT m_emptyGrpCount;

};

#endif