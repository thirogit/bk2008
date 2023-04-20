// ICellListener.h
//
#if !defined(_ICELLLISTENER_H_)
#define _ICELLLISTENER_H_

#include "cmString.h"
class cmBaseCell;

class ICmCellListener
{
public:
	virtual bool OnValueChanged(cmBaseCell* cell, double oldValue, double& newValue) = 0;
	virtual bool OnValueChanged(cmBaseCell* cell, long oldValue, long& newValue) = 0;
	virtual bool OnValueChanged(cmBaseCell* cell, bool oldValue, bool& newValue) = 0;
	virtual bool OnValueChanged(cmBaseCell* cell, const cmString& oldValue, cmString& newValue) = 0;
	virtual bool OnValueChanged(cmBaseCell* cell, const COleDateTime& oldValue, COleDateTime& newValue) = 0;
};

#endif //!defined(_ICELLLISTENER_H_)