#include "stdafx.h"

#include "PassportRecognizer.h"
#include "RegressionLine.hpp"
//#include "OCRCfg.h"
#include "../log.h"
#include "../missing/util.h"
#include "../missing/paths.h"
#include "../Utils/StringFormat.h"
#include "../classes/types/DateTime.h"
#include "../classes/TempStringLoader.h"
#include "PassportRecognitionResult.h"
#include "PassportRecognitionContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef M_PI 
#define M_PI 3.14159265358979323846
#endif

#define FROM_LEFT_START_SEARCH_AFTER 550 // 5.5cm
#define FROM_TOP_START_SEARCH_AFTER 500 //5cm
#define CHECK_EDGE_EVERY 200 //2cm
#define FINISH_SEARCH_AFTER 200 //2cm
#define PROBE_TOP_EDGE_TIMES 5 // 5 times
#define PROBE_LEFT_EDGE_TIMES 4 //4 times
#define MAX_SKEW_DEG 5.0
#define MIN_LINE_THIKNESS (2)
#define PROBE_STEP_WIDTH (5) 
#define ZERO_ANGLE_EPS (0.5*(M_PI/180))

#define SWAP_GRAY(a, b) { BYTE t; t = b; b = a; a = t; }
#define SWAP_SLICE(a, b) { BYTE *t; t = b; b = a; a = t; }

#if _DEBUG
#define SAVEDEBUGIMGS
#endif

PassportRecognizer::PassportRecognizer(Recognizer* pRecognizer) : m_pRecognizer(pRecognizer)
{	
}

PassportRecognizer::~PassportRecognizer() 
{	
}


PassportRecognitionResult PassportRecognizer::Recognize(const CString& sPassImgFilePath,UINT uResolution,
														PassProfilePassProfile& passProfile,
														const ImageSettings* pImgSettings /* = NULL*/)
{
	PassportRecognitionContext* pContext = new PassportRecognitionContext();
	boost::shared_ptr<const PassportRecognitionContext> ptrContext(pContext);

	if(pContext->m_RawPassportImage.load(sPassImgFilePath))
	{
		pContext->m_dpiResolution = uResolution;
		if(pImgSettings)
			pContext->m_ImgSettings = *pImgSettings;
		else
			pContext->m_ImgSettings = ImageSettings();

		RecognitionOperationResult prepareResult = PrepareImage(pContext);
		if(prepareResult.GetStatus() != RecognitionStatus_Success)
		{
			return PassportRecognitionResult(prepareResult,ptrContext);
		}

		
		return RecognizeAgain(ptrContext,passProfile);
	}
	else
	{
		CString sMessage = CStringFormat((LPCSTR)CTempStringLoader(IDS_OCRERRFAILTOLOADIMGFROMFILE),(LPCSTR)sPassImgFilePath);
		return PassportRecognitionResult(RecognitionOperationResult(RecognitionStatus_Failure,sMessage),boost::shared_ptr<const PassportRecognitionContext>());
	}
}

PassportRecognitionResult PassportRecognizer::Recognize(HGLOBAL hDIB,UINT uResolution,
														PassProfilePassProfile& passProfile,
													    const ImageSettings* pImgSettings /* = NULL*/)
{

	PassportRecognitionContext* pContext = new PassportRecognitionContext();
	boost::shared_ptr<const PassportRecognitionContext> ptrContext(pContext);

	if(pContext->m_RawPassportImage.copyFromHDIB(hDIB))
	{
		pContext->m_dpiResolution = uResolution;
		if(pImgSettings)
			pContext->m_ImgSettings = *pImgSettings;
		else
			pContext->m_ImgSettings = ImageSettings();

		RecognitionOperationResult prepareResult = PrepareImage(pContext);
		if(prepareResult.GetStatus() != RecognitionStatus_Success)
		{
			return PassportRecognitionResult(prepareResult,ptrContext);
		}

		return RecognizeAgain(ptrContext,passProfile);
	}
	else
	{
		
		CString sMessage = CStringFormat((LPCSTR)CTempStringLoader(IDS_OCRERRFAILETOLOADFROMDIB),hDIB);			
		return PassportRecognitionResult(RecognitionOperationResult(RecognitionStatus_Failure,sMessage),boost::shared_ptr<const PassportRecognitionContext>());
	}
}

