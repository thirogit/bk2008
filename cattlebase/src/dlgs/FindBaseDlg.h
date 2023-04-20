#ifndef __FINDBASEDLG_H__
#define __FINDBASEDLG_H__

#include "../ctrls/DateSpan.h"

class CFindSetup
{
public:
	virtual bool GetAddToResult() = 0;
	virtual bool GetNewSearch() = 0;
	virtual bool GetInResult() = 0;

	virtual bool ByCreationTime() = 0;
	virtual TIMESPAN CreationTimeSpan(CDateTime &time1,CDateTime &time2) = 0;
	virtual bool ByLastModificationTime() = 0;
	virtual TIMESPAN LastModificationTimeSpan(CDateTime &time1,CDateTime &time2) = 0;

	virtual UserData* SelectedCreator() = 0;
	virtual UserData* SelectedLastModifier() = 0;

	virtual bool ByExtras() = 0;
	virtual CString GetExtras() = 0;

	virtual void ChangeMainGrid(UINT gridId) = 0;


};


class CFindBaseDlg : public  CPropertyPage
{
public:
	CFindBaseDlg(UINT nIDTemplate) : CPropertyPage(nIDTemplate) {};

	virtual int GoSearch(CFindSetup& setup) = 0;

	
	BOOL PreTranslateMessage(MSG *pMsg)
    {
		if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		{
			GetParent()->GetParent()->SendMessage(WM_COMMAND,IDOK);
			return FALSE;
		}
        
		return CPropertyPage::PreTranslateMessage(pMsg);
    }

};

#endif