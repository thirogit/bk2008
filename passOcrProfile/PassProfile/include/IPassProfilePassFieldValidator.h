// IPassProfilePassFieldValidator.h
//
#if !defined(_IPASSPROFILEPASSFIELDVALIDATOR_H_)
#define _IPASSPROFILEPASSFIELDVALIDATOR_H_

#include "enums.h"
#include "PassProfilePassField.h"
#include <vector>

using namespace std;

class IPassProfilePassFieldValidator
{
public:
        virtual enValidationResult OnClassInitialValidation(PassProfilePassField& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassFinalValidation(PassProfilePassField& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnClassLostFocus(PassProfilePassField& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
        virtual enValidationResult OnValidationRequest_Barcode (bool* newValue, vector<cmString>& msgList) = 0;
        virtual enValidationResult OnValidationRequest_H (long* newValue, vector<cmString>& msgList) = 0;
        virtual enValidationResult OnValidationRequest_W (long* newValue, vector<cmString>& msgList) = 0;
        virtual enValidationResult OnValidationRequest_X (long* newValue, vector<cmString>& msgList) = 0;
        virtual enValidationResult OnValidationRequest_Y (long* newValue, vector<cmString>& msgList) = 0;
};

#endif //!defined(_IPASSPROFILEPASSFIELDVALIDATOR_H_)
