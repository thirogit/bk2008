#ifndef __ONEITEMBTNMENU_H__
#define __ONEITEMBTNMENU_H__

#include "CoolBtn.h"
class COneItemBtnMenu : public CCoolBtn
{
public:
	COneItemBtnMenu(UINT nItemTextResId,UINT nItemId);
	BOOL Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
protected:
	virtual void PreSubclassWindow();
	void InitMenuButton();
	bool m_bInitiated;
	UINT m_nItemTextResId;
	UINT m_nItemId;
};
#endif