PassportRecognitionResult PassportRecognizer::RecognizeAgain(boost::shared_ptr<const PassportRecognitionContext> ptrRecognitionContext,
															 PassProfilePassProfile& passProfile)
{
	//CSize passSize(passProfile.GetPassDimX(),passProfile.GetPassDimY());
	//int right = m_Org.x + (passSize.cx*m_dpiResolution)/254,
	//	bottom = m_Org.y + (passSize.cy*m_dpiResolution)/254;
	//
	//VERIFY(m_PassImg.copySubImage(m_croppedPassImg,m_Org.x,m_Org.y,right,bottom));

	const PassportRecognitionContext* pContext = ptrRecognitionContext.get();

	CString cowSex = RecognizeField(pContext,passProfile.GetSEX().GetXmlNodeName(),passProfile.GetSEX().GetPassField());
	CString passDate = RecognizeField(pContext,passProfile.GetPASSDATE().GetXmlNodeName(),passProfile.GetPASSDATE().GetPassField());
	CString passNo = RecognizeField(pContext,passProfile.GetPASSNO().GetXmlNodeName(),passProfile.GetPASSNO().GetPassField());
	CString cowNoBC = RecognizeField(pContext,passProfile.GetEANBARCODE().GetXmlNodeName(),passProfile.GetEANBARCODE().GetPassField());
	CString cowNo = RecognizeField(pContext,passProfile.GetEAN().GetXmlNodeName(),passProfile.GetEAN().GetPassField());
	CString farmNoBC = RecognizeField(pContext,passProfile.GetFARMNOBARCODE().GetXmlNodeName(),passProfile.GetFARMNOBARCODE().GetPassField());
	CString birthDatePlace = RecognizeField(pContext,passProfile.GetBIRTHINFO().GetXmlNodeName(),passProfile.GetBIRTHINFO().GetPassField());
	CString cowStock = RecognizeField(pContext,passProfile.GetSTOCK().GetXmlNodeName(),passProfile.GetSTOCK().GetPassField());
	CString motherNo = RecognizeField(pContext,passProfile.GetMOTHEREAN().GetXmlNodeName(),passProfile.GetMOTHEREAN().GetPassField());
	CString fstOwner = RecognizeField(pContext,passProfile.GetFIRSTOWNER().GetXmlNodeName(),passProfile.GetFIRSTOWNER().GetPassField());

	//LOG("DoOCR(): PASSDATE = %s,PASSNO = %s,EAN = %s,BIRTH = %s,SEX = %s,STOCK = %s,MOTHEREAN = %s,FSTOWNRADR = %s,EANBC = %s,FARMNOBC= %s",
    //    (LPCSTR)passDate,(LPCSTR)passNo,(LPCSTR)cowNo,(LPCSTR)birthDatePlace,(LPCSTR)cowSex,
	//	(LPCSTR)cowStock,(LPCSTR)motherNo,(LPCSTR)fstOwner,(LPCSTR)cowNoBC,(LPCSTR)farmNoBC);


	PassportRecognitionResult result(RecognitionOperationResult(RecognitionStatus_Success),ptrRecognitionContext);
	
	result.SetCowSex(cowSex);
	result.SetPassDate(passDate);
	result.SetPassNo(passNo);
	result.SetCowNoBC(cowNoBC);
	result.SetCowNo(cowNo);
	result.SetFarmNoBC(farmNoBC);
	result.SetBirthDatePlace(birthDatePlace);
	result.SetCowStock(cowStock);
	result.SetMotherNo(motherNo);
	result.SetFstOwner(fstOwner);

	return result;
}


