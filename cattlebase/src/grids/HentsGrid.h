#ifndef __HENTSGRID_H__
#define __HENTSGRID_H__

#include "DataGrid.h"
#include "popmenunode.h"
#include "myruntimeclass.h"
#include "../datatypes/HentArray.h"
#include "../DataTypes.h"
#include "../textproviders/HentTextProvider.h"
#include "../textproviders/BaseArrayTextProvider.h"

class CHentsGrid : public DataGrid<CHentArray,Hent>
{
	 MY_DECLARE_DYNCREATE(CHentsGrid)
public:	
    CHentsGrid(); 
protected:
	CHentsGrid(int pConfigItemId,CHentArray & HentAry) ;
protected:
	void GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData);
	UINT GetGridDataTypeId() { return DATATYPE_HENTS; };
	virtual bool HandleMessage(UINT message);
	virtual bool HandleUpdateMessage(CCmdUI *pCmdUI);
	virtual void AddPopMenuItem(CPopMenuNodeList &popmenulist);
	static GRIDHEADERS HentHeader[];

	BaseArrayTextProvider<CHentArray,HentTextProvider> m_textProvider;

	void OnSyncItHentYes();
	void OnSyncItHentNo();
	void HentSyncIt(bool bSyncIt);
		
};


class CCompanyHentsGrid : public CHentsGrid
{
	 MY_DECLARE_DYNCREATE(CCompanyHentsGrid)
public:
	
    CCompanyHentsGrid(); 
	
		
};



class CFarmerHentsGrid : public CHentsGrid
{
	 MY_DECLARE_DYNCREATE(CFarmerHentsGrid)
public:
	
    CFarmerHentsGrid(); 
	
};

#endif