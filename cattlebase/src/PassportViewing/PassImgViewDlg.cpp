#include "stdafx.h"
#include "PassImgViewDlg.h"
#include "..\PassportStorage\PassImgBase.h"
#include "..\classes\TempStringLoader.h"
// CPassImgViewDlg dialog

IMPLEMENT_DYNAMIC(CPassImgViewDlg, CDialog)

CPassImgViewDlg::CPassImgViewDlg(CCowArray &cowArray,int iIndex,CWnd* pParent/* = NULL*/) :
							m_cowArray(cowArray),m_iIndex(iIndex), CDialog(IDD,pParent)
{

}

CPassImgViewDlg::~CPassImgViewDlg()
{
}

void CPassImgViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PASSIMGVIEW,m_passView);
}


BEGIN_MESSAGE_MAP(CPassImgViewDlg, CDialog)
	ON_BN_CLICKED(IDC_PREV,OnPrev)
	ON_BN_CLICKED(IDC_NEXT,OnNext)
	ON_BN_CLICKED(IDC_PREVPREV,OnPrevPrev)
	ON_BN_CLICKED(IDC_NEXTNEXT,OnNextNext)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CPassImgViewDlg::PreTranslateMessage (MSG* pMsg) 
{
	bool bArrow = false;
    if (pMsg->message == WM_KEYDOWN)
    {
	    switch(pMsg->wParam)
		{
			case VK_LEFT:
				bArrow = true;
				OnPrev();
			break;
			case VK_RIGHT:
				bArrow = true;
				OnNext();
			break;
		}
		if(bArrow)
		{
				::TranslateMessage (pMsg);
				::DispatchMessage (pMsg);
				return TRUE;		    	
		}
    }
    return CDialog::PreTranslateMessage (pMsg);
}




void CPassImgViewDlg::OnNext()
{
	int s = m_cowArray.GetSize();
	bool bBounds = m_iIndex+1 < s;

	if(bBounds)
	{
		m_iIndex++;				
		UpdatePassImage();
		EnablePrev(TRUE);
	}
	EnableNext(bBounds);

}


void CPassImgViewDlg::OnPrev()
{
	bool bBounds = m_iIndex-1 >= 0;
	if(bBounds)
	{
		m_iIndex--;		
		UpdatePassImage();
		EnableNext(TRUE);
	}

	EnablePrev(bBounds);

	
}


void CPassImgViewDlg::OnNextNext()
{
	int size = m_cowArray.GetSize();
	while((m_iIndex+1) < size)
	{
		m_iIndex++;
		if(PassImgExists(m_cowArray[m_iIndex]) || m_iIndex == (size-1))
		{
			UpdatePassImage();
			break;
		}
	}
	EnableNext(m_iIndex+1 < size);
	EnablePrev(m_iIndex-1 >= 0);
}

void CPassImgViewDlg::OnPrevPrev()
{
	while((m_iIndex-1) >= 0)
	{
		m_iIndex--;
		if(PassImgExists(m_cowArray[m_iIndex]) || m_iIndex == 0)
		{
			UpdatePassImage();
			break;
		}
	}
	EnablePrev(m_iIndex-1 >= 0);
	EnableNext(m_iIndex+1 < m_cowArray.GetSize());
}

bool CPassImgViewDlg::PassImgExists(Cow *pCow)
{
	return pCow->HasPassImg();
}

void CPassImgViewDlg::UpdatePassImage()
{
	Cow *pCow = m_cowArray[m_iIndex];
	if(GetPassImg(pCow))
		m_passView.UpdateImage(m_passPix);
	else
	{
		CString noImageText;
		if(pCow->HasPassImg())
			noImageText = (CString)CTempStringLoader(IDS_PASSIMGNOTAVAIL);
		m_passView.NoImage(noImageText);
	}
	SetWindowText(m_cowArray[m_iIndex]->GetEAN());
}

bool CPassImgViewDlg::GetPassImg(Cow *pCow)
{
	return CPassImgBase::GetInstance()->GetPassImage(m_passPix,pCow);
}


void CPassImgViewDlg::EnableNext(BOOL bEnable)
{
	GetDlgItem(IDC_NEXT)->EnableWindow(bEnable);
	GetDlgItem(IDC_NEXTNEXT)->EnableWindow(bEnable);
}

void CPassImgViewDlg::EnablePrev(BOOL bEnable)
{
	GetDlgItem(IDC_PREV)->EnableWindow(bEnable);
	GetDlgItem(IDC_PREVPREV)->EnableWindow(bEnable);
}

BOOL CPassImgViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
			
	UpdatePassImage();

	EnableNext(m_iIndex < (m_cowArray.GetSize()-1));
	EnablePrev(m_iIndex > 0);
	
	return TRUE;
}
// CPassImgViewDlg message handlers
