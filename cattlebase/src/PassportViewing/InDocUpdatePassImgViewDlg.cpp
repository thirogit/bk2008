#include "stdafx.h"
#include "InDocUpdatePassImgViewDlg.h"




IMPLEMENT_DYNAMIC(CInDocUpdatePassImgViewDlg, CPassImgViewDlg)

CInDocUpdatePassImgViewDlg::CInDocUpdatePassImgViewDlg(CCowArray &cowArray,int iIndex,CCowDataPassImgMap &passImgs,CWnd* pParent/* = NULL*/) :
							CPassImgViewDlg(cowArray,iIndex,pParent),m_passImgs(passImgs)
{

}

CInDocUpdatePassImgViewDlg::~CInDocUpdatePassImgViewDlg()
{
}



BEGIN_MESSAGE_MAP(CInDocUpdatePassImgViewDlg, CPassImgViewDlg)
END_MESSAGE_MAP()



bool CInDocUpdatePassImgViewDlg::GetPassImg(Cow *pCow)
{
	if(!CPassImgViewDlg::GetPassImg(pCow))
	{
		CString imgPath;
		if(m_passImgs.Lookup(pCow,imgPath))
		{
			if(m_passPix.load(imgPath)) return true;
		}
		return false;
	}
	return true;
}

bool CInDocUpdatePassImgViewDlg::PassImgExists(Cow *pCow)
{
	return CPassImgViewDlg::PassImgExists(pCow) || (m_passImgs.PLookup(pCow) != NULL);
}