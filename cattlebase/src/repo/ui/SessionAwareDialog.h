#ifndef __SESSIONELEMENTVIEW_H__
#define __SESSIONELEMENTVIEW_H__

#include "SessionAwareDialogFacade.h"

template<class DLGCLASS>
class SessionElementView : public DLGCLASS,public SessionElementViewFacade
{
public:
	SessionElementView() : DLGCLASS((UINT)0,(CWnd*)NULL)
	{
	}

	virtual void Show()
	{
		ShowWindow(SW_SHOW);
	}

	virtual void Hide()
	{
		ShowWindow(SW_HIDE);
	}

};


#endif