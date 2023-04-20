#ifndef __DYMOBASEHENTLABEL_H__
#define __DYMOBASEHENTLABEL_H__

#include "DYMOBaseDoc.h"

class CDYMOBaseHentLabel : public CDYMOBaseDoc
{
	DECLARE_DYNCREATE(CDYMOBaseHentLabel)
public:
	CDYMOBaseHentLabel();
	virtual int LoadInputData(CPtrArray &docInputData);
	void SetFields(int nLabelPage);
	virtual int GetPrintPages() const;
protected:
	CHentArray m_hents;

};

#endif