// IPassProfilePassProfileValidator.h
//
#if !defined(_IPASSPROFILEPASSPROFILEVALIDATOR_H_)
#define _IPASSPROFILEPASSPROFILEVALIDATOR_H_

#include "enums.h"
#include "PassProfilePassProfile.h"
#include <vector>

using namespace std;

class IPassProfilePassProfileValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfilePassProfile& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfilePassProfile& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfilePassProfile& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnValidationRequest_PassDimX (long* newValue, vector<cmString>& msgList) = 0;
        virtual enValidationResult OnValidationRequest_PassDimY (long* newValue, vector<cmString>& msgList) = 0;
        virtual enValidationResult OnValidationRequest_PassProfileName (cmString* newValue, vector<cmString>& msgList) = 0;
};

#endif //!defined(_IPASSPROFILEPASSPROFILEVALIDATOR_H_)
