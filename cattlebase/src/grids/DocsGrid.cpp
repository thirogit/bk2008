#include "stdafx.h"
#include "DocsGrid.h"
#include "MainCfg.h"
#include "consts.h"
#include "global_arrays.h"
#include "../datatypes/invoice/NewVATInvoiceSession.h"
#include "../dlgs/NewOutDocDlg.h"
#include "../Doc/InDoc/EditInDocDlg.h"
#include "../dlgs/EditOutDocDlg.h"
#include "../datatypes/DocDataComparator.h"
//#include "hkey/hkey.h"
#include "../excel/ExcelExport.h"
#include "../inout_port/ExportCtrl.h"
#include "../inout_port/ImportCtrl.h"
#include "../datatypes/InDoc.h"
#include "../datatypes/OutDoc.h"
#include "../datatypes/invoice/SellVATInvoiceContext.h"
#include "../datatypes/invoice/BuyVATInvoiceContext.h"
#include "../dlgs/invoice/BigInvoiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDocsGrid::CDocsGrid(const GRIDHEADERS *ppHeaders, int pConfigItemId, CDocDataArray &dataArray) : 
	DataGrid<CDocDataArray,Doc>(dataArray,ppHeaders,pConfigItemId),m_textProvider(dataArray)
{
}
void CDocsGrid::GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData)
{
	m_textProvider.GetText(iItem,GetColID(nCol),pszText);

}

void CDocsGrid::AddPopMenuItem(CPopMenuNodeList &popmenulist)
{
	popmenulist.Add(new CPopMenuNode());
	popmenulist.Add(new CPopMenuNode(IDM_GRID_DOC_MAKEINVOICE, IDS_MAKEINVOICE,0));
	popmenulist.Add(new CPopMenuNode(IDM_GRID_DOC_EXCELEXPORT, IDS_EXCELEXPORT,0));
	popmenulist.Add(new CPopMenuNode(IDM_GDIR_DOC_EXPORTTOFILE, IDS_EXPORTTOFILE,0));
}

bool CDocsGrid::HandleMessage(UINT message)
{
	if(DataGrid<CDocDataArray,Doc>::HandleMessage(message))
		return true;
	switch(message)
	{
		case IDM_GRID_DOC_MAKEINVOICE:
		{
			CDocDataArray selDocs;				
			GetSelectedRows(selDocs);
			if(VATInvoice::CanMakeVATInvoiceOnDocs(selDocs))
			{					
				OnMakeInvoice(selDocs);
			}
			else
				AfxMessageBox(IDS_CANTMAKEVATINVOICE,MB_OK,0);			
			break;
		}
		case IDM_GRID_DOC_EXCELEXPORT:
			OnExcelExport();
			break;
		case IDM_GDIR_DOC_EXPORTTOFILE:
			OnExportToFile();
			break;

	default:
		return false;
	}
	return true;
}

void CDocsGrid::OnExportToFile()
{	
	CDocDataArray selectedDocs;
	GetSelectedRows(selectedDocs);
	
	if(selectedDocs.GetCount() > 0)
	{

		CString fileFilter,extension;
		extension.LoadString(IDS_IMPORTEXPORT_EXT);
		fileFilter.Format(IDS_IMPORTEXPORT_FILEFILTER,(LPCSTR)extension,(LPCSTR)extension);

		CString outFileName = "BK_";
		outFileName += CDateTime::now().GetDateInFmt(CDateTime::YYYYMMDDHHMISS);
		outFileName +='.';
		outFileName += extension;

		CFileDialog docSaveFile(FALSE,(LPCSTR)extension,(LPCSTR)outFileName,OFN_OVERWRITEPROMPT,(LPCSTR)fileFilter,this);	

		if(docSaveFile.DoModal() == IDOK)
		{
			ExportToFile(selectedDocs,docSaveFile.GetPathName());		
		}
	}
}


void CDocsGrid::OnExcelExport()
{
	CExcelExport excelExport;
	CDocDataArray selDocs;

	CFileDialog xlsSave(FALSE,NULL,NULL,OFN_OVERWRITEPROMPT,"Arkusz Excel (*.xls)|*.xls||",this);
	if(xlsSave.DoModal() == IDOK)
	{
		GetSelectedRows(selDocs);
		excelExport.Export(selDocs,xlsSave.GetPathName());
	}
}


