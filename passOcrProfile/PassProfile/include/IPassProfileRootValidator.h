// IPassProfileRootValidator.h
//
#if !defined(_IPASSPROFILEROOTVALIDATOR_H_)
#define _IPASSPROFILEROOTVALIDATOR_H_

#include "enums.h"
#include "PassProfileRoot.h"
#include <vector>

using namespace std;

class IPassProfileRootValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfileRoot& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfileRoot& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfileRoot& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEROOTVALIDATOR_H_)
