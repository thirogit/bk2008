#ifndef __WILDFILTEREDIT_H__
#define __WILDFILTEREDIT_H__


#include "../FilterEdit/FilterEdit.h"

class CWildFilterEdit : public CFilterEdit
{
public:
	CWildFilterEdit(int nAlphaChars,int nDigitChars);
	CString GetWindowText();
};


#endif