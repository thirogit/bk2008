#ifndef __GRIDTABS_H__
#define __GRIDTABS_H__

#include "DataTypes.h"

class CGridTab
{
public:
	CGridTab(UINT nId,UINT nResGridName,CRuntimeClass *pGridClass,UINT nHotKey,
		UserData::PRIVLEDGES uPrivledges,bool bActiveOnStartup);

	UINT GetGridId() const;
	UINT GetResGridName() const;
	CRuntimeClass * GetGridClass() const;
	UINT GetHotKey() const;
	UserData::PRIVLEDGES GetGridPrivledges() const;
	bool GetActiveOnStartup() const;

private:

	UINT m_nId;
	UINT m_nResGridName;
	CRuntimeClass *m_pGridClass;	
	UINT m_nHotKey;	
	UserData::PRIVLEDGES m_uPrivledges;
	bool m_bActiveOnStartup;
};


class GridTabs : public PtrFlushArray<CGridTab>
{
private:
	GridTabs();
	static GridTabs instance;
	static AutoArray arrayFlush;
public:
	static GridTabs* GetTabs();
};


#define ALLCOWSGRID 2
#define INCOWSGRID 3
#define HENTSGRID 4
#define INDOCSGRID 5
#define OUTDOCSGRID 6
#define BUYINVSGRID 7
#define SELLINVSGRID 8
#define RRINVSGRID 9
#define COMPANYHENTGRID 10
#define FARMERHENTGRID 11
#define EXPENSESGRID 12


	

#endif
