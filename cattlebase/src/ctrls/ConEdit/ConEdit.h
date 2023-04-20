#ifndef __CONEDIT_H__
#define __CONEDIT_H__

class CConEdit : public CEdit
{
public:
	CConEdit();
	~CConEdit();
	void AddText(LPCSTR pszOutput);

protected:
	CFont *m_font;

	virtual void PreSubclassWindow();
	void SetConsoleFont();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

DECLARE_MESSAGE_MAP();

};



#endif