RecognitionOperationResult PassportRecognizer::PrepareImage(PassportRecognitionContext* pContext)
{		
	pContext->m_PreparedPassportImage = pContext->m_RawPassportImage;
	

	//	if(OCRCfg().GetBool(CFG_DO_PAL_ENHANCE))
	if(pContext->m_ImgSettings.GetDoPaletteEnhance())
	{
		EnhancePalette(pContext->m_PreparedPassportImage,pContext->m_ImgSettings.GetPaletteSigma(),pContext->m_ImgSettings.GetPaletteCenter());
	}

	pContext->m_PreparedPassportImage.convertTo8Bits();

	//BLACK_WHITE_SPLIT_LEVEL = OCRCfg().GetNum(CFG_BLACKLEVEL);


	double dSkew = 0.0;
	CPoint originPt(0,0);

	if(!FindPassportFrame(pContext))
	{
		return RecognitionOperationResult(RecognitionStatus_Failure,CTempStringLoader(IDS_OCRERRANALISEERROR));
	}	

	if(dSkew != 0.0)
	{
		double dSkewDeg = (dSkew*180)/M_PI;
		
		if(fabs(dSkewDeg) > MAX_SKEW_DEG)
		{
			return RecognitionOperationResult(RecognitionStatus_Failure,CTempStringLoader(IDS_OCRERRSKEWERR));		
		}

		pContext->m_PreparedPassportImage.rotate(-dSkewDeg);

		SaveDebugImage("AfterRotate",pContext->m_PreparedPassportImage);

		if(!FindPassportFrame(pContext)) 
		{				
			return RecognitionOperationResult(RecognitionStatus_Failure,CTempStringLoader(IDS_OCRERRANALISEERROR));
		}
	}
	return RecognitionOperationResult(RecognitionStatus_Success);
}



