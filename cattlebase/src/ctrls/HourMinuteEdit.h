#ifndef __HOURMINUTEEDIT_H__
#define __HOURMINUTEEDIT_H__

#include "tipedit.h"
#include "../classes/types/Time.h"

class CHourMinuteEdit : public CTipEdit
{
public:
	CHourMinuteEdit();
	bool IsValidTime();
	void SetTime(const Time& t);
	Time GetTime() const;
protected:	
	virtual BOOL OnValidate();
	virtual void PrepareToolTip(CWnd *pParent);

};


#endif