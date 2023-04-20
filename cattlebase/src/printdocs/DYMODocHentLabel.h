#ifndef __DYMODOCHENTLABEL_H__
#define __DYMODOCHENTLABEL_H__

#include "DYMOBaseHentLabel.h"

class CDYMODocHentLabel : public CDYMOBaseHentLabel
{
	DECLARE_DYNCREATE(CDYMODocHentLabel)
public:
	CDYMODocHentLabel();
	virtual int LoadInputData(CPtrArray &docInputData);
};

#endif