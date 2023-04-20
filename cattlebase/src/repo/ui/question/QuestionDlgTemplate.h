#ifndef __QUESTIONDLGTEMPLATE_H__
#define __QUESTIONDLGTEMPLATE_H__

#include "../../NewPtrFlushArray.h"
#include "AnswerButton.h"

#define INVALID_ANSWER_ID ((UINT)-1)

class QuestionDlgTemplate 
{		

public:
	QuestionDlgTemplate();
	~QuestionDlgTemplate();
	
	
	void AddAnswerButton(const CString& sAnswerBtnCaption,UINT uiAnswerId);
	int GetAnswerButtonCount() const;
	const AnswerButton* GetButton(int iBtn) const;
	void DeleteButton(int iBtn);
	void DeleteAllButtons();
	
	void SetCaption(const CString& sCaption);
	void SetQuestion(const CString& sQuestion);
	void ShowRemember(bool bShowRemember);
	void ShowCancelId(UINT uiCancelId);
	void SeDefaultAnswer(UINT uiAnswerId);

	CString GetCaption() const;
	CString GetQuestion() const;
	bool IsShowRemebmer() const;
	UINT GetCancelId() const;
	UINT GetDefaultAnswer() const;	
		
private:
	
	NewPtrFlushArray<AnswerButton> m_AnswerButtons;
	CString m_sCaption;
	CString m_sQuestion;
	bool m_bShowRemember;
	UINT m_uiCancelId;
	UINT m_uiDefaultAnswerId;

};
	

#endif