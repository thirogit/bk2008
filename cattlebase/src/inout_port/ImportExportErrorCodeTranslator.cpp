#include "stdafx.h"
#include "ImportExportErrorCodeTranslator.h"
#include "ImportExportDocs.h"
#if 0
CString CImportExportErrorCodeTranslator::Translate(UINT errorCode)
{
	UINT errorCode2errorStr[][2] =
	{
		{CImportExportDocs::CANT_OPEN_INPUT_FILE ,IDS_ERRORCANTOPENINPUTFILE},
		{CImportExportDocs::CANT_WRITE_OUTPUT_FILE,IDS_CANTOPENOUTPUTFILE},
		{CImportExportDocs::INPUT_FILE_CORRUPTED,IDS_INPUTFILECORRUPTED},
		{CImportExportDocs::INVALID_COMPANY_INFO,IDS_EMPTYCOMPANYFARMNO},
		{CImportExportDocs::OTHER_VERSION,IDS_OTHERVERSIONFILE},
		{0,0}
	};
	CString errorStr;
	UINT *err2str = errorCode2errorStr[0];

	while(err2str[0] && err2str[1])
	{
		if(err2str[0] == errorCode)
		{
			errorStr.LoadString(err2str[1]);
			return errorStr;
		}
		err2str += 2;
	}

	errorStr.Format(IDS_UNKNERROR,errorCode);
	return errorStr;
}
#endif