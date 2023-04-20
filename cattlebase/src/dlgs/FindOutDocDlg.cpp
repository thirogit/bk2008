#include "stdafx.h"
#include "FindOutDocDlg.h"
#include "../mainfrm_ctrl.h"
#include "../global_arrays.h"
#include "../classes/TempStringLoader.h"

CFindOutDocDlg::CFindOutDocDlg () : CFindDocBaseDlg(CTempStringLoader(IDS_SEARCHINOUTDOCS))
{
}


void CFindOutDocDlg::InitControls()
{
	m_reason.InitFindCombo(Reason::REASON_OUT);
}

void CFindOutDocDlg::SearchDoc(CFindAction<CDocDataArray::ArrayAction,CDocDataMatcher,Doc>& findAction,CFindSetup& setup)
{
	ary_outdocs.ForAllAction(findAction);
	setup.ChangeMainGrid(OUTDOCSGRID );    
}