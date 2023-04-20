#include "stdafx.h"
#include "ExtrasMemo.h"


const char CExtrasMemo::m_szMax255[] =  _T( "(Max. 255 znaków)" );

BEGIN_MESSAGE_MAP(CExtrasMemo,CDimEditCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()


CExtrasMemo::CExtrasMemo() : CDimEditCtrl()
{
}

void CExtrasMemo::PreSubclassWindow()
{
	CDimEditCtrl::PreSubclassWindow();
	SetExtrasMemo();
	
}

void CExtrasMemo::SetExtrasMemo()
{
	LimitText( MAXTEXT );
	SetDimText( m_szMax255 );
	SetDimColor( DIMCOLOR);
}

int CExtrasMemo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CDimEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetExtrasMemo();
	return 0;
}