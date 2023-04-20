// cmEnums.h
//
#if !defined(_CM_ENUMS_H)
#define _CM_ENUMS_H

typedef enum enCmValidationResult
{
	enCmValidationResult_Accepted = 1,	// validation process confirmed the proposed value
	enCmValidationResult_Refused = 2,	// validation process refused the proposed value (keep the previous)
	enCmValidationResult_Error = 4,		// validated item is marked as ERROR
	enCmValidationResult_Warning = 8,	// validated item is marked as WARNING
	enCmValidationResult_Question = 16,	// validated item is marked as QUESTION
	enCmValidationResult_Info = 32,		// validated item is marked as INFO
	enCmValidationResult_Ok = 64		// validated item is not marked at all
}; 
#endif //!defined(_CM_ENUMS_H)