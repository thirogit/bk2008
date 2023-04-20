#ifndef __ORIANTATIONCTRL_H__
#define __ORIANTATIONCTRL_H__

#include "CCheckSK.h"




enum ORIENTATION_IDs
{
	HORIZONTAL = 300,
	HORIZONTAL_FLIPED = 600,
	VERTICAL_RIGHT = 900,
	VERTICAL_LEFT = 1200
};

class COrientationCtrl : public CStatic
{
public:
	COrientationCtrl();
	//SetOrientation(ORIANTATION orientation);
	//ORIANTATION GetOrientation();
	virtual ~COrientationCtrl();
	ORIENTATION GetOrientation();
	void SetOrientation(ORIENTATION orientation);
	UINT GetOrientationAngle();
	void SetOrientationAngle(UINT uiAngle);

protected:
	CCheckSK m_hor;
	CCheckSK m_horfliped;
	CCheckSK m_verl;
	CCheckSK m_verr;
	CCheckSK* m_lastChecked;

	void InitControls();
	virtual void PreSubclassWindow();
protected:
	afx_msg void OnHorizontal();
	afx_msg void OnHorizontalFliped();
	afx_msg void OnVerticalRight();
	afx_msg void OnVerticalLeft();
	//afx_msg void OnPaint() ;
	DECLARE_MESSAGE_MAP()
    
};

#endif