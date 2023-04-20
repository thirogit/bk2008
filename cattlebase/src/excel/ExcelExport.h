#ifndef _EXCELEXPORT_H_INCLUDED_
#define _EXCELEXPORT_H_INCLUDED_

//#include "BasicExcelVC6.hpp"
#include "SpreadSheet.h"
#include "global_arrays.h"

class CExcelExport
{
public:
	CExcelExport();
	bool Export(CDocDataArray &docs,CString &xlsFile);

};


#endif