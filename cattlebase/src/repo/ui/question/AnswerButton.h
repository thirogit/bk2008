#ifndef __ANSWERBUTTON_H__
#define __ANSWERBUTTON_H__

class AnswerButton
{
public:
	AnswerButton(const CString& sBtnCaption,UINT uiBtnId);
	const CString& GetCaption() const;
	UINT GetButtonId() const;
private:
	CString m_sBtnCaption;
	UINT m_uiBtnId;
};

#endif