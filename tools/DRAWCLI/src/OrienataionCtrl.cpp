#include "stdafx.h"
#include "OrientationCtrl.h"

BEGIN_MESSAGE_MAP(COrientationCtrl, CStatic)
	ON_BN_CLICKED(HORIZONTAL,OnHorizontal)
	ON_BN_CLICKED(HORIZONTAL_FLIPED,OnHorizontalFliped)
	ON_BN_CLICKED(VERTICAL_RIGHT,OnVerticalRight)
	ON_BN_CLICKED(VERTICAL_LEFT,OnVerticalLeft)


END_MESSAGE_MAP()

//#if !defined(YAXISRISESUP)
COrientationCtrl::COrientationCtrl() : m_hor(ORIENTATION_HORIZONTAL),
                                       m_horfliped(ORIENTATION_HORIZONTAL_FLIPED),
									   m_verr(ORIENTATION_VERTICAL_RIGHT),
									   m_verl(ORIENTATION_VERTICAL_LEFT)
{
}
/*#else
COrientationCtrl::COrientationCtrl() : m_hor(ORIENTATION_HORIZONTAL),
                                       m_horfliped(ORIENTATION_HORIZONTAL_FLIPED),
									   m_verr(ORIENTATION_VERTICAL_LEFT),
									   m_verl(ORIENTATION_VERTICAL_RIGHT)
{
}
#endif*/

COrientationCtrl::~COrientationCtrl()
{

}
void COrientationCtrl::InitControls()
{
	
	CRect rc;
	GetClientRect(&rc);
	ASSERT(rc.Height() >= 90 && rc.Width() >= 90 && "Static control is to small, at least 90x90");
	rc.SetRect(30,0,60,30);
	m_hor.Create(rc,this,HORIZONTAL);
	rc.SetRect(0,30,30,60);
	m_verl.Create(rc,this,VERTICAL_LEFT);
	
	rc.SetRect(60,30,90,60);
	m_verr.Create(rc,this,VERTICAL_RIGHT);
	rc.SetRect(30,60,60,90);
	m_horfliped.Create(rc,this,HORIZONTAL_FLIPED);
	

	m_lastChecked = &m_hor;
	m_hor.SetCheck(TRUE);
	SetWindowText("");

}

void COrientationCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyleEx(GetExStyle(), WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP);
	InitControls();
}

void COrientationCtrl::OnHorizontal()
{
	m_lastChecked->SetCheck(FALSE);
	m_hor.SetCheck(TRUE);
	m_lastChecked = &m_hor;
}
void COrientationCtrl::OnHorizontalFliped()
{
	m_lastChecked->SetCheck(FALSE);
	m_horfliped.SetCheck(TRUE);
	m_lastChecked = &m_horfliped;
}
void COrientationCtrl::OnVerticalRight()
{
	m_lastChecked->SetCheck(FALSE);
	m_verr.SetCheck(TRUE);
	m_lastChecked = &m_verr;
}
void COrientationCtrl::OnVerticalLeft()
{
	m_lastChecked->SetCheck(FALSE);
	m_verl.SetCheck(TRUE);
	m_lastChecked = &m_verl;
}
ORIENTATION COrientationCtrl::GetOrientation()
{
	switch(m_lastChecked->GetDlgCtrlID())
	{
	case HORIZONTAL:
		return ORIENTATION_HORIZONTAL;
	case HORIZONTAL_FLIPED:
		return ORIENTATION_HORIZONTAL_FLIPED;
	case VERTICAL_RIGHT:
		return ORIENTATION_VERTICAL_RIGHT;
	case VERTICAL_LEFT:
		return ORIENTATION_VERTICAL_LEFT;
	
	}

	return ORIENTATION_HORIZONTAL;
}

void COrientationCtrl::SetOrientation(ORIENTATION orientation)
{
	switch(orientation)
	{
	case ORIENTATION_HORIZONTAL:
		OnHorizontal();
	break;
	case ORIENTATION_HORIZONTAL_FLIPED:
		OnHorizontalFliped();
	break;

	case ORIENTATION_VERTICAL_RIGHT:
		OnVerticalRight();
	break;
	case ORIENTATION_VERTICAL_LEFT:
		OnVerticalLeft();
	break;
	
	}
}

UINT COrientationCtrl::GetOrientationAngle()
{
	UINT uiAngle = 0;
	switch(GetOrientation())
	{
	case ORIENTATION_HORIZONTAL:
		uiAngle = 0;
	break;
	case ORIENTATION_HORIZONTAL_FLIPED:
		uiAngle = 180;
	break;

	case ORIENTATION_VERTICAL_RIGHT:
		uiAngle = 90;
	break;
	case ORIENTATION_VERTICAL_LEFT:
		uiAngle = 270;;
	break;
	
	}
	return uiAngle;
}

void COrientationCtrl::SetOrientationAngle(UINT uiAngle)
{
	ORIENTATION orien;
	switch(uiAngle)
	{
	default:
	case 0:
		orien = ORIENTATION_HORIZONTAL;
	break;
	case 180:
		orien = ORIENTATION_HORIZONTAL_FLIPED;
	break;
	case 90:
		orien = ORIENTATION_VERTICAL_RIGHT;
	break;
	case 270:
		orien = ORIENTATION_VERTICAL_LEFT;
	break;	
	}
	SetOrientation(orien);
}
