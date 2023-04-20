#include "stdafx.h"
#include "welcome.h"
#include "classes/BrowseForDir.h"
#include "../PassportStorage/PassImgBase.h"
#include "taskprogress/TaskProgressDlg.h"
#include "../logo/Logo.h"
#include "../VersionNo.h"
#include "MainCfg.h"
#include "../global_arrays.h"
#include "../CattleSQL.h"
#include "../tasktranslators/LoadTaskTranslator.h"
#include "../http_update/HttpVersionInfo.h"
#include "../classes/TempStringLoader.h"
//#include "../hkey/hkey.h"
#include "../exceptions/SQLException.h"

BEGIN_MESSAGE_MAP(CWelcomeDlg, CBDialog)
	//{{AFX_MSG_MAP(CWelcomeDlg)
		//ON_WM_DESTROY()
		ON_WM_TIMER()
		ON_WM_SHOWWINDOW()
		ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LOGIN, OnBnClickedLogin)
	ON_BN_CLICKED(IDC_LOGCANCEL, OnBnClickedLogcancel)
END_MESSAGE_MAP()

#define DELAY_LOGIN_TIMER_ID 1234
#define LOGIN_DELAY 300
#define WELCOME_SCREEN_FONT_COLOR RGB(255,255,0)

CWelcomeDlg::CWelcomeDlg() : CBDialog(CWelcomeDlg::IDD),bUsersLoaded(false)
{
	//{{AFX_DATA_INIT(CWelcomeDlg)
	//}}AFX_DATA_INIT
	SetBitmap(IDB_WELCOME);	
	SetBitmapStyle(StyleCenter);	
}

void CWelcomeDlg::OnOK()
{
	CWelcomeDlg::OnBnClickedLogin();
}

void CWelcomeDlg::OnShowWindow(BOOL bShow,UINT nStatus)
{
	if(AppCfg().GetBool(CFG_AUTOLOGIN))
			SetTimer(DELAY_LOGIN_TIMER_ID,LOGIN_DELAY,NULL);
	

}

BOOL CWelcomeDlg::OnInitDialog()
{
	CBDialog::OnInitDialog();

	m_Label_status.SetWindowText("");
	VERIFY(m_facelogo.LoadBitmap(IDB_FACE));

	CDC *pDC = GetDC();
	CLogoImage::GetLogo()->copyToBitmap(m_companyLogo,pDC);
	ReleaseDC(pDC);

	m_apptitle.LoadString(AFX_IDS_APP_TITLE);
	
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	logfont.lfCharSet = DEFAULT_CHARSET;
	strcpy(logfont.lfFaceName,"Lucida Console");
	logfont.lfHeight = -40;
	logfont.lfItalic = TRUE;
	logfont.lfWeight = FW_BOLD;

	m_titleFont.CreateFontIndirect(&logfont);

	logfont.lfHeight = -12;
	m_buildFont.CreateFontIndirect(&logfont);

	m_verString.Format(IDS_WELCOME_VER,VERSION_EXTEND,VERSION_DATE,VERSION_TIME);

	
	m_Label_user.SetTextColor(WELCOME_SCREEN_FONT_COLOR);
	m_Label_user.SetFont(&m_buildFont);
	m_Label_pass.SetTextColor(WELCOME_SCREEN_FONT_COLOR);
	m_Label_pass.SetFont(&m_buildFont);
	m_Label_status.SetTextColor(WELCOME_SCREEN_FONT_COLOR);
	m_Label_status.SetFont(&m_buildFont);
	return TRUE;
}

void CWelcomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CBDialog::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_USER,m_user);
	DDX_Text(pDX,IDC_PASSWORD,m_pass);
	DDX_Control(pDX,IDC_USERLABEL, m_Label_user);
	DDX_Control(pDX,IDC_PASSLABEL,m_Label_pass);
	DDX_Control(pDX,IDC_STATUSMSG,m_Label_status);
	
}

