#ifndef __CURDATEEDIT_H__
#define __CURDATEEDIT_H__

#include "../../classes/types/DateTime.h"
#include "../../maskededit/OXMaskedEdit.h"

class CCurDateEdit : public COXMaskedEdit
{
public:
	CCurDateEdit(bool bSetCurDate = true);
	void SetCurDate();
	void SetDate(const CDateTime &date);
	bool GetDate(CDateTime &date);
	bool IsDateValid();
private:
	virtual void PreSubclassWindow();
	bool m_bSetCurDate;
	static const char m_DateMask[]; 
	static const char m_DateFormat[];
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};


#endif