#include "stdafx.h"
#include "FastSelEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CFastSelEdit, CEnterEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

CFastSelEdit::CFastSelEdit() : m_currentContent(NONE)
{	
}

void CFastSelEdit::SelAllText()
{
	SetSel(0,-1);
}
void CFastSelEdit::OnReturnKey()
{		
	SelAllText();
	CEnterEdit::OnReturnKey();
}

bool CFastSelEdit::IsTextValid (const TCHAR *pszText)
{
	if(strlen(pszText))
		return CEnterEdit::IsTextValid(pszText);

	return true;
}
void CFastSelEdit::SetContentToText()
{
	if(m_currentContent == NONE || m_currentContent != TEXT)
	{
		ResetEdit();
		ModifyStyle(ES_UPPERCASE,0);
		SetRegEx(".+");
		m_currentContent = TEXT;
	}
}
void CFastSelEdit::SetContentToNumber()
{
	if(m_currentContent == NONE || m_currentContent != NUMBER)
	{
		ResetEdit();
		ModifyStyle(0,ES_UPPERCASE);
		SetRegEx("[0-9]{0,6}");
		m_currentContent = NUMBER;
	}
}
void CFastSelEdit::SetContentToEAN()
{
	if(m_currentContent == NONE || m_currentContent != EAN)
	{
		ResetEdit();
		ModifyStyle(0,ES_UPPERCASE);
		SetRegEx("^[a-zA-Z*?]{1,2}[0-9*?]{1,12}");
		m_currentContent = EAN;
	}	
}

void CFastSelEdit::ResetEdit()
{
	SetWindowText("");
	m_bUncommited = false;
	Invalidate(TRUE);
}

void CFastSelEdit::OnKillFocus(CWnd* pNewWnd)
{
	CBarcodeScannerDriver::RemoveReceiver(this);
	CEnterEdit::OnKillFocus(pNewWnd);
}
void CFastSelEdit::OnSetFocus(CWnd* pOldWnd)
{
	CBarcodeScannerDriver::AddReceiver(this);
	CEnterEdit::OnSetFocus(pOldWnd);
}
void CFastSelEdit::OnBarcode(const CString& sBarcode)
{
	if(m_currentContent == EAN)
	{
		SetWindowText(sBarcode);
		OnReturnKey();
	}
}