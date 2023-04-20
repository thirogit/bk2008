#include "stdafx.h"
#include "DYMOBaseDoc.h"
#include "../Utils/StringFormat.h"
#include "../missing/paths.h"
#include "../missing/util.h"
#include "../exceptions/StrException.h"
#include "../classes/TempStringLoader.h"
#include "../datatypes/Hent.h"

CDYMOBaseDoc::CDYMOBaseDoc()
{
	m_LE.CreateDispatch("Dymo.LabelEngine");
}

CDYMOBaseDoc::~CDYMOBaseDoc()
{
	m_LE.ReleaseDispatch();
}

void CDYMOBaseDoc::CreateCowDocument(const PRINTDOCS* pDoc)
{
	CString hentLabelTemplatePath = paths_ConcatPath(CUtil::GetAppDir(),CStringFormat("\\lwl\\%s.LWL",pDoc->szDocTemplateName));
	if (!m_LE.OpenFile(hentLabelTemplatePath))
		throw new CStrException(CStringFormat(CTempStringLoader(IDS_DOCTEMPLATEMISSING),(LPCSTR)hentLabelTemplatePath));
	m_LE.SetShadow(FALSE);
}

CSize CDYMOBaseDoc::GetPixelPageSize()
{
	ILblInfo LI =  (ILblInfo)m_LE.LabelInfo();

	return CSize(LI.GetBitmapWidth(),LI.GetBitmapHeight());
}

CSize CDYMOBaseDoc::GetTWIPSPageSize()
{
	ILblInfo LI =  (ILblInfo)m_LE.LabelInfo();
	return CSize(LI.GetPaperWidth(),LI.GetPaperHeight());
}

CSize CDYMOBaseDoc::GetPaperSize() const
{
	CSize paperSizeTWIPS = const_cast<CDYMOBaseDoc*>(this)->GetTWIPSPageSize();

	int cx = MulDiv(paperSizeTWIPS.cx,254,1440);
	int cy = MulDiv(paperSizeTWIPS.cy,254,1440);

	return CSize(cx,cy);
}

void CDYMOBaseDoc::DrawText(const CString& sFieldName,const CString& sValue)
{
	IPrintObject PO = (IPrintObject)m_LE.PrintObject();
	long objIndex = PO.FindObj(sFieldName);
	if(objIndex != 0)
	{
		ILabelObject ILO = (ILabelObject)PO.LabelObject(objIndex);
		long objType = ILO.GetObjType();
		if(objType == 0)
		{
			ITextObj ITO;
			ITO.AttachDispatch(ILO.m_lpDispatch, false);
			((ITextAttributes)ITO.TextAttributes()).SetText(sValue);
		}
	}
}
void CDYMOBaseDoc::DrawBarcode(const CString& sBarCodeFieldName,const CString& sValue)
{
	IPrintObject PO = (IPrintObject)m_LE.PrintObject();
	long objIndex = PO.FindObj(sBarCodeFieldName);
	if(objIndex != 0)
	{
		ILabelObject ILO = (ILabelObject)PO.LabelObject(objIndex);
		long objType = ILO.GetObjType();
		if(objType == 5)
		{
			IBarCodeObj IBCO;
			IBCO.AttachDispatch(ILO.m_lpDispatch, false);
			IBCO.SetText(sValue);
		}
	}
}

void CDYMOBaseDoc::Draw(CDC* pDC)
{
	m_LE.DrawLabel((long) pDC->GetSafeHdc());
}


void CDYMOBaseDoc::Print(const CString& sDeviceName,const CString& sPort,UINT nNumberOfCopies)
{
	m_LE.PrintLabel(sDeviceName,"COM1:",nNumberOfCopies,TRUE);
}