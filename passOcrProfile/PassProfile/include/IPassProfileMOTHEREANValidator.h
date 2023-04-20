// IPassProfileMOTHEREANValidator.h
//
#if !defined(_IPASSPROFILEMOTHEREANVALIDATOR_H_)
#define _IPASSPROFILEMOTHEREANVALIDATOR_H_

#include "enums.h"
#include "PassProfileMOTHEREAN.h"
#include <vector>

using namespace std;

class IPassProfileMOTHEREANValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileMOTHEREAN& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileMOTHEREAN& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileMOTHEREAN& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEMOTHEREANVALIDATOR_H_)
