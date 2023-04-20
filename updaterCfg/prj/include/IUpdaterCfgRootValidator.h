// IUpdaterCfgRootValidator.h
//
#if !defined(_IUPDATERCFGROOTVALIDATOR_H_)
#define _IUPDATERCFGROOTVALIDATOR_H_

#include "enums.h"
#include "UpdaterCfgRoot.h"
#include <vector>

using namespace std;

class IUpdaterCfgRootValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(UpdaterCfgRoot& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(UpdaterCfgRoot& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(UpdaterCfgRoot& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IUPDATERCFGROOTVALIDATOR_H_)
