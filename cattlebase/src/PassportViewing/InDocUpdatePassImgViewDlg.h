#ifndef __INDOCUPDATEPASSIMGVIEWDLG_H__
#define   __INDOCUPDATEPASSIMGVIEWDLG_H__

#include "PassImgViewDlg.h"
#include "DataTypes.h"
#include "../PassportCommon/CowDataPassImgMap.h"

class CInDocUpdatePassImgViewDlg : public CPassImgViewDlg
{
	DECLARE_DYNAMIC(CInDocUpdatePassImgViewDlg)

public:
	CInDocUpdatePassImgViewDlg(CCowArray &cowArray,int iIndex,CCowDataPassImgMap &passImgs,CWnd* pParent = NULL);

	virtual ~CInDocUpdatePassImgViewDlg();


protected:
	CCowDataPassImgMap &m_passImgs;

	virtual bool GetPassImg(Cow *pCow);
	virtual bool PassImgExists(Cow *pCow);
protected:
	DECLARE_MESSAGE_MAP()
};
#endif