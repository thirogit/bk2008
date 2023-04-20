// IPassProfileFARMNOBARCODEValidator.h
//
#if !defined(_IPASSPROFILEFARMNOBARCODEVALIDATOR_H_)
#define _IPASSPROFILEFARMNOBARCODEVALIDATOR_H_

#include "enums.h"
#include "PassProfileFARMNOBARCODE.h"
#include <vector>

using namespace std;

class IPassProfileFARMNOBARCODEValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileFARMNOBARCODE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileFARMNOBARCODE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileFARMNOBARCODE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEFARMNOBARCODEVALIDATOR_H_)
