#include "stdafx.h"
#include "PassImgBase.h"
#include "../missing/paths.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "MainCfg.h"
#include "../missing/util.h"
#include "../log.h"
#include "../datalimits.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CPassImgBase CPassImgBase::m_instance;

CPassImgBase::CPassImgBase() 
{
	m_fiIo.read_proc = (FI_ReadProc)fipWinImageEx::FI_ReadProcNFS;
    m_fiIo.write_proc = (FI_WriteProc)fipWinImageEx::FI_WriteProcNFS;
    m_fiIo.seek_proc = (FI_SeekProc)fipWinImageEx::FI_SeekProcNFS;   
    m_fiIo.tell_proc = (FI_TellProc)fipWinImageEx::FI_TellProcNFS;

	
	
}

void CPassImgBase::GetMountList(CMountedFilesArray &mountList)
{
	mountList.FreeAndFlush();
	for(int i = 0;i < m_mounts.GetSize();i++)
		mountList.Add(new CFileMountedPath(*m_mounts[i]));
}

void CPassImgBase::LoadMounts()
{
	int i,j;
	m_mounts.FreeAndFlush();

	UINT mountedFilesCfgItems[MAX_MOUNTED_FILES] =
	{
		CFG_MOUNT1_ITEM,
		CFG_MOUNT2_ITEM,
		CFG_MOUNT3_ITEM,
		CFG_MOUNT4_ITEM,
		CFG_MOUNT5_ITEM
	};

	for(i = 0;i < MAX_MOUNTED_FILES;i++)
	{
		m_mounts.Add(new CFileMountedPath(AppCfg().GetString(mountedFilesCfgItems[i],0),
			AppCfg().GetBool(mountedFilesCfgItems[i],1) == TRUE));		
	}

	CloseMounted();

	CFileMountedPath *mountFile;
	for(i = 0,j = 0;i < MAX_MOUNTED_FILES;i++)
	{
		mountFile = m_mounts[i];
		if(mountFile->m_bDoMount)
		{
			if(!m_mntStrgs[j]) m_mntStrgs[j] = new CSSFile();
			if(m_mntStrgs[j]->OpenSSFile(mountFile->m_filePath))
			{
				mountFile->m_bMounted = true;
				j++;
			}
		}	

	}


}
void CPassImgBase::CloseMounted()
{
	for(int i = 0;i < MAX_MOUNTED_FILES;i++)
	{
		if(m_mntStrgs[i])
		{
			if(m_mntStrgs[i]->IsOpen())
				m_mntStrgs[i]->Close();

			delete m_mntStrgs[i];

			m_mntStrgs[i] = NULL;
		}
	}
	
}


CPassImgBase::~CPassImgBase()
{
	if(m_SSFile.IsOpen())
		m_SSFile.Close();
	CloseMounted();
	m_mounts.FreeAndFlush();
}

CPassImgBase* CPassImgBase::GetInstance()
{
	return &m_instance;
}

bool CPassImgBase::OpenPassImgBase(LPCSTR szBaseDir/* = NULL*/,bool bCreate/* = false*/)
{
	
	if(m_SSFile.IsOpen())
		m_SSFile.Close();

	CString fsPath,storageFile;
	storageFile.LoadString(IDS_PASSSTORAGEFILE);

	LoadMounts();
	
	if(!szBaseDir)
		fsPath = paths_ConcatPath(CUtil::GetAppDir(),storageFile);
	else
	{
		fsPath = paths_ConcatPath(CString(szBaseDir),storageFile);
	}


	LOG("Loading PASSIMGDB from %s",(LPCSTR)fsPath);

	if (!m_SSFile.OpenSSFile(fsPath))
	{
		if(bCreate)
			if(m_SSFile.CreateSSFile(fsPath))
				return true;
		return false;
	}


	return true;
}

bool CPassImgBase::OpenStream(const CString & name, COleStreamFile & sf, DWORD mode/*  = CFile::modeReadWrite | CFile::shareExclusive*/)
{
	if(!m_SSFile.IsOpen()) return false;

	if (!m_SSFile.OpenStream(name, sf))
	{		
		for(int i = 0; i < MAX_MOUNTED_FILES;i++)
		{
			if(m_mntStrgs[i])
				if(m_mntStrgs[i]->OpenStream(name,sf,mode)) return true;
		}
		return false;
	}

	return true;
}



bool CPassImgBase::GetPassImageOle(COleStreamFile &inStream,Cow *pCow)
{
	COleStreamFile in;
	BYTE pImgBuff[1024];
	UINT bytesRead;
	HRESULT hr;

	ASSERT(!inStream.m_lpStream);
	
	if(!pCow->HasPassImg()) return false;

	if (!OpenStream(pCow->GetPassImgLabel(), in))
		return false;	
	
	inStream.CreateMemoryStream(NULL);
	
	LONG lSize = in.GetLength();
	CComVariant varSize(lSize);
	hr = varSize.WriteToStream(inStream.GetStream());
	ATLASSERT(SUCCEEDED(hr));
	
	while(bytesRead = in.Read(pImgBuff,sizeof(pImgBuff)))
	{
		inStream.Write(pImgBuff,bytesRead);		
	}			
	inStream.SeekToBegin();
	in.Close();
	return true;	
}


bool CPassImgBase::GetPassImage(fipWinImageEx &outImage,Cow *pCow)
{
	BOOL bRet;
	COleStreamFile in;

	if(!pCow->HasPassImg()) return false;
	if (!OpenStream(pCow->GetPassImgLabel(), in))	return false;	

	bRet = outImage.loadFromHandle(&m_fiIo,(fi_handle)&in);	
	in.Close();
	return bRet == TRUE;
}

