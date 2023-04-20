// IPassProfileBIRTHINFOValidator.h
//
#if !defined(_IPASSPROFILEBIRTHINFOVALIDATOR_H_)
#define _IPASSPROFILEBIRTHINFOVALIDATOR_H_

#include "enums.h"
#include "PassProfileBIRTHINFO.h"
#include <vector>

using namespace std;

class IPassProfileBIRTHINFOValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileBIRTHINFO& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileBIRTHINFO& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileBIRTHINFO& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEBIRTHINFOVALIDATOR_H_)
