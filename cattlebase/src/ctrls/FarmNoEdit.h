#ifndef __FARMNOEDIT_H__
#define __FARMNOEDIT_H__


#include "TipEdit.h"

class CFarmNoEdit : public CTipEdit
{
public:
	CFarmNoEdit();
	bool IsValidFarmNo();
	void SetInitialValue(CString &initValue);

protected:
	CString m_initialValue;	
	bool m_bIgnore;	
	bool CheckFarmNo(LPCSTR szFarmNo);

	virtual BOOL OnValidate();
	virtual void PrepareToolTip(CWnd *pParent);

DECLARE_MESSAGE_MAP()

};

#endif