// IPassProfileFIRSTOWNERValidator.h
//
#if !defined(_IPASSPROFILEFIRSTOWNERVALIDATOR_H_)
#define _IPASSPROFILEFIRSTOWNERVALIDATOR_H_

#include "enums.h"
#include "PassProfileFIRSTOWNER.h"
#include <vector>

using namespace std;

class IPassProfileFIRSTOWNERValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileFIRSTOWNER& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileFIRSTOWNER& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileFIRSTOWNER& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEFIRSTOWNERVALIDATOR_H_)
