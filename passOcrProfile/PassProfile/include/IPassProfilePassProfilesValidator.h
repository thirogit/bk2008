// IPassProfilePassProfilesValidator.h
//
#if !defined(_IPASSPROFILEPASSPROFILESVALIDATOR_H_)
#define _IPASSPROFILEPASSPROFILESVALIDATOR_H_

#include "enums.h"
#include "PassProfilePassProfiles.h"
#include <vector>

using namespace std;

class IPassProfilePassProfilesValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfilePassProfiles& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfilePassProfiles& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfilePassProfiles& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
};

#endif //!defined(_IPASSPROFILEPASSPROFILESVALIDATOR_H_)
