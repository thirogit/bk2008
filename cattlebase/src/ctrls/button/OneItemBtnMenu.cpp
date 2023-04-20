#include "stdafx.h"
#include "OneItemBtnMenu.h"

COneItemBtnMenu::COneItemBtnMenu(UINT nItemTextResId,UINT nItemId) :  m_bInitiated(false), 
								m_nItemTextResId(nItemTextResId), m_nItemId(nItemId)
{
}
BOOL COneItemBtnMenu::Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	if(!CCoolBtn::Create(lpszCaption,dwStyle,rect,pParentWnd,nID)) return FALSE;
	InitMenuButton();
	return 0;

}
void COneItemBtnMenu::PreSubclassWindow()
{
	CCoolBtn::PreSubclassWindow();
	InitMenuButton();

}

void COneItemBtnMenu::InitMenuButton()
{
	CString mentItemText;
	VERIFY(mentItemText.LoadString(m_nItemTextResId));
	  if(m_bInitiated) return;
		AddMenuItem(m_nItemId,(LPCSTR)mentItemText,0);
	
	  m_bInitiated = true;
}

