#ifndef __LOADTASKTRANSLATOR_H__
#define __LOADTASKTRANSLATOR_H__

#include "../taskprogress/TaskIdTranslator.h"

class LoadTaskTranslator : public TaskIdTranslator
{
public:
	LoadTaskTranslator();
private:
	const static UINT LoadingDBTaskId2TaskName[][2];
};



#endif 
