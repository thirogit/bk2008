#ifndef __SPLASHPROGRESS_H__
#define  __SPLASHPROGRESS_H__

#include "SplashWnd.h"
#include "../taskprogress/ProgressIFace.h"

class CSplashProgress : public CSplashWnd,public ITaskProgres
{
public:
	virtual void SetTask(UINT taskID,int steps);
	virtual bool OnStep(int step);
	virtual void OnTaskError(const CString& sError);

};

#endif