#ifndef __RECOGNITIONENGINE_H__
#define __RECOGNITIONENGINE_H__

#include "Recognizer.h"
#include "../configuration/EngineSettings.h"

class RecognitionEngine : public Recognizer
{
public:
	virtual void Create(const EngineSettings& settings) = 0;
	virtual void Destroy() = 0;		
};



#endif 
