#ifndef __BASEDATAGRID_H__
#define __BASEDATAGRID_H__

#include "Grid.h"
#include "../datatypes/RawData.h"

#define HLIGHTCOLOR RGB(0xFF,0xFF,0x7F)

class BaseDataGrid : public CGrid
{
public:
	
    BaseDataGrid(const GRIDHEADERS *ppHeaders, int pConfigItemId);
	
	virtual int GetSelectedRows(CPtrArray &selDataArray) = 0;
	virtual void UpdateItemCount() = 0;
	virtual void MarkRow(int nRow) = 0;
	virtual bool IsItemMarked(int iItem) = 0;
	virtual void SelectMarked() = 0;
	virtual RawData *GetItemPtr(int iItem) = 0;
	virtual bool GetDataCellColor(int iItem,int nCol,COLORREF &cr) = 0;

	virtual void UnMarkAllRows() = 0;
	virtual bool HandleMessage(UINT message) = 0;
	virtual bool HandleUpdateMessage(CCmdUI *pCmdUI) = 0;
	virtual void Lock();
	virtual void Unlock();


private:	
	virtual bool GetCellColor(int iItem,int nCol,COLORREF &cr);
	
};

#endif