#ifndef __DOCSGRID_H__
#define __DOCSGRID_H__

#include "popmenunode.h"
#include "myruntimeclass.h"
#include "DataGrid.h"
#include "../datatypes/DocDataArray.h"
#include "../DataTypes.h"
#include "../textproviders/DocTextProvider.h"
#include "../textproviders/BaseArrayTextProvider.h"

class CDocsGrid : public DataGrid<CDocDataArray,Doc>
{
public:
	CDocsGrid(const GRIDHEADERS *ppHeaders, int pConfigItemId, CDocDataArray &dataArray);
protected:
	void GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData);
	virtual void AddPopMenuItem(CPopMenuNodeList &popmenulist);
	virtual bool HandleMessage(UINT message);
	
	void OnExcelExport();
	void OnExportToFile();

	virtual void ExportToFile(CDocDataArray& docs,const CString &outputFile) = 0;
	virtual void OnMakeInvoice(CDocDataArray& docs) = 0;

	BaseArrayTextProvider<CDocDataArray,DocTextProvider> m_textProvider;


};

class CInDocsGrid : public CDocsGrid
{
	MY_DECLARE_DYNCREATE(CInDocsGrid)
public:
	CInDocsGrid(); 
	
private:
	void GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData);
	bool GetDataCellColor(int iItem,int nCol,COLORREF &cr);
	virtual bool HandleMessage(UINT message);
	virtual void AddPopMenuItem(CPopMenuNodeList &popmenulist);	
	virtual UINT GetGridDataTypeId() { return DATATYPE_INDOCS; };
	static GRIDHEADERS InDocHeader[];

	void OnImportCommand();
	virtual void ExportToFile(CDocDataArray& docs,const CString &outputFile);
	virtual void OnMakeInvoice(CDocDataArray& docs);
	

};


class COutDocsGrid : public CDocsGrid
{
	MY_DECLARE_DYNCREATE(COutDocsGrid)
public:
	COutDocsGrid(); 
	
private:
	bool GetDataCellColor(int iItem,int nCol,COLORREF &cr);
	virtual bool HandleMessage(UINT message);
	virtual UINT GetGridDataTypeId() { return DATATYPE_OUTDOCS; };
	static GRIDHEADERS OutDocHeader[];

	virtual void ExportToFile(CDocDataArray& docs,const CString &outputFile);
	virtual void OnMakeInvoice(CDocDataArray& docs);
	 
};

#endif