#include "stdafx.h"
#include "QuestionDlgTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


QuestionDlgTemplate::QuestionDlgTemplate() : m_uiCancelId(INVALID_ANSWER_ID),
											 m_uiDefaultAnswerId(INVALID_ANSWER_ID),
											 m_bShowRemember(false)
{
}

QuestionDlgTemplate::~QuestionDlgTemplate()
{
	DeleteAllButtons();
}


void QuestionDlgTemplate::AddAnswerButton(const CString& sAnswerBtnCaption,UINT uiAnswerId)
{
	m_AnswerButtons.Add(new AnswerButton(sAnswerBtnCaption,uiAnswerId));
}

int QuestionDlgTemplate::GetAnswerButtonCount() const
{
	return m_AnswerButtons.GetSize();
}

const AnswerButton* QuestionDlgTemplate::GetButton(int iBtn) const
{
	return const_cast<QuestionDlgTemplate*>(this)->m_AnswerButtons.GetAt(iBtn);
}

void QuestionDlgTemplate::DeleteButton(int iBtn)
{
	m_AnswerButtons.RemoveAt(iBtn);
}

void QuestionDlgTemplate::DeleteAllButtons()
{
	m_AnswerButtons.FreeAndFlush();
}

void QuestionDlgTemplate::SetCaption(const CString& sCaption)
{
	m_sCaption = sCaption;
}

void QuestionDlgTemplate::SetQuestion(const CString& sQuestion)
{
	m_sQuestion = sQuestion;
}

void QuestionDlgTemplate::ShowRemember(bool bShowRemember)
{
	m_bShowRemember = bShowRemember;
}

void QuestionDlgTemplate::ShowCancelId(UINT uiCancelId)
{
	m_uiCancelId = uiCancelId;
}

void QuestionDlgTemplate::SeDefaultAnswer(UINT uiAnswerId)
{
	m_uiDefaultAnswerId = uiAnswerId;
}

CString QuestionDlgTemplate::GetCaption() const
{
	return m_sCaption;
}

CString QuestionDlgTemplate::GetQuestion() const
{
	return m_sQuestion;
}

bool QuestionDlgTemplate::IsShowRemebmer() const
{
	return m_bShowRemember;
}

UINT QuestionDlgTemplate::GetCancelId() const
{
	return m_uiCancelId;
}

UINT QuestionDlgTemplate::GetDefaultAnswer() const
{
	return m_uiDefaultAnswerId;
}
	
	
	