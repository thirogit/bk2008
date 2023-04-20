#include "stdafx.h"
#include "KeyStateDlg.h"
#include "../../cattlebase.h"
#include "../../classes/types/DateTime.h"
#include "../../classes/TempStringLoader.h"
#include "../../Utils/StringFormat.h"

#define CHECK_STATE_INTERVAL_MS (5*1000)
#define CHECK_STATE_TIMER_ID 1

CKeyStateDlg::CKeyStateDlg(CWnd* pParent /*=NULL*/) : CDialog(CKeyStateDlg::IDD, pParent)
{
	
}


BEGIN_MESSAGE_MAP(CKeyStateDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CKeyStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Text(pDX,IDC_KEYSTATELABEL,m_sKeyState);		
}

BOOL CKeyStateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(CheckKeyState())
	{
		SetTimer(CHECK_STATE_TIMER_ID,CHECK_STATE_INTERVAL_MS,NULL);
	}
	else
	{
		EndDialog(IDOK);
	}

	return TRUE;
}

bool CKeyStateDlg::CheckKeyState()
{
	KeyManager* pKeyManager = GetApplicationKeyManager();
	KeyManager::KEY_STATE keyState = pKeyManager->GetKeyState();

	switch(keyState)
	{
		case KeyManager::VALID:
			EndDialog(IDOK);
			return false;

		case KeyManager::NOT_FOUND:

			m_sKeyState = (LPCSTR)CTempStringLoader(IDS_ERRNOHKEY);
			break;

		case KeyManager::EXPIRED:
		{
			CDateTime expiryDt;
			expiryDt.SetDateTime(pKeyManager->GetKeyExpiryDt());
			m_sKeyState = (LPCSTR)CStringFormat(CTempStringLoader(IDS_ERRHKEYEXPIREDFMT),expiryDt.GetDateInFmt(CDateTime::YYYYdotMMdotDD_HHcolonMMcolonSS));
			break;
		}
		case KeyManager::REQUIRES_UPDATE:

			m_sKeyState = (LPCSTR)CStringFormat(CTempStringLoader(IDS_ERRHKEYAPPREQUIREDUPDATEFMT),pKeyManager->GetKeyRequiredVersion());
			break;
		
		case KeyManager::ABSENT_TOO_LONG:

			m_sKeyState = (LPCSTR)CStringFormat(CTempStringLoader(IDS_ERRHKETABSENTTOOLONGFMT),pKeyManager->GetAbsentLimitSecs()/60);
			break;

		case KeyManager::INVALID_CLOCK:
		{
			CDateTime now = CDateTime::now();
			CDateTime keyClock;
			keyClock.SetDateTime(pKeyManager->GetKeyClock());
			m_sKeyState = (LPCSTR)CStringFormat(CTempStringLoader(IDS_ERRHKEYINVALIDCLOCK),
				now.GetDateInFmt(CDateTime::YYYYdotMMdotDD_HHcolonMMcolonSS),
				keyClock.GetDateInFmt(CDateTime::YYYYdotMMdotDD_HHcolonMMcolonSS));
			
			break;
		}

	}
	UpdateData(FALSE);
	return true;
}

void CKeyStateDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == CHECK_STATE_TIMER_ID)
	{
		KillTimer(CHECK_STATE_TIMER_ID);
		if(CheckKeyState())
		{
			SetTimer(CHECK_STATE_TIMER_ID,CHECK_STATE_INTERVAL_MS,NULL);
		}
	}
}