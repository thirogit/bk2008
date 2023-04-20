#include "stdafx.h"
#include "curdateedit.h"
#include "global_arrays.h"

BEGIN_MESSAGE_MAP(CCurDateEdit,COXMaskedEdit)

	ON_WM_CREATE()

END_MESSAGE_MAP()


const char CCurDateEdit::m_DateMask[] = "####-##-##";

CCurDateEdit::CCurDateEdit(bool bSetCurDate/*= true*/) : m_bSetCurDate(bSetCurDate)
{

}
void CCurDateEdit::SetCurDate()
{
	SetDate(CDateTime::now());
}

void CCurDateEdit::SetDate(const CDateTime &date)
{
	if(date.IsNull()) EmptyData(TRUE);
	else
		SetInputData(date.GetDateInFmt(CDateTime::YYYYdashMMdashDD));
	
	
}

void CCurDateEdit::PreSubclassWindow()
{
	COXMaskedEdit::PreSubclassWindow();
	SetMask(m_DateMask);
	if(m_bSetCurDate) SetCurDate();
}


int CCurDateEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(COXMaskedEdit::OnCreate(lpCreateStruct) == -1) return -1;
	SetMask(m_DateMask);
	if(m_bSetCurDate) SetCurDate();

	return 0;
}

bool CCurDateEdit::IsDateValid()
{
	CDateTime tempdate;
	CString tempstr;
	if(IsInputEmpty()) 
		return false;
	else	
		GetWindowText(tempstr);
		
	return tempdate.ParseDate(CDateTime::YYYYdashMMdashDD,(LPCSTR)tempstr);
	
}

bool CCurDateEdit::GetDate(CDateTime &date)
{
	CDateTime tempdate;
	CString tempstr;

	if(IsInputEmpty()) 
	{
		date.MakeNull();
		return true;
	}
	else	
		GetWindowText(tempstr);
		
	return date.ParseDate(CDateTime::YYYYdashMMdashDD,(LPCSTR)tempstr);
	
	
	
}