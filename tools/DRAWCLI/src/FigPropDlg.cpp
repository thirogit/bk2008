// FigPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRAWCLI.h"
#include "FigPropDlg.h"


// CFigPropDlg dialog

IMPLEMENT_DYNAMIC(CFigPropDlg, CBaseDlg)
CFigPropDlg::CFigPropDlg(CDrawRect *rectObj)
	: CBaseDlg(CFigPropDlg::IDD)
{
	m_Height = abs(rectObj->m_position.Height());
	m_Width = abs(rectObj->m_position.Width());
	m_roundnes = rectObj->m_roundness;
	m_Shape = rectObj->m_nShape;
}

CFigPropDlg::~CFigPropDlg()
{
}

void CFigPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROUNDX,m_roundX);
	DDX_Control(pDX, IDC_ROUNDY,m_roundY);
}


BEGIN_MESSAGE_MAP(CFigPropDlg, CBaseDlg)
	/*ON_NOTIFY(UDN_DELTAPOS,IDC_LEFTSPIN,OnDeltaPosX)
	ON_NOTIFY(UDN_DELTAPOS,IDC_RIGHTSPIN,OnDeltaPosX)
	ON_NOTIFY(UDN_DELTAPOS,IDC_TOPSPIN,OnDeltaPosY)
	ON_NOTIFY(UDN_DELTAPOS,IDC_BOTTOMSPIN,OnDeltaPosY)*/
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

BOOL CFigPropDlg::OnInitDialog()
{
	if(!CBaseDlg::OnInitDialog()) return FALSE;

	m_roundX.SetRange(0,m_Width/*/2*/);
	m_roundX.SetPos(m_roundnes.x);
	m_roundX.SetTicFreq(15);

	m_roundY.SetRange(0,m_Height/*/2*/);
	m_roundY.SetPos(m_roundnes.y);
	m_roundY.SetTicFreq(15);
	UpdateRoundnes();

	
	if(m_Shape != 	CDrawRect::roundRectangle) 
	{
		m_roundX.EnableWindow(FALSE);
		m_roundY.EnableWindow(FALSE);
	}


	return TRUE;
}/*
void CPosDlg::OnDeltaPosX(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	int nNextPos= (pNMUpDown->iPos+pNMUpDown->iDelta);
	int posRight = 0,posLeft = 0;
	
	switch(pNMUpDown->hdr.idFrom)
	{
	case IDC_LEFTSPIN:
		posLeft = nNextPos;
		posRight = m_RightSpin.GetPos32();
	break;		
	case IDC_RIGHTSPIN:
		posRight = nNextPos;
		posLeft = m_LeftSpin.GetPos32();
	break;
	}

	m_roundX.SetRange(0,abs(posRight-posLeft)/2);
	UpdateRoundnes();

	*pResult = 0;
}

void CPosDlg::OnDeltaPosY(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	int nNextPos= (pNMUpDown->iPos+pNMUpDown->iDelta);
	int posTop = 0,posBottom = 0;
	
	switch(pNMUpDown->hdr.idFrom)
	{
	case IDC_TOPSPIN:
		posTop = nNextPos;
		posBottom = (m_BottomSpin.GetPos32());
	break;		
	case IDC_BOTTOMSPIN:
		posBottom = nNextPos;
		posTop = (m_TopSpin.GetPos32());
	break;
	}

	m_roundY.SetRange(0,abs(posTop-posBottom)/2);
	UpdateRoundnes();
		

	*pResult = 0;
}*/

void CFigPropDlg::UpdateRoundnes()
{
	CString str;
	str.Format("(%d,%d)",m_roundX.GetPos(),m_roundY.GetPos());
	SetDlgItemText(IDC_ROUNDNES,str);
}

void CFigPropDlg::DoChanges(CDrawObj *pObj)
{
	((CDrawRect*)pObj)->m_roundness = m_roundnes;
}

void CFigPropDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	pScrollBar;
	nSBCode;
	nPos;
	UpdateRoundnes();
}

void CFigPropDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	pScrollBar;
	nSBCode;
	nPos;
	UpdateRoundnes();
}

BOOL CFigPropDlg::OnApply()
{
	m_roundnes.SetPoint(m_roundX.GetPos(),m_roundY.GetPos());
	return TRUE;
}