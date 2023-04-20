#ifndef __RECOGNIZER_H__
#define __RECOGNIZER_H__

#include "FreeImagePlus.h"

class Recognizer
{
public:
	virtual CString RecognizeText(const fipWinImage& img) = 0;
	virtual CString RecognizeBarcode(const fipWinImage& img) = 0;
};



#endif 
