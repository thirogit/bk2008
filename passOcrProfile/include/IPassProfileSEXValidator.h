// IPassProfileSEXValidator.h
//
#if !defined(_IPASSPROFILESEXVALIDATOR_H_)
#define _IPASSPROFILESEXVALIDATOR_H_

#include "enums.h"
#include "PassProfileSEX.h"
#include <vector>

using namespace std;

class IPassProfileSEXValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileSEX& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileSEX& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileSEX& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILESEXVALIDATOR_H_)
