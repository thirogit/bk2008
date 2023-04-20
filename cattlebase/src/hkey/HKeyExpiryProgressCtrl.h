#ifndef __HKEYEXPIRYPROGRESSCTRL_H__
#define __HKEYEXPIRYPROGRESSCTRL_H__

#include < Prof-UIS.h >

class CHardwareKeyExpiryProgressCtrl : public CExtProgressWnd
{
public:
	CHardwareKeyExpiryProgressCtrl();
protected:
	virtual LRESULT WindowProc(    
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		);
	
}; 
#endif 
