// IPassProfilePassProfileArrayValidator.h
//
#if !defined(_IPASSPROFILEPASSPROFILEARRAYVALIDATOR_H_)
#define _IPASSPROFILEPASSPROFILEARRAYVALIDATOR_H_

#include "enums.h"
#include "PassProfilePassProfile.h"
#include "PassProfilePassProfileArray.h"

class IPassProfilePassProfileArrayValidator
{
public:
    virtual enValidationResult OnInitialValidation(PassProfilePassProfileArray& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
    virtual enValidationResult OnFinalValidation(PassProfilePassProfileArray& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
    virtual enValidationResult OnItemChanged(int itemIndex, PassProfilePassProfile& item, cmString& notes, bool& highlightTreeNode) = 0;
    virtual enValidationResult OnBeforeItemDelete(PassProfilePassProfileArray& toValidate, PassProfilePassProfile& item, cmString& notes, bool& highlightTreeNode, bool& cancel) = 0;
    virtual enValidationResult OnBeforeItemAdd(PassProfilePassProfileArray& toValidate, cmString& notes, bool& highlightTreeNode, bool& cancel) = 0;
};

#endif //!defined(_IPASSPROFILEPASSPROFILEARRAYVALIDATOR_H_)
