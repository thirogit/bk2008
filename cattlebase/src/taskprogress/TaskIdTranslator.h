#ifndef __TASKIDTRANSLATOR_H__
#define __TASKIDTRANSLATOR_H__

#include "../translator.h"

class TaskIdTranslator : Translator<UINT,UINT>
{
public:
	CString TraslateTaskId(UINT taskId);	
	UINT Translate(UINT taskId);
	void Add(UINT taskId,UINT nResStrId);
	
private:
	CMap<UINT,UINT,UINT,UINT> m_translateMap;
	
};



#endif 
