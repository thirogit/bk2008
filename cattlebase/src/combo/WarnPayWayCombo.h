#ifndef __WARNPAYWAYCOMBO_H__
#define __WARNPAYWAYCOMBO_H__

#include "PaywayCombo.h"


class CWarnPaywayCombo : public CStatic
{
public:
	CWarnPaywayCombo();
	void ReloadCombo(Invoice::PAYWAYS curSel = Invoice::PAY_TRANS);
	void SetWarnBlinkInterval(int miliseconds);
	void SetHent(Hent *pHent);

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	Invoice::PAYWAYS GetPayway();
	int GetCurSel();
	void SetWarn(bool bWarn);
	
protected:
	int m_warnBlinkIntervalMili;
	CPaywayCombo m_payway;
	CImageList m_warnBitmap;
	Hent *m_pHent;
	bool m_bWarn;

	virtual void PreSubclassWindow();
	BOOL InitControls();
	void PosControls(void);
	

protected:
	afx_msg void OnPaywayChange();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEnable(BOOL bEnable); 
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP();
};



#endif