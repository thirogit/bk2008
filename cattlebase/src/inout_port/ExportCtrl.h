#ifndef __EXPORTCTRL_H__
#define __EXPORTCTRL_H__

#include "../datatypes/Doc.h"
#include "../datatypes/DocDataArray.h"
#include "ExportSettings.h"

class CExportCtrl
{
public:
	CExportCtrl(CWnd* pParentWnd);
	void ExportOutDoc(CDocDataArray& docs,const CString& outputFilePath);
	void ExportInDoc(CDocDataArray& docs,const CString& outputFilePath);
private:
	CWnd* m_pParentWnd;

};

#endif