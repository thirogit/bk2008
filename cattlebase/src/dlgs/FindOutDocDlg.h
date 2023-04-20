#ifndef __FINDOUTDOCDLG_H__
#define __FINDOUTDOCDLG_H__

#include "../Doc/FindDocBaseDlg.h"

class CFindOutDocDlg : public  CFindDocBaseDlg
{
public:
	CFindOutDocDlg ();
protected:
	virtual void SearchDoc(CFindAction<CDocDataArray::ArrayAction,CDocDataMatcher,Doc>& findAction,CFindSetup& setup);
	virtual void InitControls();
	
};


#endif