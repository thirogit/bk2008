// IPassProfilePassFieldArrayValidator.h
//
#if !defined(_IPASSPROFILEPASSFIELDARRAYVALIDATOR_H_)
#define _IPASSPROFILEPASSFIELDARRAYVALIDATOR_H_

#include "enums.h"
#include "PassProfilePassField.h"
#include "PassProfilePassFieldArray.h"

class IPassProfilePassFieldArrayValidator
{
public:
    virtual enValidationResult OnInitialValidation(PassProfilePassFieldArray& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
    virtual enValidationResult OnFinalValidation(PassProfilePassFieldArray& toValidate, cmString& notes, bool& highlightTreeNode) = 0;
    virtual enValidationResult OnItemChanged(int itemIndex, PassProfilePassField& item, cmString& notes, bool& highlightTreeNode) = 0;
    virtual enValidationResult OnBeforeItemDelete(PassProfilePassFieldArray& toValidate, PassProfilePassField& item, cmString& notes, bool& highlightTreeNode, bool& cancel) = 0;
    virtual enValidationResult OnBeforeItemAdd(PassProfilePassFieldArray& toValidate, cmString& notes, bool& highlightTreeNode, bool& cancel) = 0;
};

#endif //!defined(_IPASSPROFILEPASSFIELDARRAYVALIDATOR_H_)
