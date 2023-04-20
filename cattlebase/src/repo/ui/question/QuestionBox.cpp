#include "stdafx.h"
#include "QuestionBox.h"
#include "QuestionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

QuestionBox::QuestionBox()
{
}

QuestionBox::~QuestionBox()
{
	
}

QuestionBox::QuestionBox(const CString& sCaption) 
{
	m_DlgTemplate.SetCaption(sCaption);
}

QuestionBox& QuestionBox::AddAnswer(const CString& sAnswer,UINT uiAnswerId)
{
	m_DlgTemplate.AddAnswerButton(sAnswer,uiAnswerId);
	return *this;
}

QuestionBox& QuestionBox::SetCaption(const CString& sCaption)
{
	m_DlgTemplate.SetCaption(sCaption);
	return *this;
}

QuestionBox& QuestionBox::SetQuestion(const CString& sQuestion)
{
	m_DlgTemplate.SetQuestion(sQuestion);
	return *this;
}

QuestionBox& QuestionBox::ShowRemember(bool bShowRemember)
{
	m_DlgTemplate.ShowRemember(bShowRemember);
	return *this;
}

QuestionBox& QuestionBox::ShowCancelId(UINT uiCancelId)
{
	m_DlgTemplate.ShowCancelId(uiCancelId);
	return *this;
}

UINT QuestionBox::Ask(CWnd* pParentWnd)
{
	QuestionDlg dlg(&m_DlgTemplate);
	return dlg.Display(pParentWnd);
}
		
