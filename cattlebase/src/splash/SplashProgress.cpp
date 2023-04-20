#include "stdafx.h"
#include "SplashProgress.h"

void CSplashProgress::SetTask(UINT taskID,int steps)
{
	AddTextLine((LPCSTR)taskID);
}
bool CSplashProgress::OnStep(int step)
{
	return true;
}
void CSplashProgress::OnTaskError(const CString& sError)
{

}
