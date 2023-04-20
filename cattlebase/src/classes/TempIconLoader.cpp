#include "stdafx.h"
#include "TempIconLoader.h"




CTempIconLoader::CTempIconLoader(UINT uResourceID)
{	
	m_hIcon = AfxGetApp()->LoadIcon(uResourceID);
}

CTempIconLoader::~CTempIconLoader()
{
	if (m_hIcon)
		VERIFY( DestroyIcon(m_hIcon) );
}

CTempIconLoader::operator HICON()  const
{
	return this == NULL ? NULL : m_hIcon;
}


