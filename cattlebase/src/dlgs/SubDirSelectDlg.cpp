#include "stdafx.h"
#include "SubDirSelectDlg.h"
#include "MainCfg.h"

//#include <boost/filesystem/operations.hpp>




/////////////////////////////////////////////////////////////////////////////////////////

const GRIDHEADERS CSubDirList::m_SubDirListCols[] =
{
	{IDS_HEADERSUBDIR,1},
	{0,0}	
};
CSubDirList::CSubDirList(CStringArray &subdirList) : m_array(subdirList),CBaseListCtrl(m_SubDirListCols,CFGID_SUBDIRLIST)
{
}

void CSubDirList::UpdateItemCount()
{
	SetItemCount(m_array.GetSize());
}
void CSubDirList::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	strcpy(pszItemText,(LPCSTR)m_array[nItem]);
}
/////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSubDirSelect, CDialog)
CSubDirSelect::CSubDirSelect(const CString &directoryWild,CWnd* pParent /*=NULL*/)
	: CDialog(CSubDirSelect::IDD, pParent),m_subDirList(m_subDirectories)
{
	
	WIN32_FIND_DATA dirs;
	HANDLE handle = FindFirstFile((LPCSTR)directoryWild, &dirs);

	if(handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if( dirs.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Is a directory
				m_subDirectories.Add(dirs.cFileName);			
		} while(FindNextFile(handle, &dirs));
	}
}

CSubDirSelect::~CSubDirSelect()
{
}

void CSubDirSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST,m_subDirList);
}


BEGIN_MESSAGE_MAP(CSubDirSelect, CDialog)
END_MESSAGE_MAP()


BOOL CSubDirSelect::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_subDirList.UpdateItemCount();
	SetWindowText(m_caption);
	return TRUE;
}

void CSubDirSelect::SetCaption(UINT nCaption)
{
	m_caption.LoadString(nCaption);
}
void CSubDirSelect::SetCaption(const CString &sCaption)
{
	m_caption = sCaption;
}

void CSubDirSelect::OnOK()
{
	int iSel = m_subDirList.GetCurSel();
	if(iSel < 0)
	{
		AfxMessageBox(IDS_ERRCHOOSEDIRECTORY,MB_OK,0);
		return;
	}

	m_selectedSubDir = m_subDirectories[iSel];
	EndDialog(IDOK);
}
LPCSTR CSubDirSelect::GetSelectedSubDirPath()
{
	return (LPCSTR)m_selectedSubDir;
}