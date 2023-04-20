// enums.h
//    
#if !defined(_ENUMS_H_)
#define _ENUMS_H_

typedef enum enValidationResult
{
    enValidationResult_Invalid		= 0,
    enValidationResult_Accepted	= 1,
    enValidationResult_Refused		= 2,
    enValidationResult_Error		= 4,
    enValidationResult_Warning		= 8,
    enValidationResult_Question	= 16,
    enValidationResult_Info		= 32,
    enValidationResult_Ok			= 64
} enValidationResult;

#endif //!defined(_ENUMS_H_)