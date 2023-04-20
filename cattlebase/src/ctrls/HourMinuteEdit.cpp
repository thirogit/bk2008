#include "stdafx.h"
#include "HourMinuteEdit.h"

CHourMinuteEdit::CHourMinuteEdit() : CTipEdit("##:##")
{
	
}

Time CHourMinuteEdit::GetTime() const
{
	Time t;
	t.MakeNull();
	if(IsInputEmpty()) return t;
	if(!IsFullData()) return t;
	
	CString sTime;
	GetWindowText(sTime);
	t.ParseTime(Time::HHcolonMI,sTime,false);
	return t;
}

void CHourMinuteEdit::SetTime(const Time& t)
{
	SetWindowText(t.GetTime(Time::HHcolonMI));
}

bool CHourMinuteEdit::IsValidTime()
{
	if(IsInputEmpty()) return true;
	Time t;
	t = GetTime();
	return !t.IsNull();
}

BOOL CHourMinuteEdit::OnValidate()
{
	if(!COXMaskedEdit::OnValidate()) return FALSE;

	if(!IsValidTime())
	{
		DisplayToolTip(true);
		return FALSE;
	}
	return TRUE;
}
void CHourMinuteEdit::PrepareToolTip(CWnd *pParent)
{
	CreateToolTip(pParent,"Wpisz poprawn¹ godzine i minute w formacie HH:MM",2,"Niew³aœciwa wartoœæ czasu.");
}

