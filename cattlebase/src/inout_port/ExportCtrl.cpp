#include "stdafx.h"
#include "ExportCtrl.h"
#include "ImportExportDocs.h"
#include "ImportExportErrorCodeTranslator.h"
#include "../classes/TempStringLoader.h"
#include "ExportOptionsDlg.h"

CExportCtrl::CExportCtrl(CWnd* pParentWnd) : m_pParentWnd(pParentWnd)
{
}
void CExportCtrl::ExportOutDoc(CDocDataArray& docs,const CString& outputFilePath)
{
	CImportExportDocs importExportDocs;
	
	CImportExportDocs iwd;
	ImportExportTaskTranslator importTaskTrans;
	CTaskProgressDialog TaskDlg(CTempStringLoader(IDS_EXPORTDOCS),&importTaskTrans);

	ExportSettings exportSettings;
	exportSettings.Load();
	CExportOptionsDlg exportOptsDlg(exportSettings,m_pParentWnd);
	
	
	UINT uiDialogResult = exportOptsDlg.DoModal();
	bool bDoExport = uiDialogResult == IDSAVEANDUSE || uiDialogResult == IDUSEONLYONCE;
	if(uiDialogResult == IDSAVEANDUSE)
	{
		exportSettings.Save();		
	}

	if(bDoExport)
	{

		TaskDlg.Create();
		if(!importExportDocs.WriteOutDoc(docs,exportSettings,outputFilePath,&TaskDlg))
		{
			if(!TaskDlg.CheckCancelButton())
			{
				AfxMessageBox(TaskDlg.GetLastError(),MB_OK);
			}
			return;	
		}
	}
}

void CExportCtrl::ExportInDoc(CDocDataArray& docs,const CString& outputFilePath)
{
	CImportExportDocs importExportDocs;
	
	CImportExportDocs iwd;
	ImportExportTaskTranslator importTaskTrans;
	CTaskProgressDialog TaskDlg(CTempStringLoader(IDS_EXPORTDOCS),&importTaskTrans);

	ExportSettings exportSettings;
	exportSettings.Load();
	CExportOptionsDlg exportOptsDlg(exportSettings,m_pParentWnd);
	UINT uiDialogResult = exportOptsDlg.DoModal();
	bool bDoExport = uiDialogResult == IDSAVEANDUSE || uiDialogResult == IDUSEONLYONCE;
	if(uiDialogResult == IDSAVEANDUSE)
	{
		exportSettings.Save();		
	}

	if(bDoExport)
	{
		TaskDlg.Create();
		if(!importExportDocs.WriteInDoc(docs,exportSettings,outputFilePath,&TaskDlg))
		{
			if(!TaskDlg.CheckCancelButton())
			{
				AfxMessageBox(TaskDlg.GetLastError(),MB_OK);
			}
			return;	
		}
	}




}
