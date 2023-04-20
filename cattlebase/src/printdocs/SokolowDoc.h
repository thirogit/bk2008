#ifndef __SOKOLOWDOC_H__
#define __SOKOLOWDOC_H__

#include "../printing/CowDrwDoc.h"
#include "../barcodeimage/BarcodeImage.h"
#include "../barcodeimage/Code39.h"
#include "../DataTypes.h"

#define CODE39_NARROW_BAR (0.012)
#define CODE39_BARCODE_RATIO (4.0)
#define CODE39_BARCODE_DPI (96)
#define CODE39_BARCODE_HEIGHT_INCH (0.6)

class CSokolowDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(CSokolowDoc)
public:
	CSokolowDoc();
	
	int GetPrintPages() const;
	void PrintPage(int page);
	int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
private:
	CCowArray	m_cows;
	CCode39			m_c39Barcode;
	CBarcodeImage	m_barcodeImage;
	COleStreamFile	m_logoStream;
	COleStreamFile	m_barcodeStream;
};

#endif