GRIDHEADERS CInDocsGrid::InDocHeader[] =
{
	{IDS_HEADERDOCNO,DOCS_SORT_BY_NUMBER},
	{IDS_HEADERDOCDATE,DOCS_SORT_BY_DOCDATE},
	{IDS_HEADERDELIVER,DOCS_SORT_BY_HENT},
	{IDS_HEADERAGENT,DOCS_MEMBER_AGENT},
	{IDS_HEADERINDATE,DOCS_SORT_BY_LOADDATE},
	{IDS_HEADERPLATENO,DOCS_SORT_BY_PLATENO},
	{IDS_HEADERAMOUNT,DOCS_SORT_BY_COWCOUNT},
	{IDS_HEADERINVOICE,DOCS_SORT_BY_INVOICE},
	{IDS_HEADEREXTRAS,DOCS_SORT_BY_EXTRAS},
	{0,0}
};

MY_IMPLEMENT_DYNCREATE(CInDocsGrid,CDocsGrid)

CInDocsGrid::CInDocsGrid() : CDocsGrid(InDocHeader,CFGID_IGRID,ary_indocs)
{
}


void CInDocsGrid::AddPopMenuItem(CPopMenuNodeList &popmenulist)  
{

	CDocsGrid::AddPopMenuItem(popmenulist);
	
	popmenulist.Add(new CPopMenuNode(IDM_GRID_INDOC_MAKEOUTDOC, IDS_MAKEOUTDOCMENUTEXT,0));
	popmenulist.Add(new CPopMenuNode(IDM_GRID_INDOC_IMPORTFROMFILE, IDS_IMPORTINDOC,0));
	
	
}

void CInDocsGrid::GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData)
{
	InDoc *pDoc = (InDoc*)GetRowData(iItem);
	
	if(!pDoc->HasInvoice() && GetColID(nCol) == DOCS_SORT_BY_INVOICE)
	{
		if(!pDoc->HaveNoRRInvs())
			lstrcpy(pszText,"RR");	
		else
			lstrcpy(pszText,szMissing);	
	}
	else
		CDocsGrid::GetData(iItem,nCol,pszText,pData);
}


bool CInDocsGrid::HandleMessage(UINT message)
{
	if(CDocsGrid::HandleMessage(message))
		return true;

	int iSelItem = GetCurSel();

	switch(message)
	{
		case IDM_GRID_EDITROW:
		{
			InDoc *pInDoc = (InDoc*)GetRowData(iSelItem);
			CEditInDocDlg docedit(pInDoc);
			docedit.DoModal();					
			break;
		}
		
		case IDM_GRID_INDOC_MAKEOUTDOC:
		{
			CDocDataArray selDocs;
			GetSelectedRows(selDocs);

			if(selDocs.GetSize() > 0)
			{
				CCowArray cows;
				Doc* pDoc;
				Cow* pCow;

				CDocDataArray::ArrayIterator docIt = selDocs.iterator();
				while(docIt.hasNext())
				{
					pDoc = *(docIt++);
					
					CCowArray::ArrayIterator cowIt = pDoc->GetCows()->iterator();
					while(cowIt.hasNext())
					{		
						pCow = *(cowIt++);
						if(!pCow->IsOut())
							cows.Add(pCow);
					}				
							
				}


				CNewOutDocDlg newOutDocDlg(cows);
				newOutDocDlg.DoModal();
			}
			break;
		}

		case IDM_GRID_INDOC_IMPORTFROMFILE:
			OnImportCommand();
			break;
	default:
		return false;
	}
	return true;
}

void CInDocsGrid::OnImportCommand()
{

	CString fileFilter,extension;
	extension.LoadString(IDS_IMPORTEXPORT_EXT);
	fileFilter.Format(IDS_IMPORTEXPORT_FILEFILTER,(LPCSTR)extension,(LPCSTR)extension);
	
	CFileDialog openImportDoc(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,(LPCSTR)fileFilter);

	if(openImportDoc.DoModal() == IDOK)
	{
		CImportCtrl importCtrl(this);
		importCtrl.ImportInDoc(openImportDoc.GetPathName());
	}
}

