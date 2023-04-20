// CfgDem.cpp: implementation of the CCfgDem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrintDocsCfg.h"
#include "../missing/util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define __CFG_IMPLEMENTATION__
#include "PrintDocsCfg.h"				// When __CFG_IMPLEMENTATION__ is defined 
									// MainCfg.h actually defines the CFGDEF
									// structure array

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPrintCfg CPrintCfg::m_printCfg;

CPrintCfg::CPrintCfg() 
{
	CString printCfgFilePath = CUtil::GetAppDir() + "\\PRINT.CFG";
	MakeCfgList(PRINTCFG);	// Make the cfg node list from the cfgdef struct array.
	VERIFY(Init((LPCSTR)printCfgFilePath) == CFG_NOERR);	


}

CPrintCfg::~CPrintCfg()
{
	Flush();
}