bool CWelcomeDlg::LoadUsers()
{
	LPCSTR szBaseDir = NULL;
	CString strBaseDir,browseCaption,errorMsg;
	CPassImgBase *pPassDB = CPassImgBase::GetInstance();
	browseCaption.LoadString(IDS_CHOOSEBASEDIR);

	if(AppCfg().GetBool(CFGID_ASK4BASEDIR))
	{
		if(CBrowseForDir::BrowseForDir(strBaseDir,(LPCSTR)browseCaption,this))
		{
			szBaseDir = (LPCSTR)strBaseDir;
		}
	}

	do
	{
		if(!SQLDB.OpenCattleBase(szBaseDir))
		{		
			SetDlgItemText(IDC_STATUSMSG,(LPCSTR)SQLDB.GetLastError());
			return false;
		}
		try
		{
			if(SQLDB.RequiresUpdate())
			{
				if(AfxMessageBox(IDS_ASKDBUPDATE,MB_YESNO,0) == IDYES)
				{
					if(!SQLDB.UpdateToRequired())
					{
						SQLDB.CloseCattleBase();
						SetDlgItemText(IDC_STATUSMSG,(LPCSTR)CTempStringLoader(IDS_FAILEDTOUPDATEDB));
						return false;
					}
					break;
				}
				else
				{
					SQLDB.CloseCattleBase();
					if(CBrowseForDir::BrowseForDir(strBaseDir,(LPCSTR)browseCaption,this))
					{
						szBaseDir = (LPCSTR)strBaseDir;
						continue;
					}
					else
					{
						SetDlgItemText(IDC_STATUSMSG,(LPCSTR)CTempStringLoader(IDS_FAILEDTOLOADDB));
						return false;
					}
				}
			}
			else
			{
				break;
			}
		}
		catch(CSQLException* e)
		{
			SetDlgItemText(IDC_STATUSMSG,(LPCSTR)e->GetError());
			e->Delete();
			return false;
		}
	}
	while(true);


	if(!pPassDB->OpenPassImgBase(szBaseDir))
	{
		if(AfxMessageBox(IDS_CANTOPENPASSDB,MB_YESNO,0) == IDYES)
		{
			if(pPassDB->OpenPassImgBase(szBaseDir,true))
			{
				AfxMessageBox(IDS_CANTCREATEPASSDB,MB_OK,0);
				return false;
			}

		}
		else
			return false;
	}

	
	if(!bUsersLoaded)
	{
		if(!SQLDB.LoadUsers(ary_users))
		{
			SetDlgItemText(IDC_STATUSMSG,(LPCSTR)SQLDB.GetLastError());
			return false;
		}
		bUsersLoaded = true;
	}
	return true;

}

bool CWelcomeDlg::Login(UserData *pUser,CPassword &password)
{
	CString errmsg;

	if(!bUsersLoaded) return false;

	if(!pUser)
	{
			errmsg.LoadString(IDS_ERRWRONGUSRNAME);
			m_Label_status.SetWindowText(errmsg);
			return false;
	}
				
	SetDlgItemText(IDC_USER,pUser->username);

	if(pUser->pass != password)
	{
		errmsg.LoadString(IDS_ERRWRONGPASS);
		m_Label_status.SetWindowText(errmsg);
		return false;
	}
	UserData::SetCurrentUser(pUser);
	pUser->UpdateLastLogin();
							
	errmsg.LoadString(IDS_LOADING);
	m_Label_status.SetWindowText(errmsg);

	EnableControls(FALSE);


	LoadTaskTranslator loadTaskTrans;
	CTaskProgressDialog TaskDlg(CTempStringLoader(IDS_LOADING),&loadTaskTrans);	
	
	TaskDlg.Create(this);

	if(SQLDB.LoadDB(true,&TaskDlg))	
	{
		return true;
	}
	else
	{
		if(!TaskDlg.CheckCancelButton())
			m_Label_status.SetWindowText((LPCSTR)SQLDB.GetLastError());
		else
		{
			errmsg.LoadString(IDS_LOADSTOP);
			m_Label_status.SetWindowText((LPCSTR)SQLDB.GetLastError());

		}

		ary_reasons.FreeAndFlush();
		ary_countries.FreeAndFlush();
		ary_stocks.FreeAndFlush();
		ary_rules.FreeAndFlush();
		ary_htypes.FreeAndFlush();
		ary_hents.FreeAndFlush();
		ary_indocs.FreeAndFlush();
		ary_outdocs.FreeAndFlush();
		ary_cows.FreeAndFlush();
		ary_sell_invs.FreeAndFlush();
		ary_buy_invs.FreeAndFlush();
		ary_rr_invs.FreeAndFlush();
		ary_expenses.FreeAndFlush();
		VERIFY(ary_expensecategories.RemoveAll());
		


		EnableControls(TRUE);

	}
				
	return false;

}

