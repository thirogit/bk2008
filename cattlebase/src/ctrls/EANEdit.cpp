#include "stdafx.h"
#include "EANEdit.h"
#include "enteredit\EnterEdit.h"
#include "../datalimits.h"
#include "../consts.h"
#include "../missing/util.h"
#include "../MainCfg.h"
#include "../classes/validators/CowEanValidator.h"

#define MASK_CHARACTER 'A'
#define IGNORE_EANCHKSUM_CMD (-2)
#define FLAGIMG_MARGIN_WIDTH 18

BEGIN_MESSAGE_MAP(CEANEdit, CCountryEdit)
    ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CEANEdit::CEANEdit() : CCountryEdit(g_EANMaskPL),m_bIgnore(false)
{
}


CString CEANEdit::EscapeMaskCharacter(const CString& code2a)
{
	CString esapedCode2a;

	for(int i = 0;i < code2a.GetLength();i++)
	{
		if(code2a[i] == MASK_CHARACTER)
		{
			esapedCode2a += '\\';
			esapedCode2a += MASK_CHARACTER;
		}
		else
		{
			esapedCode2a += code2a[i];
		}
	}
	return esapedCode2a;
}

void CEANEdit::OnCountryChange(Country *newCountry)
{
	char szMaskBuff[30];
	CString maskPrefix;

	if(newCountry == (Country*)-1)
	{
		maskPrefix = MASK_CHARACTER;
		maskPrefix += MASK_CHARACTER;
	}
	else
	{		
		maskPrefix = EscapeMaskCharacter(newCountry->GetCountryCode2A());
	}

	_snprintf(szMaskBuff,sizeof(szMaskBuff),"%s############",(LPCSTR)maskPrefix);
	SetMask(szMaskBuff);
	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW/* | RDW_ERASE*/);

}


void CEANEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
    CCountryEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
   
    // set button area equal to client area of edit control
    m_rcFlagRect = lpncsp->rgrc[0];
           
    lpncsp->rgrc[0].left += FLAGIMG_MARGIN_WIDTH;
    // shrink the button so its right side is at the left side of client area
     m_rcFlagRect.right = lpncsp->rgrc[0].left;
    
  
    if (bCalcValidRects)
        // convert button coordinates from parent client coordinates to control window coordinates
        m_rcFlagRect.OffsetRect(-lpncsp->rgrc[1].left, -lpncsp->rgrc[1].top);

    m_rcFlagRect.NormalizeRect();

    
}


void CEANEdit::OnSize(UINT nType, int cx, int cy) 
{
    CCountryEdit::OnSize(nType, cx, cy);
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CEANEdit::OnNcPaint() 
{
    CCountryEdit::OnNcPaint();      
    DrawFlag();
}


void CEANEdit::DrawFlag()
{
	CWindowDC	dcDraw( this );
	UINT flagImgIndex = m_flagRepository.GetFlagImageIndex(m_checkedCountry);

	CBrush whiteBrush;
	whiteBrush.Attach(::GetStockObject(WHITE_BRUSH));

	dcDraw.FillRect(&m_rcFlagRect,&whiteBrush);
	m_flagRepository.GetFlagsImageList().Draw(&dcDraw,flagImgIndex,CPoint(3,3),ILD_TRANSPARENT);

		
}

BOOL CEANEdit::OnValidate()
{
	if(!IsValidEAN())
	{
		DisplayToolTip(TRUE);
		return FALSE;
		
	}
	return TRUE;
}

Country* CEANEdit::ExtractCountry(const CString& EAN)
{
	CString countryCode = EAN.Left(2);
	return GetCountry(countryCode);
}

bool CEANEdit::SetEAN(const CString &EANValue)
{
	m_initialValue = EANValue;
	if(::IsWindow(m_hWnd))
	{
		CCowEanValidator eanValidator;

		if(eanValidator.IsHardViolation(EANValue))
			return false;

		Country* pCountry = ExtractCountry(EANValue);
		if(pCountry == NULL)
		{
			pCountry = (Country*)-1;
			m_checkedCountry = OTHER_COUNTRY_ISO_NUMBER;
		}
		else
		{
			m_checkedCountry = pCountry->GetCountryISONumber();
		}

		OnCountryChange(pCountry);
		SetWindowText(EANValue);
	}
	return true;
}

CString CEANEdit::GetEAN() const
{
	CString EAN;
	GetWindowText(EAN);
	return EAN;
}


BOOL CEANEdit::CheckEAN(LPCSTR EAN2Check)
{
	return CUtil::InCowEANValid(EAN2Check);
}

void CEANEdit::PrepareToolTip(CWnd *pParent)
{
	CString ttText,ttTitle;
	ttText.LoadString(IDS_EANTOOLTIPTEXT);
	ttTitle.LoadString(IDS_EANTOOLTIPTITLE);

	CreateToolTip(pParent,ttText,TTI_ERROR,ttTitle);
}

BOOL CEANEdit::IsValidEAN()
{
	if(AppCfg().GetBool(CFG_CHECKEANS))
	{
		CString data;
		if(IsInputEmpty()) return TRUE;
		GetWindowText(data);
		if(data == m_initialValue || m_bIgnore) return TRUE;
		return CheckEAN(data);		
	}
	return TRUE;
}
void CEANEdit::ExpandContextMenu(CMenu* pMenu) 
{
	ASSERT(pMenu);
	CString menuText;
	menuText.LoadString(IDS_MENUTEXTIGNORECKSUM);
	DWORD checkFlag = m_bIgnore ? MF_CHECKED : 0;
	pMenu->InsertMenu(-1,MF_BYPOSITION | checkFlag,IGNORE_EANCHKSUM_CMD , menuText);
}
BOOL CEANEdit::OnExpandedCommand(UINT nCommand)
{
	if(nCommand == (UINT)IGNORE_EANCHKSUM_CMD)
	{
		m_bIgnore = !m_bIgnore;
		return TRUE;
	}
	return FALSE;
	
}

void CEANEdit::PreSubclassWindow()
{
	CCountryEdit::PreSubclassWindow();
	//ModifyStyle(0, 0, SWP_FRAMECHANGED);
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	/*DWORD margins = GetMargins();
	WORD left = LOWORD(margins);
	WORD right = HIWORD(margins);

	SetMargins(left+20,right);*/

}

//
//BEGIN_MESSAGE_MAP(CEnterEANEdit, CEANEdit)
//    ON_WM_CHAR()
//	ON_WM_GETDLGCODE()
//END_MESSAGE_MAP()
//
//
//CEnterEANEdit::CEnterEANEdit() : CEANEdit()
//{}
//
//UINT CEnterEANEdit::OnGetDlgCode()
//{
//	return CEANEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
//}
//void CEnterEANEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//switch(nChar)
//{ /* nChar */
//	case VK_RETURN:
//	{
//	   	NMHDR hdr;
//		hdr.code = EN_EDITCOMPLETE;
//		hdr.hwndFrom = GetSafeHwnd();
//		hdr.idFrom = GetDlgCtrlID();
//		GetParent()->SendMessage(WM_NOTIFY,hdr.idFrom,(LPARAM)&hdr);
//	   return;
//	}
//}
//CEANEdit::OnChar(nChar, nRepCnt, nFlags);
//}
//
