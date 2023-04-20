#ifndef __DIFFERENCELIST_H__
#define __DIFFERENCELIST_H__

#include "../NewPtrFlushArray.h"

class DifferenceList : public CStatic
{
public:
	void Create(CWnd* pParentWnd);
	void AddRow(const DifferenceRow& row);
	
private:
	NewPtrFlushArray m_Rows;

};

#endif