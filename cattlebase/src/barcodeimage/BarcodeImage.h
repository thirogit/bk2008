#ifndef __BARCODEIMAGE_H__
#define __BARCODEIMAGE_H__

#include "../PassportCommon/fipWinImageEx.h"
#include <afxole.h>
#include "BaseBarcode.h"

class CBarcodeImage : public fipWinImageEx
{
public:
	CBarcodeImage();
	bool DrawBarcode(const CBaseBarcode& barcode);
	bool GetImageStream(COleStreamFile &logoStream);
private:
	FreeImageIO m_fiIo;		
};


#endif 
