#include "stdafx.h"
#include "ConEdit.h"

BEGIN_MESSAGE_MAP(CConEdit,CEdit)
	ON_WM_CREATE()
END_MESSAGE_MAP()

CConEdit::CConEdit() : CEdit(),m_font(NULL)
{
}

CConEdit::~CConEdit()
{
	if(m_font) delete m_font;
}

void CConEdit::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();
	SetConsoleFont();
}

void CConEdit::AddText(LPCSTR pszOutput)
{
	int nSize = GetWindowTextLength();
	SetSel(nSize, nSize);
	ReplaceSel(pszOutput);		// add the message to the end of Edit control
}

void CConEdit::SetConsoleFont()
{
	LOGFONT lf;
	if(m_font) return;

	CFont *oldFont = GetFont();
	oldFont->GetLogFont(&lf);
	strcpy(lf.lfFaceName,"Lucida Console");
	m_font = new CFont();
	m_font->CreateFontIndirect(&lf);
	SetFont(m_font);
}

int CConEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CEdit::OnCreate(lpCreateStruct) == -1) return -1;
	SetConsoleFont();
	return 0;
}