bool PassportRecognizer::FindPassportFrame(PassportRecognitionContext* pContext)
{
	fipWinImage median;
	fipWinImage edgeDetect;

	int BLACK_WHITE_SPLIT_LEVEL = pContext->m_ImgSettings.GetBlackLevel();

	const int flssa_du = (FROM_LEFT_START_SEARCH_AFTER*pContext->m_dpiResolution) / 254,
			  ftssa_du = (FROM_TOP_START_SEARCH_AFTER*pContext->m_dpiResolution) / 254,
			  cee_du   = (CHECK_EDGE_EVERY*pContext->m_dpiResolution) / 254,
			  fsa_du   = (FINISH_SEARCH_AFTER*pContext->m_dpiResolution) / 254;

	//tutaj assery rozmiaru
	
	pContext->m_PreparedPassportImage.copySubImage(edgeDetect,flssa_du,0,flssa_du +(PROBE_TOP_EDGE_TIMES)*CHECK_EDGE_EVERY,fsa_du);
		
	Median(edgeDetect,median);

	int i,j,ttimes,ltimes,lineThikness;

	double topA,topB,leftA,leftB;
	bool bFound;

	BYTE *pLine,*pLineProbe1,*pLineProbe2,*pLineProbe3;

	RegressionLine::Points pnts;
	pnts.clear();

	int probe1,probe2,probe3;

	int imgx =  median.getWidth(),
		imgy = median.getHeight();
	
	for(i = PROBE_STEP_WIDTH + 1,ttimes = 0;i < imgx && ttimes < PROBE_TOP_EDGE_TIMES;i += cee_du,ttimes++)
	{
		j = 0;
		while(j < (fsa_du) && j < imgy)
		{
			pLine = median.getScanLine(imgy-j-1);
			if(pLine[i] > BLACK_WHITE_SPLIT_LEVEL) // if its grather than 0
				break;

			j++;
		}

		//TRACE("Skip top %d black pixels\r\n",j);
		bFound = false;
		lineThikness = 0;
		for(; j < (fsa_du) && j < imgy;j++) 
		{
			pLine = median.getScanLine(imgy-j-1);

			probe1 = pLine[i-PROBE_STEP_WIDTH] <= BLACK_WHITE_SPLIT_LEVEL ? 1 : 0;
			probe2 = pLine[i] <= BLACK_WHITE_SPLIT_LEVEL ? 1 : 0;
			probe3 = pLine[i+PROBE_STEP_WIDTH] <= BLACK_WHITE_SPLIT_LEVEL ? 1 : 0;


			pLine[i-PROBE_STEP_WIDTH] = 0;
			pLine[i] = 0;
			pLine[i+PROBE_STEP_WIDTH] = 0;

			

			
			if(probe1+probe2+probe3 >= 2) //at least two probes found line
			{
				lineThikness++;
				if(lineThikness >= MIN_LINE_THIKNESS)
				{
					pnts[(double)i] = -(double)(j);
					bFound = true;
					//TRACE("TOP PROBE %d,AT %d,  RESULT = %d\n",ttimes,i,j);
					break;
				}
			}


		}
		if(!bFound) break; //didnt find all points
	}

	SaveDebugImage("median_probesTop",median);
	
	if(ttimes !=  PROBE_TOP_EDGE_TIMES) return false;
	
	RegressionLine topLine(pnts);

	//TRACE("Top regresion coef = %f\r\n",topLine.regressionCoefficient());

	topA = topLine.slope();
	topB = topLine.yIntercept();

	//TRACE("TOP ANGLE %f\r\n",(atan(topA)*180)/M_PI);
	//TRACE("TOP A = %f,TOP B = %f\n",topA,topB);

	pnts.clear();

	pContext->m_PreparedPassportImage.copySubImage(edgeDetect,0,ftssa_du,fsa_du,ftssa_du +(PROBE_LEFT_EDGE_TIMES)*CHECK_EDGE_EVERY);
	
	Median(edgeDetect,median);

	imgx =  median.getWidth();
	imgy = median.getHeight();


	for(j = PROBE_STEP_WIDTH + 1,ltimes = 0;j < imgy && ltimes < PROBE_LEFT_EDGE_TIMES;j += cee_du,ltimes++)
	{
		pLineProbe1 = median.getScanLine(imgy-j-1-PROBE_STEP_WIDTH);
		pLineProbe2 = median.getScanLine(imgy-j-1);
		pLineProbe3 = median.getScanLine(imgy-j-1+PROBE_STEP_WIDTH);
		
		
		i = 0;
		while(i < (fsa_du) && i < imgx)
		{
			if(pLineProbe2[i] > BLACK_WHITE_SPLIT_LEVEL)
				break;
			i++;
		}

		//TRACE("Skip left %d black pixels\r\n",i);
		bFound = false;
		lineThikness = 0;
		for(;i < (fsa_du) && i < imgx;i++)
		{

			probe1 = pLineProbe1[i] <= BLACK_WHITE_SPLIT_LEVEL ? 1 : 0;
			probe2 = pLineProbe2[i] <= BLACK_WHITE_SPLIT_LEVEL ? 1 : 0;
			probe3 = pLineProbe3[i] <= BLACK_WHITE_SPLIT_LEVEL ? 1 : 0;


			pLineProbe1[i] = 0;
			pLineProbe2[i] = 0;
			pLineProbe3[i] = 0;


			 
			if(probe1+probe2+probe3 >= 2)
			{
				lineThikness++;
				if(lineThikness >= MIN_LINE_THIKNESS)
				{
					pnts[(double)j] = (double)(i);
					bFound = true;
	//				TRACE("LEFT PROBE %d,AT %d,  RESULT = %d\n",ltimes,j,i);
					break;
				}
			}
		}
		if(!bFound) break;

	}
	if(ltimes !=  PROBE_LEFT_EDGE_TIMES) return false;

	SaveDebugImage("median_probesLeft",median);

	RegressionLine leftLine(pnts);

	//TRACE("Left regresion coef = %f\r\n",leftLine.regressionCoefficient());

	leftA = leftLine.slope();
	leftB = leftLine.yIntercept();

	double leftAngle = atan(leftA),topAngle = atan(topA);
	
	if(topAngle < ZERO_ANGLE_EPS && topAngle > (-ZERO_ANGLE_EPS))
		pContext->m_Skew = 0.0;
	else
		pContext->m_Skew = topAngle;


	if(leftAngle < ZERO_ANGLE_EPS && leftAngle > (-ZERO_ANGLE_EPS))
	{
		pContext->m_Org.x = (int)(leftB);
		pContext->m_Org.y = -(int)(topA*leftB + topB);
	}
	else
	{
		leftB = -leftB/leftA;
		leftA = 1/leftA;

		pContext->m_Org.x = (int)((leftB-topB)/(topA-leftA));
		pContext->m_Org.y = -(int)(topA*pContext->m_Org.x + topB);
	}

	//TRACE("LEFT ANGLE %f\r\n",(atan(leftA)*180)/M_PI);
	//TRACE("LEFT A = %f,LEFT B = %f\n",leftA,leftB);
	//TRACE("Point of orgin : x = %d, y = %d\r\n",m_Org.x,m_Org.y);
	return true;
}

