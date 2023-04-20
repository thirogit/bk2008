#ifndef __COWGRID_H__
#define __COWGRID_H__

#include "DataGrid.h"
#include "../datatypes/CowDataArray.h"
#include "../MarkWarnShowOptions.h"
#include "../Zebra/ZebraPOS.h"

class CCowGrid : public DataGrid<CCowArray,Cow>
{
public:	
    CCowGrid(CCowArray &cowarray,  const GRIDHEADERS *ppHeaders,  int pConfigItemId); 
	
protected:	
	void AddPopMenuItem(CPopMenuNodeList &popmenulist) ; 
	virtual UINT GetGridDataTypeId() { return DATATYPE_COWS; };
	void GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData);
	bool GetDataCellColor(int iItem,int nCol,COLORREF &cr);
	BOOL GetToolTipText(int iItem,int iSubItem,CString &tooltipText);
	virtual void OnDoubleClick(int iItem,int iSubItem) ;
	virtual bool HandleMessage(UINT message);
	virtual bool HandleUpdateMessage(CCmdUI *pCmdUI);
	
	void DoCowEdit(Cow *cowd);
	void OnMakeRRInvoice();
	void OnShow(UINT nID);
	void ShowHent(Hent* pHent);

	void OnSumPrices(UINT nID);
	void OnSumSubPrices(UINT nID);
	void OnPrintCowLabel(const Cow* pCow);
	void ShowHideMyPrice();

	CMarkWarnShowOptions *m_mws_opts;
	ZebraPOS m_ZebraPOS;
	
};


class CAllCowsGrid : public CCowGrid
{
	 MY_DECLARE_DYNCREATE(CAllCowsGrid)
public:
	 CAllCowsGrid(); 
private:
	static GRIDHEADERS AllCowHeader[];

	
};


class CInCowsGrid : public CCowGrid
{
	 MY_DECLARE_DYNCREATE(CInCowsGrid)
public:
    CInCowsGrid(); 
private:
	static GRIDHEADERS InCowHeader[];
	
};


#endif