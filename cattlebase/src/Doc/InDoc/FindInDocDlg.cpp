#include "stdafx.h"
#include "FindInDocDlg.h"
#include "../../mainfrm_ctrl.h"
#include "../../global_arrays.h"
#include "../../classes/TempStringLoader.h"

CFindInDocDlg::CFindInDocDlg () : CFindDocBaseDlg(CTempStringLoader(IDS_SEARCHININDOCS))
{

}


void CFindInDocDlg::InitControls()
{
	m_reason.InitFindCombo(Reason::REASON_IN);
}

void CFindInDocDlg::SearchDoc(CFindAction<CDocDataArray::ArrayAction,CDocDataMatcher,Doc>& findAction,CFindSetup& setup)
{
	ary_indocs.ForAllAction(findAction);
	setup.ChangeMainGrid(INDOCSGRID );
}
