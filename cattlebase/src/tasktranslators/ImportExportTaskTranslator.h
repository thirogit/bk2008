#ifndef __IMPORTEXPORTTASKTRANSLATOR_H__
#define __IMPORTEXPORTTASKTRANSLATOR_H__

#include "../taskprogress/TaskIdTranslator.h"

class ImportExportTaskTranslator : public TaskIdTranslator
{
public:
	ImportExportTaskTranslator();
private:
	const static UINT ImportExportTaskId2TaskName[][2];
};



#endif 
