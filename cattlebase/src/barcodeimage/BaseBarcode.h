#ifndef __BASEBARCODE_H__
#define __BASEBARCODE_H__


#include "FreeImagePlus.h"

class CBaseBarcode  
{
public:
	virtual void DrawBitmap(int x,int y,fipImage &image) const = 0;
	virtual long GetBarcodePixelWidth() const = 0;
	virtual long GetBarcodePixelHeight() const = 0;
};

#endif