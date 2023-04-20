#ifndef __EXTRASMEMO_H__
#define __EXTRASMEMO_H__

#include "../dimedit/DimEditCtrl.h"

#define MAXTEXT 255
#define DIMCOLOR RGB( 0xFF, 0x80, 0x80 )

class CExtrasMemo : public CDimEditCtrl
{
public:
	CExtrasMemo();
protected:
	virtual void PreSubclassWindow();
	virtual void SetExtrasMemo();
private:
	static const char m_szMax255[];
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};


#endif