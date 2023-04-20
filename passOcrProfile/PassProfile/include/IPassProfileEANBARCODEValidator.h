// IPassProfileEANBARCODEValidator.h
//
#if !defined(_IPASSPROFILEEANBARCODEVALIDATOR_H_)
#define _IPASSPROFILEEANBARCODEVALIDATOR_H_

#include "enums.h"
#include "PassProfileEANBARCODE.h"
#include <vector>

using namespace std;

class IPassProfileEANBARCODEValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileEANBARCODE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileEANBARCODE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileEANBARCODE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEEANBARCODEVALIDATOR_H_)
