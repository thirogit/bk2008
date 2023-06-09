#pragma once


// CTransparentStatic2

#define CTransparentStatic CTransparentStatic2

class CTransparentStatic2 : public CStatic
{
	DECLARE_DYNAMIC(CTransparentStatic2)

public:
	CTransparentStatic2();
	virtual ~CTransparentStatic2();
	void SetTextColor(COLORREF clrText);
	

protected:
   afx_msg LRESULT OnSetText(WPARAM,LPARAM);
   afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
private:
   CBitmap m_Bmp;
   COLORREF m_clrText;
};


