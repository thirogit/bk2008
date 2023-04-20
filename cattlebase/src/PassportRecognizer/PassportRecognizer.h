#ifndef __PASSPORTRECOGNIZER_H__
#define __PASSPORTRECOGNIZER_H__

#include "../PassportCommon/fipWinImageEx.h"
#include "../../passOcrProfile/include/PassProfilePassProfile.h"
#include "engine/Recognizer.h"
#include "configuration/ImageSettings.h"
#include "RecognitionOperationResult.h"
#include <boost/shared_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

class PassportRecognitionResult;
class PassportRecognitionContext;

class PassportRecognizer
{
public:
	PassportRecognizer(Recognizer* pRecognizer);
    ~PassportRecognizer();
	
	PassportRecognitionResult Recognize(const CString& sPassImgFilePath,UINT uResolution,
										PassProfilePassProfile& passProfile,
										const ImageSettings* pImgSettings = NULL);
	PassportRecognitionResult Recognize(HGLOBAL hDIB,UINT uResolution,
										PassProfilePassProfile& passProfile,
										const ImageSettings* pImgSettings = NULL);

	PassportRecognitionResult RecognizeAgain(boost::shared_ptr<const PassportRecognitionContext> ptrRecognitionContext,PassProfilePassProfile& passProfile);

protected:
	const CString RecognizeField(const PassportRecognitionContext* pContext,const CString& fieldName,const PassProfilePassField& passField);
	const CString RecognizeBarcodeField(fipWinImage& fieldImg);
	const CString RecognizeTextField(fipWinImage& fieldImg);
	RecognitionOperationResult PrepareImage(PassportRecognitionContext* pContext);

	void TresholdFieldImage(fipWinImage& fieldImg);
	void CutFieldFromImage(const PassProfilePassField& passField,fipWinImage& outFieldImg);		
	bool FindPassportFrame(PassportRecognitionContext* pContext);
	void EnhancePalette(fipImage& img, int iSIGMA,int iCENTER);

	void Median(fipWinImage &inImage,fipWinImage &outImage);
	void SORT_SLICE(BYTE a[]);
	void MERGE_SLICE(BYTE a[], BYTE b[], BYTE bc[]);
    BYTE CALC_MEDIAN(BYTE x[], BYTE bc[]);

	void SaveDebugImage(LPCSTR imgFileName,fipImage &image);
	Recognizer* m_pRecognizer;
};


#endif