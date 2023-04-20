// CfgDem.cpp: implementation of the CCfgDem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MainCfg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define __CFG_IMPLEMENTATION__
#include "MainCfg.h"				// When __CFG_IMPLEMENTATION__ is defined 
									// MainCfg.h actually defines the CFGDEF
									// structure array

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainCfg CMainCfg::m_mainCfg;

CMainCfg::CMainCfg()
{
	MakeCfgList(MAINCFG);	// Make the cfg node list from the cfgdef struct array.
}

CMainCfg::~CMainCfg()
{

}