#ifndef __ABBYY80RECOGNITIONENGINE_H__
#define __ABBYY80RECOGNITIONENGINE_H__

#include "../RecognitionEngine.h"
#include "FREngine.h"
#include "../../../Utils/SafePtr.h"

typedef HRESULT ( STDAPICALLTYPE* GetEngineObjectFunc )( BSTR, BSTR, BSTR, IEngine** );
typedef HRESULT ( STDAPICALLTYPE* DeinitializeEngineFunc )();

class Abbyy80RecognitionEngine : public RecognitionEngine
{
public:
	Abbyy80RecognitionEngine();
	virtual CString RecognizeText(const fipWinImage& img);
	virtual CString RecognizeBarcode(const fipWinImage& img);

	void Create(const EngineSettings& settings);
	void Destroy();

private:
	bool UnloadFREngine();
	bool LoadFREngine(const CString& sFreDllPath);
	void ThrowIfFailed(HRESULT hResult);
	CString GetPlainText(IText* pText);
	
private:
	HMODULE m_FRDllHandle;
	CSafePtr<IEngine> m_Engine;

	GetEngineObjectFunc m_pGetEngineObjectFn;
	DeinitializeEngineFunc m_pDeinitializeEngineFn;
};



#endif 