void PassportRecognizer::SaveDebugImage(LPCSTR imgFileName,fipImage &image)
{
#ifdef SAVEDEBUGIMGS
	CString imgPath = paths_ConcatPath(CUtil::GetAppDir(),CString("OCRIMGDBG"));
	paths_EnsurePathExist(imgPath);
	CString imgFilePath = paths_ConcatPath(imgPath,CStringFormat("%s.BMP",(LPCSTR)imgFileName));
	image.save((LPCSTR)imgFilePath);
#endif
}



const CString PassportRecognizer::RecognizeField(const PassportRecognitionContext* pContext,
												 const CString& fieldName, 
												 const PassProfilePassField& passField)
{
	fipWinImage fieldImg;

	int dpiResolution = pContext->m_dpiResolution;

	int l = pContext->m_Org.x + (passField.GetX()*dpiResolution)/254;
	int t = pContext->m_Org.y + (passField.GetY()*dpiResolution)/254;
	int w = (passField.GetW()*dpiResolution)/254;
	int h = (passField.GetH()*dpiResolution)/254;
	int r = l+w;
	int b = t+h;

	pContext->m_PreparedPassportImage.copySubImage(fieldImg,l,t,l+w,t+h);
	
	fieldImg.adjustContrast(30);
	fieldImg.threshold(pContext->m_ImgSettings.GetBlackLevel());

	SaveDebugImage(fieldName,fieldImg);

	if(passField.GetBarcode())
		return RecognizeBarcodeField(fieldImg);
	else
		return RecognizeTextField(fieldImg);

}

const CString PassportRecognizer::RecognizeTextField(fipWinImage& fieldImg)
{	
	return m_pRecognizer->RecognizeText(fieldImg);
}

const CString PassportRecognizer::RecognizeBarcodeField(fipWinImage& fieldImg)
{
	return m_pRecognizer->RecognizeBarcode(fieldImg);
}


void PassportRecognizer::EnhancePalette(fipImage& img, int iSIGMA,int iCENTER)
{
	WORD palSize = img.getPaletteSize();
	int numColors = palSize/sizeof(RGBQUAD);
	RGBQUAD* pal = img.getPalette();
	RGBQUAD *palEnt = NULL;

	//int SIGMA_CFG = OCRCfg().GetNum(CFG_SIGMA);
	//int CENTER_CFG = OCRCfg().GetNum(CFG_CENTER);

	double Sigma = iSIGMA/255.;
	double SigmaSqr = 2*128*128*Sigma*Sigma;

	short i = 256;
	short val;
	while(i--)
	{	
		val = (short)(255.*exp(-(i-iCENTER)*(i-iCENTER)/SigmaSqr));		
		palEnt = &pal[i];
		palEnt->rgbBlue  = (BYTE)(val  > 255 ? 255 : (val < 0 ? 0 : val));
		palEnt->rgbGreen = palEnt->rgbBlue;
		palEnt->rgbRed   = palEnt->rgbBlue;
	}
}

