#ifndef __TEMPICONLOADER_H__
#define __TEMPICONLOADER_H__

//////////////////////////////////////////////////////////////////////////////
// CTempIconLoader

class CTempIconLoader
{
public:
	CTempIconLoader(UINT uResourceID);
	~CTempIconLoader();

	operator HICON() const;

protected:
	HICON m_hIcon;
};

#endif