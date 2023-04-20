#ifndef __FINDINDOCDLG_H__
#define __FINDINDOCDLG_H__

#include "../FindDocBaseDlg.h"

class CFindInDocDlg : public  CFindDocBaseDlg
{
public:
	CFindInDocDlg ();
protected:
	virtual void SearchDoc(CFindAction<CDocDataArray::ArrayAction,CDocDataMatcher,Doc>& findAction,CFindSetup& setup);
	virtual void InitControls();
		
};


#endif