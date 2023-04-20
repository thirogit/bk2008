// IUpdaterCfgUpdaterCfgValidator.h
//
#if !defined(_IUPDATERCFGUPDATERCFGVALIDATOR_H_)
#define _IUPDATERCFGUPDATERCFGVALIDATOR_H_

#include "enums.h"
#include "UpdaterCfgUpdaterCfg.h"
#include <vector>

using namespace std;

class IUpdaterCfgUpdaterCfgValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(UpdaterCfgUpdaterCfg& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(UpdaterCfgUpdaterCfg& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(UpdaterCfgUpdaterCfg& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnValidationRequest_BaseURL (cmString* newValue, vector<cmString>& msgList) = 0;
        virtual enValidationResult OnValidationRequest_VersionInfoFileName (cmString* newValue, vector<cmString>& msgList) = 0;
        virtual enValidationResult OnValidationRequest_ZipsDirName (cmString* newValue, vector<cmString>& msgList) = 0;
};

#endif //!defined(_IUPDATERCFGUPDATERCFGVALIDATOR_H_)
