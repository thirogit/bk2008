// IPassProfileEANValidator.h
//
#if !defined(_IPASSPROFILEEANVALIDATOR_H_)
#define _IPASSPROFILEEANVALIDATOR_H_

#include "enums.h"
#include "PassProfileEAN.h"
#include <vector>

using namespace std;

class IPassProfileEANValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileEAN& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileEAN& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileEAN& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEEANVALIDATOR_H_)
