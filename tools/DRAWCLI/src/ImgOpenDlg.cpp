// ImgOpenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRAWCLI.h"
#include "ImgOpenDlg.h"
#include ".\imgopendlg.h"


// CImgOpenDlg dialog

IMPLEMENT_DYNAMIC(CImgOpenDlg, CBaseDlg)
CImgOpenDlg::CImgOpenDlg(CDrawImg *pImg)	: CBaseDlg(CImgOpenDlg::IDD),m_bNew(false)
{
	VERIFY(pImg);
	m_preview.Copy(pImg->m_picture);
}

CImgOpenDlg::~CImgOpenDlg()
{
	
}

void CImgOpenDlg::DoChanges(CDrawObj *pObj)
{
	CDrawImg *pImg = (CDrawImg*)pObj;
	if(m_bNew)	pImg->m_picture.Copy(m_preview);
}

void CImgOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImgOpenDlg, CBaseDlg)
	
	ON_BN_CLICKED(IDC_OPENIMG, OnOpenImg)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CImgOpenDlg message handlers

/*BOOL CImgOpenDlg::OnInitDialog()
{
	if(!CBaseDlg::OnInitDialog()) return FALSE;

	return TRUE;
}*/

void CImgOpenDlg::OnPaint()
{
	CBaseDlg::OnPaint();
	CClientDC dc(this);

	CRect rc,btnrc;
	GetClientRect(&rc);

	GetDlgItem(IDC_OPENIMG)->GetClientRect(&btnrc);
	rc.top += btnrc.Height()+20;
    
	m_preview.Draw(&dc,rc.TopLeft(),CSize(rc.Width(),rc.Height()));


}

void CImgOpenDlg::OnOpenImg()
{
	CFileDialog l_openimg(TRUE,NULL,m_ImgFile,OFN_OVERWRITEPROMPT,
		  "Windows Bitmap (*.bmp)|*.bmp|JPEG Compressed Image (*.jpg)|*.jpg|GIF (*.gif)|*.gif||");
	m_bNew = false;
	if(l_openimg.DoModal() == IDOK)
	{
		m_ImgFile = l_openimg.GetPathName();
		if(!m_preview.Load(m_ImgFile))
			AfxMessageBox("Faild to load image",MB_OK,0);
		else
		{
			m_bNew = true;
			Invalidate();
		}
	}
}
