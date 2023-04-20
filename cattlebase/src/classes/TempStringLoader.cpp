#include "stdafx.h"
#include "TempStringLoader.h"


CTempStringLoader::CTempStringLoader(UINT uResourceID)
{	
	m_String.LoadString(uResourceID);
}

CTempStringLoader::operator CString()  const
{
	return m_String;
}
CTempStringLoader::operator LPCSTR() const
{
	return (LPCSTR)m_String;
}

