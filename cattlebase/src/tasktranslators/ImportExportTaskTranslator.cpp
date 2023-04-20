#include "stdafx.h"
#include "ImportExportTaskTranslator.h"
#include "../inout_port/ImportExportTasks.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



const UINT ImportExportTaskTranslator::ImportExportTaskId2TaskName[][2] =
{
	{WRITE_PRCSNG_DOCS,IDS_TASKNAME_PRCSNG_DOCS},
	{WRITE_PRCSNG_RRIVS,IDS_TASKNAME_PRCSNG_RRINVS},
	{WRITE_PRCSNG_HENTS,IDS_TASKNAME_PRCSNG_HENTS},
	{WRITE_PRCSNG_STOCKS,IDS_TASKNAME_PRCSNG_STOCKS},
	{WRITE_WRITING_TO_FILE,IDS_TASKNAME_WRITINGTOFILE},	
	{MERGING_STOCKS,IDS_TASKNAMEMERGINGSTOCKS},
	{MERGING_HENTS,IDS_TASKNAMEMERGINGHENTS},
	{FIND_DUPLICATES,IDS_TASKNAMEFINDINGDUPLICATES},
	{MERGING_COWS,IDS_TASKNAMEMERGINGCOWS},
	{MERGING_DOCS,IDS_TASKNAMEMERGINGDOCS},
	{INSERTING_PASSIMGS,IDS_INSERTINGPASSIMGS},
	{0,0}
};


ImportExportTaskTranslator::ImportExportTaskTranslator()
{
	for(const UINT *tasks = ImportExportTaskId2TaskName[0];
		tasks[0] && tasks[1];tasks += 2)
	{
		Add(tasks[0],tasks[1]);		
	}
}

