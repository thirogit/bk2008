#include "stdafx.h"
#include "RepoToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_TOOLBAR_BTNS 50

#define ID_TOOLBAR_FIRSTBTN 3001
#define ID_TOOLBAR_LASTBTN  (ID_TOOLBAR_FIRSTBTN+MAX_TOOLBAR_BTNS-1)

ToolbarButton::ToolbarButton(boost::function<void()> &onClick,HBITMAP hBtnImg,bool bDeleteImage) : m_hBtnImage(hBtnImg),m_bDeleteImage(bDeleteImage)
{
	m_onClick = onClick;
}

ToolbarButton::~ToolbarButton()
{
	if(m_bDeleteImage && m_hBtnImage!= NULL)
		::DeleteObject(m_hBtnImage);
}

void ToolbarButton::OnClick()
{
	m_onClick();
}

HBITMAP ToolbarButton::GetBtnImage() const
{
	return m_hBtnImage;
}

bool ToolbarButton::GetDeleteImage() const
{
	return m_bDeleteImage;
}


//#######################################################################################

BEGIN_MESSAGE_MAP( CRepoToolBar, CStatic )
	ON_COMMAND_RANGE(ID_TOOLBAR_FIRSTBTN,ID_TOOLBAR_LASTBTN,OnBtnClick)
	ON_WM_CREATE()
	//ON_WM_PAINT()
END_MESSAGE_MAP()

CRepoToolBar::CRepoToolBar() : m_bDynCreate(FALSE)
{
}

void CRepoToolBar::AddButton(boost::function<void()> &onClick,HBITMAP hBtnImg,bool bDeleteImage/* = true*/)
{
	AddButtonNoReToolBar(onClick,hBtnImg,bDeleteImage);
	ReButtonToolBar();	
}

void CRepoToolBar::AddButtonNoReToolBar(boost::function<void()> &onClick,HBITMAP hBtnImg,bool bDeleteImage)
{
	if(m_Buttons.GetCount() < MAX_TOOLBAR_BTNS)
	{
		m_Buttons.Add(new ToolbarButton(onClick,hBtnImg,bDeleteImage));		
	}
}

void CRepoToolBar::RemoveAllButtons()
{
	m_Buttons.FreeAndFlush();
	ReButtonToolBar();
}

void CRepoToolBar::RemoveAt(int iBtnIndex)
{
	ToolbarButton* pBtn = m_Buttons.GetAt(iBtnIndex);
	m_Buttons.RemoveAt(iBtnIndex);
	delete pBtn;
	ReButtonToolBar();
}

int CRepoToolBar::GetButtonCount()
{
	return m_Buttons.GetCount();
}

ToolbarButton* CRepoToolBar::GetButtonAt(int iBtnIndex)
{
	return  m_Buttons.GetAt(iBtnIndex);
}

BOOL CRepoToolBar::Create(CWnd* pParentWnd,const RECT& rect,UINT nID)
{
	m_bDynCreate = TRUE;
	return CStatic::Create("",SS_CENTERIMAGE,rect,pParentWnd,nID);
}

int CRepoToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateToolBar();

	return 0;
}

void CRepoToolBar::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	if(!m_bDynCreate)
	{
		CreateToolBar();
	}
}

void CRepoToolBar::ReButtonToolBar()
{
	while(m_btnsImgList.GetImageCount() > 0)
		m_btnsImgList.Remove(0);

	int btnCount = m_ToolBar.GetCount();
	while(btnCount > 0)
	{
		m_ToolBar.SendMessage(TB_DELETEBUTTON, 0, 0);
		btnCount--;
	}
	
	btnCount = m_Buttons.GetCount();
	UINT* buttonIds = new UINT[btnCount];
	CBitmap btnImage;
	for(int i = 0;i < btnCount;i++)
	{
		buttonIds[i] = ID_TOOLBAR_FIRSTBTN+i;
		btnImage.Attach(m_Buttons.GetAt(i)->GetBtnImage());
		m_btnsImgList.Add(&btnImage, (CBitmap*)NULL);
		btnImage.Detach();
	}

	if(btnCount > 0)
	{
		m_ToolBar.SetButtons(buttonIds,btnCount);

		for(int i = 0;i < btnCount;i++)
		{
			m_ToolBar.SetButtonInfo(i,buttonIds[i],TBBS_BUTTON,i);
		}

		CSize toolBarSize = m_ToolBar.CalcFixedLayout( FALSE, TRUE );
		m_ToolBar.SetWindowPos( NULL, 0, 0, toolBarSize.cx, toolBarSize.cy, 0 );
		m_ToolBar.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ToolBar.ShowWindow(SW_HIDE);
	}
	delete[] buttonIds;

}

BOOL CRepoToolBar::CreateToolBar()
{
	BOOL bRC = TRUE;


	DWORD uStyleFlags = GetStyle();
	BOOL bRightAligned =  (uStyleFlags & SS_RIGHT) != 0;
	BOOL bLeftAligned  =  (uStyleFlags & SS_LEFT) != 0;

	
	DWORD dwToolbarStyle = CBRS_ALIGN_TOP;

	if(bRightAligned)
		dwToolbarStyle = dwToolbarStyle | CBRS_ALIGN_RIGHT;

	if(bLeftAligned)
		dwToolbarStyle = dwToolbarStyle | CBRS_ALIGN_LEFT;

	bRC = m_ToolBar.Create( this, dwToolbarStyle );
	SetWindowText("");
	
	if (bRC == 0)
    {
		AfxThrowResourceException();		
    }

	CSize buttonSize(39,39);
	CSize imageSize(32,32);
	m_ToolBar.SetSizes(buttonSize,imageSize);

	m_btnsImgList.DeleteImageList();
	VERIFY(m_btnsImgList.Create(imageSize.cx, imageSize.cy, ILC_COLOR24  , 8, 8));
	m_ToolBar.GetToolBarCtrl().SetImageList(&m_btnsImgList);
	
	return bRC;
}


void CRepoToolBar::EnableButton(int iBtnIndex,BOOL bEnable)
{
	m_ToolBar.SendMessage(TB_ENABLEBUTTON, iBtnIndex+ID_TOOLBAR_FIRSTBTN, bEnable);
}

CRepoToolBar::~CRepoToolBar()
{
	m_Buttons.FreeAndFlush();
}

void CRepoToolBar::OnBtnClick(UINT btnId)
{
	int btnIndex  = btnId-ID_TOOLBAR_FIRSTBTN;
	ToolbarButton* pBtn = m_Buttons.GetAt(btnIndex);
	pBtn->OnClick();
}


void CRepoToolBar::OnPaint()
{

}



