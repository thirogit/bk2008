#include "stdafx.h"
#include "ScanPassportsBtn.h"
#include "../../../classes/TempStringLoader.h"

CScanPassportsBtn::CScanPassportsBtn() 
{
}

BOOL CScanPassportsBtn::Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	if(!CCoolBtn::Create(lpszCaption,dwStyle,rect,pParentWnd,nID)) return FALSE;
	CreateMenu();
	return 0;

}

void CScanPassportsBtn::PreSubclassWindow()
{
	CCoolBtn::PreSubclassWindow();
	CreateMenu();
}

void CScanPassportsBtn::CreateMenu()
{
	CString sUpdateMenuItemText = CTempStringLoader(IDS_SCANPASSMENUUPDATE);
	CString sConfigureItemText = CTempStringLoader(IDS_SCANPASSMENUCONFIGURE);

	AddMenuItem(IDM_SCANNPASSPORTS_UPDATE,(LPCSTR)sUpdateMenuItemText,0);
	AddMenuItem(IDM_SCANNPASSPORTS_CONFIGURE,(LPCSTR)sConfigureItemText,0);
	
	
}

