// ICmCellListener.h
//
#if !defined(_ICMCELLLISTENER_H_)
#define _ICMCELLLISTENER_H_

#include "cmString.h"
#include "cmEnums.h"


class ICmCellListener
{
public:
	virtual enCmValidationResult OnValueChanged(const cmString& fieldName, double oldValue, double& newValue) = 0;
	virtual enCmValidationResult OnValueChanged(const cmString& fieldName, long oldValue, long& newValue) = 0;
	virtual enCmValidationResult OnValueChanged(const cmString& fieldName, bool oldValue, bool& newValue) = 0;
	virtual enCmValidationResult OnValueChanged(const cmString& fieldName, const cmString& oldValue, cmString& newValue) = 0;

};

#endif //!defined(_ICMCELLLISTENER_H_)