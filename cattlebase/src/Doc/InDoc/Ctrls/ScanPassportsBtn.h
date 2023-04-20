#ifndef __SCANPASSPORTSBTN_H__
#define __SCANPASSPORTSBTN_H__

#include "../../../Ctrls/button/CoolBtn.h"

#define IDM_SCANNPASSPORTS_UPDATE (WM_APP+3000)
#define IDM_SCANNPASSPORTS_CONFIGURE (WM_APP+3001)

class CScanPassportsBtn : public CCoolBtn
{
public:
	CScanPassportsBtn();
	BOOL Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
protected:
	virtual void PreSubclassWindow();
	void CreateMenu();
	
};

#endif