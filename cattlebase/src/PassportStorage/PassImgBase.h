#ifndef _PASSIMGBASE_H_
#define _PASSIMGBASE_H_


#include "global_arrays.h"
#include "DataTypes.h"
#include "../PassportCommon/fipWinImageEx.h"
#include "SSFile.h"

#define MAX_MOUNTED_FILES 5

class CFileMountedPath
{
public:
	CFileMountedPath(const CString& filePath,bool bDoMount) : 
	  m_filePath(filePath),m_bDoMount(bDoMount),m_bMounted(false) {};
	CFileMountedPath(const CFileMountedPath &copy)
	{
		m_filePath = copy.m_filePath;
		m_bMounted = copy.m_bMounted;
		m_bDoMount = copy.m_bDoMount;
	};

	CString m_filePath;
	bool m_bMounted;
	bool m_bDoMount;
};

typedef PtrFlushArray<CFileMountedPath> CMountedFilesArray;

class CPassImgBase
{
protected:
	CPassImgBase();
	~CPassImgBase();
	static CPassImgBase m_instance;
public:
	static CPassImgBase* GetInstance();

	bool IsOpen();
	bool OpenPassImgBase(LPCSTR szBaseDir = NULL,bool bCreate = false);
	bool GetPassImageOle(COleStreamFile &inStream,Cow *pCow);
	bool GetPassImage(fipWinImageEx &outImage,Cow *pCow);
	bool StorePassImage(Cow *pCow, const char* szPath,bool bDelete);
	//bool StorePassImage(Cow *pCow,const fipWinImageEx &srcImage);
	void ClosePassImgBase();
	bool DeletePassImg(Cow *pCow);
	bool DoBackup(const CString &backupPath);
	
	void PurgeDB();
	bool RestorePassFile(const CString &srcDir);
	ULONGLONG GetPassDbFileSize();
	CString GetPrimaryPassDBFilePath();
	bool GetLoadedPassDBFilePath(CString &outPath);

	void GetMountList(CMountedFilesArray &mountList);
	void LoadMounts();
	bool Rename(const CString &oldname,const CString &newname);
protected:
	CSSFile m_SSFile;
	
	CSSFile *m_mntStrgs[MAX_MOUNTED_FILES];
	CMountedFilesArray m_mounts;

	void CloseMounted();
	bool OpenStream(const CString & name, COleStreamFile & sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);
public:
	bool HasImage(const CString &label);
	CString GeneratePassImgLabel();

	FreeImageIO m_fiIo;

	



};


#endif