// IPassProfilePASSDATEValidator.h
//
#if !defined(_IPASSPROFILEPASSDATEVALIDATOR_H_)
#define _IPASSPROFILEPASSDATEVALIDATOR_H_

#include "enums.h"
#include "PassProfilePASSDATE.h"
#include <vector>

using namespace std;

class IPassProfilePASSDATEValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfilePASSDATE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfilePASSDATE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfilePASSDATE& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEPASSDATEVALIDATOR_H_)
