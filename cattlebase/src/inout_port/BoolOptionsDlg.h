#ifndef __BOOLOPTIONSDLG_H__
#define __BOOLOPTIONSDLG_H__

#include "../dlgs/CheckListDlg.h"
#include "BoolSettings.h"

#define IDSAVEANDUSE 100100
#define IDUSEONLYONCE 100200

class CBoolOptionsDlg : public CCheckListDlg
{
public:	
	struct BOOLOPTIONS
	{
		UINT nResIdOptionName;
		bool *bValuePtr;
	};

public:
	CBoolOptionsDlg(BOOLOPTIONS* pBoolOptions,CWnd* pParent = NULL);   
	virtual ~CBoolOptionsDlg();
protected:
	virtual CString GetDlgCaption() = 0;
	virtual void SaveAndUse();
	virtual void Init();
	virtual void UseOnlyOnce();

	BOOLOPTIONS* m_pBoolOptions;
	
};

#endif