#ifndef __DYMOBASEDOC_H__
#define __DYMOBASEDOC_H__

#include "../printing/DYMO/DYMO.h"
#include "../printing/CowBasePrintDoc.h"

class CDYMOBaseDoc : public CCowBasePrintDoc
{
public:
	CDYMOBaseDoc();
	virtual void CreateCowDocument(const PRINTDOCS* pDoc);
	virtual int LoadInputData(CPtrArray &docInputData) = 0;
	virtual ~CDYMOBaseDoc();
	CSize GetPixelPageSize();
	CSize GetTWIPSPageSize();
	virtual CSize GetPaperSize() const; //MM / 10
	virtual void SetFields(int nLabelPage) = 0;
	virtual void Draw(CDC* pDC);
	void Print(const CString& sDeviceName,const CString& sPort,UINT nNumberOfCopies); 
	virtual int GetPrintPages() const = 0;
protected:
	void DrawBarcode(const CString& sBarCodeFieldName,const CString& sValue);
	void DrawText(const CString& sFieldName,const CString& sValue);
protected:
	ILabelEngine m_LE;
	CHentArray m_hents;

};

#endif