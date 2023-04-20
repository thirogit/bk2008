#ifndef __COLOROPTIONBTN_H__INCLUDED
#define __COLOROPTIONBTN_H__INCLUDED

#include "XColourPicker.h"


class CColorOptionBtn : public CXColourPicker
{
	DECLARE_DYNCREATE(CColorOptionBtn);

public:
	CColorOptionBtn();
	virtual ~CColorOptionBtn();
private:
	static COLORREF m_CustColors[CUSTOM_COLORS_COUNT];
	static bool m_CustColorLoader;
	static int m_refCounter;


protected:
	afx_msg BOOL OnClicked();
	afx_msg LONG OnSelEndOK(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSelEndCancel(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


#endif 