void CInDocsGrid::ExportToFile(CDocDataArray& docs,const CString &outputFile)
{
	CExportCtrl exportCtrl(this);
	
	

	exportCtrl.ExportInDoc(docs,outputFile);
			
}

void CInDocsGrid::OnMakeInvoice(CDocDataArray& docs)
{
	BuyVATInvoiceContext buyVATInvoiceContext;
	NewVATInvoiceSession newVatInvoiceSession(&buyVATInvoiceContext,docs);

	CBigInvoiceDlg bigInvoiceDlg(&newVatInvoiceSession,this);
	bigInvoiceDlg.DoModal();	
}

bool CInDocsGrid::GetDataCellColor(int iItem,int nCol,COLORREF &cr)
{
	bool bRet = false;
	
	InDoc *pInDoc = (InDoc*)GetRowData(iItem);
	switch(GetColID(nCol))
	{
		case DOCS_SORT_BY_NUMBER:
			if(!pInDoc->HasInvoice())
			{
				CCowArray::ArrayIterator it = pInDoc->GetCows()->iterator();
				while(it.hasNext())
				{
					Cow* pCow = *(it++);
					if(!pCow->HasAnyBuyInvoice())
					{
						cr = RGB(255,0,0);
						bRet = true;
					}
				}				
			}			
		break;		
	}

	return bRet;	
}


//***************************************************************************************

GRIDHEADERS COutDocsGrid::OutDocHeader[] =
{
	{IDS_HEADERDOCNO,DOCS_SORT_BY_NUMBER},
	{IDS_HEADERDOCDATE,DOCS_SORT_BY_DOCDATE},
	{IDS_HEADERBUYER,DOCS_SORT_BY_HENT},
	{IDS_HEADERAGENT,DOCS_MEMBER_AGENT},
	{IDS_HEADEROUTDATE,DOCS_SORT_BY_LOADDATE},
	{IDS_HEADERPLATENO,DOCS_SORT_BY_PLATENO},
	{IDS_HEADERAMOUNT,DOCS_SORT_BY_COWCOUNT},
	{IDS_HEADERINVOICE,DOCS_SORT_BY_INVOICE},
	{IDS_HEADEREXTRAS,DOCS_SORT_BY_EXTRAS},
	{0,0}
};

MY_IMPLEMENT_DYNCREATE(COutDocsGrid,CDocsGrid)
COutDocsGrid::COutDocsGrid() : CDocsGrid(OutDocHeader,CFGID_OGRID,ary_outdocs)
{
}

bool COutDocsGrid::HandleMessage(UINT message)
{
	if(CDocsGrid::HandleMessage(message))
		return true;

	switch(message)
	{
		case IDM_GRID_EDITROW:
		{
			OutDoc *pOutDoc = (OutDoc*)GetRowData(GetCurSel());
			CEditOutDocDlg docedit(pOutDoc);
			docedit.DoModal();
			break;
		}		
		default:
			return false;
	}
	return true;
}

bool COutDocsGrid::GetDataCellColor(int iItem,int nCol,COLORREF &cr)
{
	bool bRet = false;
	
	OutDoc *pOutDoc = (OutDoc*)GetRowData(iItem);
	switch(GetColID(nCol))
	{
		case DOCS_SORT_BY_NUMBER:
			if(!pOutDoc->HasInvoice())
			{
				cr = RGB(255,0,0);
				bRet = true;								
			}			
		break;		
	}

	return bRet;	
}


void COutDocsGrid::ExportToFile(CDocDataArray& docs,const CString &outputFile)
{
	CExportCtrl exportCtrl(this);
	exportCtrl.ExportOutDoc(docs,outputFile);
}

void COutDocsGrid::OnMakeInvoice(CDocDataArray& docs)
{
	SellVATInvoiceContext sellVATInvoiceContext;
	NewVATInvoiceSession newVatInvoiceSession(&sellVATInvoiceContext,docs);

	CBigInvoiceDlg bigInvoiceDlg(&newVatInvoiceSession,this);
	bigInvoiceDlg.DoModal();	
}