#if !defined(__PROGRESSIFACE_H__)
#define __PROGRESSIFACE_H__


interface ITaskProgres
{
	virtual void SetTask(UINT taskID,int steps) = 0;
	virtual bool OnStep(int step) = 0;
	virtual void OnTaskError(const CString& sError) = 0;
};

#endif 
