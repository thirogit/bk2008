#ifndef __LOGO_H__
#define __LOGO_H__

#include "../PassportCommon/fipWinImageEx.h"
#include <afxole.h>

#define MAX_LOGO_WIDTH 200
#define MAX_LOGO_HEIGHT 200

class CLogoImage : public fipWinImageEx
{
private:
	CLogoImage();
	static CLogoImage m_logo;
	bool m_bLoaded;
	void LoadLogo();

	FreeImageIO m_fiIo;

	

public:
	static CLogoImage* GetLogo();
	bool GetOleLogoImage(COleStreamFile &logoStream);

	
	
};


#endif 
