#include "stdafx.h"
#include "FileDeleter.h"


CFileDeleter::CFileDeleter() : m_bRelease(false)
{
}
void CFileDeleter::AddFile(const CString &filePath)
{
	m_files.Add(filePath);
}
CFileDeleter::~CFileDeleter()
{
	if(!m_bRelease)
	{
		for(int i = 0,s = m_files.GetSize();i < s;i++)
			DeleteFile((LPCSTR)m_files[i]);
	}
}
void CFileDeleter::Release(bool bRelease/* = true*/)
{
	m_bRelease = bRelease;
}

