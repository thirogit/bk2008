#ifndef __ACCOUNTNOEDIT_H__
#define __ACCOUNTNOEDIT_H__

#include "tipedit.h"

class CAccountNoEdit : public CTipEdit
{
public :
	CAccountNoEdit();
	bool IsValidAccountNo();
	
protected:	
	virtual BOOL OnValidate();
	virtual void PrepareToolTip(CWnd *pParent);

};


#endif