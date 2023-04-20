// IPassProfileSTOCKValidator.h
//
#if !defined(_IPASSPROFILESTOCKVALIDATOR_H_)
#define _IPASSPROFILESTOCKVALIDATOR_H_

#include "enums.h"
#include "PassProfileSTOCK.h"
#include <vector>

using namespace std;

class IPassProfileSTOCKValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileSTOCK& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileSTOCK& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileSTOCK& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILESTOCKVALIDATOR_H_)
