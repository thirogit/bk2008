// cmCellFactory.h: interface for the cmCellFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCELLFACTORY_H_)
#define _CMCELLFACTORY_H_

#include "cmCellEnum.h"
#include <set>

using namespace std;

class cmBaseCell;
class cmBaseCellInfo;

class cmCellFactory  
{
public:

	static cmBaseCell* CreateCell(enCmCellType type);
	static cmBaseCellInfo* CreateCellInfo();
	static void ReleaseCellInfo(cmBaseCellInfo* info);

protected:
	static set<cmBaseCellInfo*> infoList_;
};

#endif // !defined(_CMCELLFACTORY_H_)
