#ifndef  __FILEDELETER_H__
#define __FILEDELETER_H__

class CFileDeleter 
{
public:
	CFileDeleter();
	void AddFile(const CString &filePath);
	~CFileDeleter();
	void Release(bool bRelease = true);
private:
	CArray<CString> m_files;
	bool m_bRelease;

};



#endif