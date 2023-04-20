#ifndef __QUESTIONBOX_H__
#define __QUESTIONBOX_H__

#include "../../NewPtrFlushArray.h"
#include "AnswerButton.h"
#include "QuestionDlgTemplate.h"

class QuestionBox 
{		

public:
	QuestionBox();
	~QuestionBox();
	QuestionBox(const CString& sCaption);
	
	QuestionBox& AddAnswer(const CString& sAnswer,UINT uiAnswerId);
	QuestionBox& SetCaption(const CString& sCaption);
	QuestionBox& SetQuestion(const CString& sQuestion);
	QuestionBox& ShowRemember(bool bShowRemember);
	QuestionBox& ShowCancelId(UINT uiCancelId);
	QuestionBox& ShowDefaultAnswer(UINT uiAnswerId);

	
	UINT Ask(CWnd* pParentWnd);
		
private:
	QuestionDlgTemplate m_DlgTemplate;
	

};
	

#endif