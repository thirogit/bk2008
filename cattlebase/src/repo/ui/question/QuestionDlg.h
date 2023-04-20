#ifndef __QUESTIONDLG_H__
#define __QUESTIONDLG_H__

#include "QuestionDlgTemplate.h"
#include "../../NewPtrFlushArray.h"

class QuestionDlg
{
public:
	QuestionDlg(const QuestionDlgTemplate* pTemplate); 
	virtual ~QuestionDlg();
	UINT Display(CWnd* pParentWnd);

protected:

	CSize PrepareMessageBox(CWnd* pDlgWnd);
	void RemoveAllButtons();

	void OnInitDialog(CWnd* pDlgWnd);

	static BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	const QuestionDlgTemplate* m_pDlgTemplate;

	NewPtrFlushArray<CWnd> m_DlgItems;
};

#endif