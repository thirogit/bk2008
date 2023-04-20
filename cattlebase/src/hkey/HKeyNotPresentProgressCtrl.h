#ifndef __HKEYNOTPRESENTPROGRESSCTRL_H__
#define __HKEYNOTPRESENTPROGRESSCTRL_H__

#include < Prof-UIS.h >

class CHardwareKeyNotPresentProgressCtrl : public CExtProgressWnd
{
public:
	CHardwareKeyNotPresentProgressCtrl();
protected:
	virtual LRESULT WindowProc(    
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		);	
}; 
#endif 