void PassportRecognizer::SORT_SLICE(BYTE a[]) 
{
  if (a[0] > a[1]) SWAP_GRAY(a[0], a[1]); 
  if (a[1] > a[2]) SWAP_GRAY(a[1], a[2]); 
  if (a[0] > a[1]) SWAP_GRAY(a[0], a[1]); 
}

void PassportRecognizer::MERGE_SLICE(BYTE a[], BYTE b[], BYTE bc[]) 
{
  int i, j, k;

  for (i = 0, j = 0, k = 0; i < 6; i++)
    if (j > 2)
      bc[i] = b[k++];
    else if (k > 2)
      bc[i] = a[j++];
    else 
      bc[i] = (a[j] < b[k]) ? a[j++] : b[k++];
}

BYTE PassportRecognizer::CALC_MEDIAN(BYTE x[], BYTE bc[]) 
{
  int i, j, k, m;

  for (i = 0, j = 0, k = 1; i < 4; i++)
    if (j > 2)
      m = bc[k++];
    else
      m = (x[j] < bc[k]) ? x[j++] : bc[k++];

  return m;
}


void PassportRecognizer::Median(fipWinImage &inImage,fipWinImage &outImage)
{
	int cols = inImage.getWidth(),rows = inImage.getHeight();
	int col,row;
   
	outImage.setSize(inImage.getImageType(),cols,rows,inImage.getBitsPerPixel());
										
// just copy border pixels to output
  for (col = 0; col < cols; col++) 
  {
	  outImage.getScanLine(0)[col] = inImage.getScanLine(0)[col];
	  outImage.getScanLine(rows-1)[col] = inImage.getScanLine(rows-1)[col];
  }

  for (row = 1; row < rows - 1; row++) 
  { 
	 outImage.getScanLine(row)[0] = inImage.getScanLine(row)[0];
	 outImage.getScanLine(row)[cols-1] = inImage.getScanLine(row)[cols-1];
  }

  BYTE _a[3], _b[3], _c[3], _d[3], bc[6];
  BYTE *a = _a, *b = _b, *c = _c, *d = _d,
		*scanL1,*scanL2,*scanL3,*oscanL;
  
  
	for (row = 1; row < rows - 1; row++) 
	{
	    scanL1 = inImage.getScanLine(row-1);
		scanL2 = inImage.getScanLine(row);
		scanL3 = inImage.getScanLine(row+1);
	    
	    a[0] = scanL1[0];
	    a[1] = scanL2[0];
	    a[2] = scanL3[0];
	    SORT_SLICE(a);

	    b[0] = scanL1[1];
	    b[1] = scanL2[1];
	    b[2] = scanL3[1];
		
	    SORT_SLICE(b);

	    for (col = 1; col < cols - 1; col += 2) 
		{

	      c[0] = scanL1[col + 1];
	      c[1] = scanL2[col + 1];
	      c[2] = scanL3[col + 1];

	      d[0] = scanL1[col + 2];
	      d[1] = scanL2[col + 2];
	      d[2] = scanL3[col + 2];

	      // sort slice c and d 
	      // slice a and b are already sorted

	      SORT_SLICE(c);
	      SORT_SLICE(d);

	      // merge slice b and c producing slice bc
	      MERGE_SLICE(b, c, bc);

		  oscanL = outImage.getScanLine(row);
	      // merge slice bc and a to calculate median 1
		  oscanL[col] = CALC_MEDIAN(a, bc);

	      // merge slice bc and d to calculate median 2
	      oscanL[col + 1] = CALC_MEDIAN(d, bc);

	      SWAP_SLICE(a, c);
	      SWAP_SLICE(b, d);
		}
	}
}
  
