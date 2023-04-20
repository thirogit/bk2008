
#include "stdafx.h"
#include "ColorOptionBtn.h"
#include "../MainCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


COLORREF CColorOptionBtn::m_CustColors[CUSTOM_COLORS_COUNT];
bool CColorOptionBtn::m_CustColorLoader = false;
int CColorOptionBtn::m_refCounter = 0;



IMPLEMENT_DYNCREATE(CColorOptionBtn, CXColourPicker)

BEGIN_MESSAGE_MAP(CColorOptionBtn, CXColourPicker)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
	ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)

END_MESSAGE_MAP()

CColorOptionBtn::CColorOptionBtn()
{
	m_refCounter++;
	if(!m_CustColorLoader)
	{		
		for(int i = 0;i < CUSTOM_COLORS_COUNT;i++)
		{
			m_CustColors[i] = AppCfg().GetRGB(CFG_CUSTOMCOLORS_ITEM,i);
		}
		m_CustColorLoader = true;
	}
}

CColorOptionBtn::~CColorOptionBtn()
{
	m_refCounter--;
	if(m_refCounter == 0)
	{
		if(m_CustColorLoader)
		{		
			for(int i = 0;i < CUSTOM_COLORS_COUNT;i++)
			{
				AppCfg().SetRGB(CFG_CUSTOMCOLORS_ITEM,i,m_CustColors[i]);
			}
			m_CustColorLoader = false;
		}
	}
}


LONG CColorOptionBtn::OnSelEndOK(WPARAM wParam, LPARAM lParam)
{
	GetCustomColors(m_CustColors);
	CXColourPicker::OnSelEndOK(wParam,lParam);
	return TRUE;
}

BOOL CColorOptionBtn::OnClicked()
{
	SetCustomColors(m_CustColors);
	CXColourPicker::OnClicked();
	return TRUE;
}

LONG CColorOptionBtn::OnSelEndCancel(WPARAM wParam, LPARAM)
{
	GetCustomColors(m_CustColors);
	CXColourPicker::OnSelEndCancel(wParam,0);
	return TRUE;
}

