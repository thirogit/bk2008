#if !defined(AFX_CODE39_H__6FE17747_EADF_4E89_9DCF_7688B04897BC__INCLUDED_)
#define AFX_CODE39_H__6FE17747_EADF_4E89_9DCF_7688B04897BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "1DBarcode.h"

class CCode39 : public C1DBarcode  
{
public:
	virtual void DrawBitmap(int x,int y,fipImage &image) const;
	CCode39();
	virtual ~CCode39();
private:
	virtual int	DrawPattern(const CString &csPattern,int x,int y,fipImage &image) const;
	CString	RetrievePattern( char c ) const;	

};

#endif 
