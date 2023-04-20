#ifndef __COMBOENTEREDIT_H__
#define __COMBOENTEREDIT_H__

#include "../enteredit/enterEdit.h"

#define IDC_COMBOENTEREDIT 1001

class CComboEnterEdit : public CComboBox
{
public:
	CComboEnterEdit();
	virtual void PreSubclassWindow();
private:
	CEnterEdit m_enteredit;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStuct);
	afx_msg BOOL OnSelChange(/*NMHDR*,LRESULT *pResult*/);

	DECLARE_MESSAGE_MAP();
};




#endif 
