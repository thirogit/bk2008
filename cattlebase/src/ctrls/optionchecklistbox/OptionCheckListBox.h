#ifndef __OPTIONCHECKLISTBOX_H__
#define __OPTIONCHECKLISTBOX_H__

#include "CheckOption.h"

class COptionCheckListBox : public CCheckListBox
{
public:
	COptionCheckListBox();
	virtual ~COptionCheckListBox();
	void AddOption(UINT uiConfigId,const CString& sOptDesc,bool bValue);
protected:

	DECLARE_MESSAGE_MAP()
};

#endif 
