#include "stdafx.h"
#include "ReasonCombo.h"
#include "global_arrays.h"


CReasonCombo::CReasonCombo(Reason::REASON_INOUT_TYPE inout) : CExtendedComboBox(),m_inout(inout)
{
}

BOOL CReasonCombo::IsItemEnabled(UINT item) const
{
	ASSERT(GetCount() > 0);

	Reason *reason = (Reason*)GetItemData(item);
	Reason::REASON_INOUT_TYPE reasonType = reason->GetReasonType();
	switch(m_inout)
	{
	case Reason::REASON_BOTH:
		 return TRUE;
	case Reason::REASON_OUT:
	case Reason::REASON_IN :
		if(reasonType == Reason::REASON_BOTH) return TRUE;
		else return (reasonType == m_inout);
		
	}
	return FALSE;
}



void CReasonCombo::ReloadCombo(int initReason/*=0*/)
{
	int item, nSel = -1;
	ModifyStyle(CBS_SORT,0,0);
	ResetContent();
	Reason * reason;
	for(int i = 0;i< ary_reasons.GetSize();i++)
	{
		reason = ary_reasons[i];
        item = AddString(reason->GetReasonCode());
		SetItemData(item,(DWORD_PTR)reason);
		if(initReason == reason->GetId()) nSel = item;
	}
	SetCurSel(nSel);
}

Reason *CReasonCombo::GetSelReason()
{
	int nSel = GetCurSel();
	if(nSel < 0) return NULL;
	return (Reason*)GetItemData(nSel);
}



void CFindReasonCombo::InitFindCombo(Reason::REASON_INOUT_TYPE inout/* = Reason::REASON_BOTH*/)
{
	m_inout = inout;
	CBaseFindCombo<CReasonCombo>::InitFindCombo();
}
BOOL CFindReasonCombo::IsItemEnabled(UINT item) const
{
	if(m_nNeutralItem == (int)item) return TRUE;
	return CReasonCombo::IsItemEnabled(item);
}

