// IPassProfilePASSNOValidator.h
//
#if !defined(_IPASSPROFILEPASSNOVALIDATOR_H_)
#define _IPASSPROFILEPASSNOVALIDATOR_H_

#include "enums.h"
#include "PassProfilePASSNO.h"
#include <vector>

using namespace std;

class IPassProfilePASSNOValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfilePASSNO& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfilePASSNO& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfilePASSNO& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEPASSNOVALIDATOR_H_)
