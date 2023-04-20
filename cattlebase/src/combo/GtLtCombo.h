#ifndef __GTLTCOMBO_H__
#define __GTLTCOMBO_H__

#define IT_IS_GT (1)
#define IT_IS_LT ((UINT)-1)
#define GTLT_NONE (0)

class CGtLtCombo : public CComboBox
{
public:
	//CSexCombo();
	virtual void InitCombo(UINT select = GTLT_NONE);
	UINT GetSelGtLt();
	void SelGtLt(UINT gtlt);
protected:
	//virtual void PreSubclassWindow();
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//DECLARE_MESSAGE_MAP();
};


#endif