bool CPassImgBase::HasImage(const CString &label)
{
	COleStreamFile in;		
	if (!OpenStream(label, in)) return false;
	in.Close();
	return true;
	
}

bool CPassImgBase::StorePassImage(Cow *pCow, const char* szPath,bool bDelete)
{
	size_t nBytes;
	BYTE Buffer[4096];
	COleStreamFile out;
	CString newLabel;
	
	do
	{
		newLabel = GeneratePassImgLabel();
	}
	while(HasImage(newLabel));


	if (!m_SSFile.CreateStream(newLabel, out))
		return false;

	CFile imgFile;

	if(!imgFile.Open(szPath,CFile::modeRead))
	{
		return false;
		out.Close();
	}

	if(pCow->HasPassImg())
		DeletePassImg(pCow);
	
	do
	{
		nBytes = imgFile.Read(Buffer,sizeof(Buffer));
        out.Write(Buffer,nBytes);		
	}
	while(nBytes > 0);

	imgFile.Close();

	if(bDelete) DeleteFile(szPath);

	out.Close();

	pCow->ChangePassImgLabel(newLabel);
	
	return true;

}
/*
bool CPassImgBase::StorePassImage(Cow *pCow,const fipWinImageEx &srcImage)
{
	COleStreamFile out;
	if (!m_SSFile.CreateStream(szEAN, out))
		return false;

	srcImage.saveToHandle(FIF_JPEG,&m_fiIo,(fi_handle)&out);
	out.Close();

	return true;
}
*/
void CPassImgBase::ClosePassImgBase()
{
	m_SSFile.Close();
}

ULONGLONG CPassImgBase::GetPassDbFileSize()
{
	CString StrgFilePath;	
	StrgFilePath = GetPrimaryPassDBFilePath();
	return (ULONGLONG)boost::filesystem::file_size(boost::filesystem::path((LPCSTR)StrgFilePath));
}

bool CPassImgBase::GetLoadedPassDBFilePath(CString &outPath)
{
	if(m_SSFile.IsOpen())
	{
		outPath = m_SSFile.GetFilename();
		return true;
	}
	return false;
}

bool CPassImgBase::DoBackup(const CString &backupPath)
{
	bool bRet = FALSE;

	CString dbFile,newFilePath,StrgFilePath;	
	dbFile.LoadString(IDS_PASSSTORAGEFILE);

	newFilePath = paths_ConcatPath(backupPath,dbFile);
	StrgFilePath = paths_ConcatPath(CUtil::GetAppDir(),dbFile);
	
	m_SSFile.Close(); //flush file

	bRet = CopyFile(StrgFilePath,newFilePath,FALSE) == TRUE;
	
	bRet &= m_SSFile.OpenSSFile(StrgFilePath);

	return bRet;
}
bool CPassImgBase::DeletePassImg(Cow *pCow)
{
	if(pCow->HasPassImg())
		return m_SSFile.DestroyElement(pCow->GetPassImgLabel());
	return false;
}

void CPassImgBase::PurgeDB()
{
	IEnumSTATSTG *estg;
	IStorage *stg = m_SSFile.GetCurrentStorage();

	if (stg)
	{
		if (stg->EnumElements(0, NULL, 0, &estg) != S_OK)
			return;

		STATSTG stgstruct;
		ULONG fetched;
		while (estg->Next(1, &stgstruct, &fetched) == S_OK)
		{			
			stg->DestroyElement(stgstruct.pwcsName);			
		}
		estg->Release();
	}

}

bool CPassImgBase::RestorePassFile(const CString &srcDir)
{
	bool bRet = FALSE;

	CString dbFile,restoreFilePath,StrgFilePath;	
	dbFile.LoadString(IDS_PASSSTORAGEFILE);

	restoreFilePath = paths_ConcatPath(srcDir,dbFile);
	StrgFilePath = paths_ConcatPath(CUtil::GetAppDir(),dbFile);

	m_SSFile.Close(); //flush file

	bRet = CopyFile(restoreFilePath,StrgFilePath,FALSE) == TRUE;

	bRet &= m_SSFile.OpenSSFile(StrgFilePath);

	return bRet == TRUE;

}

CString CPassImgBase::GetPrimaryPassDBFilePath()
{
	CString dbFile;
	dbFile.LoadString(IDS_PASSSTORAGEFILE);
	return paths_ConcatPath(CUtil::GetAppDir(),dbFile);	
}

bool CPassImgBase::Rename(const CString &oldname,const CString &newname)
{
	IStorage *stg = m_SSFile.GetCurrentStorage();
	OLECHAR oldNameWide[MAX_PATH+1],newNameWide[MAX_PATH+1];

	MultiByteToWideChar(CP_ACP,0,(LPCSTR)oldname,oldname.GetLength()+1,oldNameWide,MAX_PATH);
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)newname,newname.GetLength()+1,newNameWide,MAX_PATH);
	

	if (stg)
	{
		return SUCCEEDED(stg->RenameElement(oldNameWide,newNameWide));
	}
	return false;
}

#define GetRandomIndex(max) (rand() % (int)(max))

CString CPassImgBase::GeneratePassImgLabel()
{
	char charset[] = "QA1Z2XS3W4ED56CVF7RTG68B87N193HY4U06JM9KIO0L";
	const int nMaxIndex = sizeof(charset)-1;

	// initialize random numbers generator
	srand((unsigned int)GetTickCount());

	CString strGeneratedLabel;
	for (int i=0; i<MAXPASSIMGLBL; i++)
	{
		strGeneratedLabel += charset[GetRandomIndex(nMaxIndex)];
	}
	return strGeneratedLabel;	
}

bool CPassImgBase::IsOpen()
{
	return m_SSFile.IsOpen();
}