void CWelcomeDlg::EnableControls(BOOL bEnable)
{

	UINT controls[] = {IDC_USER,IDC_PASSWORD,IDC_LOGIN,IDC_LOGCANCEL};
	for(int i =0;i<sizeof(controls)/sizeof(UINT);i++)
		GetDlgItem(controls[i])->EnableWindow(bEnable);



}

void CWelcomeDlg::OnBnClickedLogin()
{
	UpdateData();
	
	if(m_user.IsEmpty())
	{
		
		CString errmsg;
		errmsg.LoadString(IDS_ERREMPTYUSERNAME);
		m_Label_status.SetWindowText(errmsg);
		return;
	}

	if(LoadUsers())
	{
		UserData *cuser = ary_users.GetPtrByUserName(m_user);		
		CPassword password(m_pass);

		if(Login(cuser,password)) EndDialog(IDOK);
	}
}

void CWelcomeDlg::OnBnClickedLogcancel()
{
	if(bUsersLoaded)
		ary_users.FreeAndFlush();

	EndDialog(IDCANCEL);
}

void CWelcomeDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == DELAY_LOGIN_TIMER_ID)
	{
		KillTimer(DELAY_LOGIN_TIMER_ID);
		if(LoadUsers())
		{
				CPassword password(AppCfg().GetString(CFG_AUTOPASS));
				UserData *pUser = ary_users.GetPtrByUserId(AppCfg().GetNum(CFG_AUTOUSERID));
				if(Login(pUser,password))EndModalLoop(IDOK);				
			
		}
		
		
		
	}
}


void CWelcomeDlg::PutBmp(CDC* pDC,int x,int y,CBitmap &bmp)
{
	CDC cDC;
	cDC.CreateCompatibleDC(pDC);
	int bmw, bmh ;
	BITMAP bmap;
	bmp.GetBitmap(&bmap);
	bmw = bmap.bmWidth;
	bmh = bmap.bmHeight;
	cDC.SelectObject(&bmp);
	pDC->BitBlt(x,y,bmw,bmh,&cDC,0,0,SRCCOPY);


}
void CWelcomeDlg::OnPaint()
{
	CBDialog::OnPaint();
	CClientDC dc(this);
	
	BITMAP bmap;
	CRect thisRect;
	
	GetClientRect(&thisRect);
	m_facelogo.GetBitmap(&bmap);
	
	PutBmp(&dc,thisRect.right-bmap.bmWidth-1,thisRect.bottom-bmap.bmHeight-1,m_facelogo);
	PutBmp(&dc,1,1,m_companyLogo);

	

	thisRect.top += 200;
	CFont *oldFont;
	
	oldFont = dc.SelectObject(&m_titleFont);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(WELCOME_SCREEN_FONT_COLOR);

	
	dc.DrawText(m_apptitle,thisRect,DT_CENTER);
	
	dc.SelectObject(&m_buildFont);

	
	CSize verStringTextSize = dc.GetTextExtent(m_verString);
	thisRect.top = thisRect.bottom-verStringTextSize.cy-2;
	dc.DrawText(m_verString,thisRect,DT_LEFT);

	dc.SelectObject(oldFont);


}
/*
void CWelcomeDlg::BelndText(CDC& dc,const CString& sText,int x,int y)
{
	CDC dcText;
	CSize textSize = dc.GetTextExtent(sText);

	dcText.CreateCompatibleDC( dc );	
		
	CBitmap bmpText;
	bmpText.CreateCompatibleBitmap( dc,textSize.cx, textSize.cy ); 
	dcText.SelectBitmap( bmpText );
	dcText.DrawText(sText,CRect(0,0,textSize.cx,textSize.cy),DT_LEFT);
}*/


void CWelcomeDlg::OnCancel()
{
	if(bUsersLoaded)
		ary_users.FreeAndFlush();

}