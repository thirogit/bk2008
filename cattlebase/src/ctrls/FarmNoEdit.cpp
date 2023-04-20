#include "stdafx.h"
#include "FarmNoEdit.h"
#include "../consts.h"
#include "../missing/util.h"
#include "../MainCfg.h"

BEGIN_MESSAGE_MAP(CFarmNoEdit, CTipEdit)
    
END_MESSAGE_MAP()

bool CFarmNoEdit::CheckFarmNo(LPCSTR szFarmNo)
{
	return CUtil::IsFarmNoValid(szFarmNo);
}



CFarmNoEdit::CFarmNoEdit() : CTipEdit(g_EANMaskPL),m_bIgnore(false)
{
}


BOOL CFarmNoEdit::OnValidate()
{
	if(!IsValidFarmNo())
	{
		DisplayToolTip(TRUE);
		return FALSE;		
	}
	return TRUE;
}

void CFarmNoEdit::SetInitialValue(CString &initValue)
{
	m_initialValue = initValue;
	SetWindowText(initValue);
}


void CFarmNoEdit::PrepareToolTip(CWnd *pParent)
{
	CString ttText,ttTitle;
	ttText.LoadString(IDS_EANTOOLTIPTEXT);
	ttTitle.LoadString(IDS_EANTOOLTIPTITLE);
	CreateToolTip(pParent,ttText,TTI_ERROR,ttTitle);
}

bool CFarmNoEdit::IsValidFarmNo()
{
	if(AppCfg().GetBool(CFG_CHECKFARMNO))
	{
		CString data;
		if(IsInputEmpty()) return true;
		GetWindowText(data);
		if(data == m_initialValue || m_bIgnore) return true;
		return CheckFarmNo(data);		
	}
	return true;
}


