#include "stdafx.h"
#include "ExcelExport.h"

#include "../consts.h"

CExcelExport::CExcelExport() 
{
}

bool CExcelExport::Export(CDocDataArray &docs,CString &xlsFile)
{
	ASSERT(docs.GetSize() > 0);
	int docsize = docs.GetSize(),cowsize = 0,i,j,dot;
	CString sheetName,saveFile;
	Doc *pDoc = NULL;
	const char xlsExt[] = ".xls";

	i = xlsFile.ReverseFind('\\');
	dot = xlsFile.ReverseFind('.');
	if(dot < 0)
	{
		saveFile = xlsFile + xlsExt;
	}
	else
	{
		if(dot < i)
			saveFile = xlsFile + xlsExt;
		else
			saveFile = xlsFile;
		
	}


	DeleteFile(saveFile); //delete file if exists


	CStringArray aHeaders;
	aHeaders.Add(CString());
	
	for(i = 0;i < docsize;i++)
	{

		pDoc = docs[i];
		sheetName = pDoc->GetDocNoStr();

		CSpreadSheet sheet(saveFile,sheetName,false);
		sheet.AddHeaders(aHeaders);
		sheet.BeginTransaction();

		j = 0;
		CCowArray::ArrayIterator cowIt = pDoc->GetCows()->iterator();
		while (cowIt.hasNext())
		{
			sheet.AddCell((*(cowIt++))->GetEAN(),1,j+1);
			j++;
		}
		sheet.Commit();
	
		
		
	}


	return true;

}
