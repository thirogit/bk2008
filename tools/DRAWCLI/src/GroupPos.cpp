// GroupPos.cpp : implementation file
//

#include "stdafx.h"
#include "DRAWCLI.h"
#include "GroupPos.h"


// CGroupPos dialog

IMPLEMENT_DYNAMIC(CGroupPos, CDialog)
CGroupPos::CGroupPos(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupPos::IDD, pParent)
{
	PosX = 0;
	negPosY = 0;
}

CGroupPos::~CGroupPos()
{
}

void CGroupPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_VERT,m_VSlider);
	DDX_Control(pDX,IDC_HORZ,m_HSlider);
	
}

BEGIN_MESSAGE_MAP(CGroupPos, CDialog)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


BOOL CGroupPos::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;

	m_HSlider.SetRangeMin(-200);
	m_HSlider.SetRangeMax(200);
	m_HSlider.SetPos(0);
	m_HSlider.SetTicFreq(5);

	m_VSlider.SetRangeMin(-200);
	m_VSlider.SetRangeMax(200);
	m_VSlider.SetPos(0);
	m_VSlider.SetTicFreq(5);

	SetStatics();
	return TRUE;

}


void CGroupPos::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int min,max,page;
	bool l_GoPaint = false;
	
	switch(nSBCode)
	{
	case SB_THUMBTRACK:
	case TB_LINEDOWN:
	case TB_LINEUP:
	case TB_TOP:
	case SB_THUMBPOSITION:
	
		
		PosX = nPos;
		l_GoPaint = true;
	
		break;

	case  SB_PAGEDOWN:

		max = m_HSlider.GetRangeMax();
		page = m_HSlider.GetPageSize();
		
		if((PosX + page) < max) PosX += page;
		else PosX = max;
		l_GoPaint = true;
	
		break;
	case SB_PAGEUP:

		min = m_HSlider.GetRangeMin();
		
		page = m_HSlider.GetPageSize();
	
			if((PosX - page) > min) PosX -= page;
		else PosX = min;
		l_GoPaint = true;

		break;
	}

	if(l_GoPaint)
	{
		SetStatics();
	}


}

void CGroupPos::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int min,max,page;
	bool l_GoPaint = false;
	
	switch(nSBCode)
	{
	case SB_THUMBTRACK:
	case TB_LINEDOWN:
	case TB_LINEUP:
	case TB_TOP:
	case SB_THUMBPOSITION:
	
		negPosY = nPos;
		l_GoPaint = true;
		break;

	case  SB_PAGEDOWN:

		max = m_VSlider.GetRangeMax();
		page = m_VSlider.GetPageSize();
	
		
		if((negPosY + page) < max) negPosY += page;
		else negPosY = max;
		l_GoPaint = true;
		break;
	case SB_PAGEUP:

		min = m_VSlider.GetRangeMin();
		
		page = m_VSlider.GetPageSize();

			if((negPosY - page) > min) negPosY -= page;
		else negPosY = min;
	l_GoPaint = true;

		break;
	}

	if(l_GoPaint)
	{

		SetStatics();
	
	}


}

CSize CGroupPos::GetPos()
{
	return CSize(PosX,negPosY);
}

void CGroupPos::SetStatics()
{
		SetDlgItemInt(IDC_POSHORZ,PosX);
		SetDlgItemInt(IDC_POSVERT,-negPosY);
}