#include "stdafx.h"
#include "FloatFormatCtrl.h"
#include "../../classes/TempStringLoader.h"
#include "../../classes/IntString.h"

#define ID_GROUPBOXID 		0x1001
#define ID_PRECISIONCBID 	0x1004
#define ID_DECIMALPTCBID	0x1005



CFloatFormatCtrl::CFloatFormatCtrl()
{
	m_pGroupBox = new CButton;
	m_pDecimalPtLabel = new CStatic;
	m_pPrecisionLabel = new CStatic;
	m_pPrecisionCb = new CComboBox;
	m_pDecimalPtCb = new CComboBox;
	
	
}

CFloatFormatCtrl::~CFloatFormatCtrl()
{
	delete m_pGroupBox;
	delete m_pDecimalPtLabel;
	delete m_pPrecisionLabel;
	delete m_pPrecisionCb;
	delete m_pDecimalPtCb;
	
}


BEGIN_MESSAGE_MAP(CFloatFormatCtrl, CStatic)
	ON_WM_ENABLE()
END_MESSAGE_MAP()


void CFloatFormatCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP );
	CreateControls();
	InitControls();
}


BOOL CFloatFormatCtrl::CreateControls()
{

	CRect rect;
	GetClientRect(&rect);

	CString sText;
	GetWindowText(sText);

	CFont* pFont = GetFont();
	m_pGroupBox->Create(sText,WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_TABSTOP,rect,this,ID_GROUPBOXID);
	m_pGroupBox->ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	m_pGroupBox->EnableWindow(TRUE);
	m_pGroupBox->ShowWindow(SW_SHOW);
	m_pGroupBox->SetFont(pFont);
	m_pGroupBox->GetClientRect(&rect);


	CClientDC dc(m_pGroupBox);
	dc.SelectObject(GetFont());	
	CSize headerSize = dc.GetTextExtent(sText);
		
	CString sDecimalPtLblTxt = CTempStringLoader(IDS_DECIMALPOINTLBL);
	
	CSize decimalPtTxtSize = dc.GetTextExtent(sDecimalPtLblTxt);

	int top = headerSize.cy+2;
	int decimalPtLblLeft = 3;
	int decimalPtLblRight = decimalPtLblLeft+decimalPtTxtSize.cx;


	int decimalPtCbLeft = decimalPtLblRight+5;
	int decimalPtCbRight = decimalPtCbLeft+45;
	int decimalPtCbTop = top;
	int decimalPtCbBottom = decimalPtCbTop+200;
	CRect decimalPtCbRect(decimalPtCbLeft,decimalPtCbTop,decimalPtCbRight,decimalPtCbBottom);
	m_pDecimalPtCb->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,decimalPtCbRect,m_pGroupBox,ID_DECIMALPTCBID);
	m_pDecimalPtCb->EnableWindow(TRUE);
	m_pDecimalPtCb->ShowWindow(SW_SHOW);
	m_pDecimalPtCb->SetFont(GetFont());

	CRect cbRect;
	m_pDecimalPtCb->GetClientRect(&cbRect);
	int cbHeight = cbRect.Height();

	int middleOffset = (cbHeight-decimalPtTxtSize.cy)/2;
	int decimalPtLblTop = top+middleOffset;

	CRect decimalPtLblRect(decimalPtLblLeft,decimalPtLblTop,decimalPtLblRight,decimalPtLblTop+decimalPtTxtSize.cy);

	m_pDecimalPtLabel->Create(sDecimalPtLblTxt,WS_CHILD | WS_VISIBLE,decimalPtLblRect,m_pGroupBox);
	m_pDecimalPtLabel->EnableWindow(TRUE);
	m_pDecimalPtLabel->ShowWindow(SW_SHOW);
	m_pDecimalPtLabel->SetFont(GetFont());


	CString sPrecisionLblText = CTempStringLoader(IDC_PRECISIONLBL);
	CSize precisionTxtSize = dc.GetTextExtent(sPrecisionLblText);
	
	int precLblLeft = decimalPtCbRect.right+10;
	int precLblRight = precLblLeft+precisionTxtSize.cx;
	
	int precisionCbLeft = precLblRight+5;
	int precisionCbRight = precisionCbLeft+45;
	int precisionCbTop = top;
	int precisionCbBottom = precisionCbTop+200;
	CRect precisionCbRect(precisionCbLeft,precisionCbTop,precisionCbRight,precisionCbBottom);
	m_pPrecisionCb->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,precisionCbRect,m_pGroupBox,ID_PRECISIONCBID);
	m_pPrecisionCb->EnableWindow(TRUE);
	m_pPrecisionCb->ShowWindow(SW_SHOW);
	m_pPrecisionCb->SetFont(GetFont());
	
	m_pPrecisionCb->GetClientRect(&cbRect);
	cbHeight = cbRect.Height();

	middleOffset = (cbHeight-precisionTxtSize.cy)/2;
	int precLblTop = top+middleOffset;
	int precLblBottom = precLblTop+precisionTxtSize.cy;
	CRect precisionLblRect(precLblLeft,precLblTop,precLblRight,precLblBottom);
	m_pPrecisionLabel->Create(sPrecisionLblText,WS_CHILD | WS_VISIBLE,precisionLblRect,m_pGroupBox);
	m_pPrecisionLabel->EnableWindow(TRUE);
	m_pPrecisionLabel->ShowWindow(SW_SHOW);
	m_pPrecisionLabel->SetFont(GetFont());

	return TRUE;
}

BOOL CFloatFormatCtrl::InitControls(const FloatFormat* pFormat/* = NULL*/)
{
	const char decimalPointCharacters[] = {'.',',',0};
	const int precisions[] = {2,3,4,5,6,0};


	m_pDecimalPtCb->ResetContent();
	CString sItemText;
	int iItem,iSelectedItem = -1;
	const char* pDecimalPoint = decimalPointCharacters;
	while(*pDecimalPoint)
	{
		sItemText.Empty();
		sItemText += *pDecimalPoint;
		iItem = m_pDecimalPtCb->AddString(sItemText);
		m_pDecimalPtCb->SetItemData(iItem,*pDecimalPoint);

		if(pFormat && *pDecimalPoint == pFormat->GetDecimalPoint() && pFormat->GetDecimalPoint() != '\0')
		{
			iSelectedItem = iItem;
		}

		pDecimalPoint++;
	}

	if(iSelectedItem < 0 && pFormat)
	{
		sItemText.Empty();
		sItemText += pFormat->GetDecimalPoint();
		iSelectedItem = m_pDecimalPtCb->AddString(sItemText);
		m_pDecimalPtCb->SetItemData(iSelectedItem,pFormat->GetDecimalPoint());
	}
	m_pDecimalPtCb->SetCurSel(iSelectedItem);


	m_pPrecisionCb->ResetContent();
	iSelectedItem = -1;
	const int* pPresision = precisions;
	while(*pPresision)
	{
		iItem = m_pPrecisionCb->AddString(CIntString(*pPresision));
		m_pPrecisionCb->SetItemData(iItem,*pPresision);

		if(pFormat && *pPresision == pFormat->GetPrecision() && pFormat->GetPrecision() > 0)
		{
			iSelectedItem = iItem;
		}

		pPresision++;
	}

	if(iSelectedItem < 0 && pFormat)
	{
		iSelectedItem = m_pPrecisionCb->AddString(CIntString(pFormat->GetPrecision()));
		m_pPrecisionCb->SetItemData(iItem,pFormat->GetPrecision());
	}
	m_pPrecisionCb->SetCurSel(iSelectedItem);

	return TRUE;

}

void CFloatFormatCtrl::OnEnable(BOOL bEnable)
{
	m_pGroupBox->EnableWindow(bEnable);
	m_pDecimalPtLabel->EnableWindow(bEnable);
	m_pPrecisionLabel->EnableWindow(bEnable);
	m_pPrecisionCb->EnableWindow(bEnable);
	m_pDecimalPtCb->EnableWindow(bEnable);

	CStatic::OnEnable(bEnable);

}

FloatFormat CFloatFormatCtrl::GetFloatFormat() const
{
	char cDecimalPoint = '\0';
	int iPrecision = 0;

	int iSelectedPrecision = m_pPrecisionCb->GetCurSel();
	int iSelectedDecimalPoint = m_pDecimalPtCb->GetCurSel();
	if(iSelectedDecimalPoint >= 0)
	{
		cDecimalPoint = (char)m_pDecimalPtCb->GetItemData(iSelectedDecimalPoint);
	}

	if(iSelectedPrecision >= 0)
	{
		iPrecision = (int)m_pPrecisionCb->GetItemData(iSelectedPrecision);
	}

	return FloatFormat(iPrecision,cDecimalPoint);
}

void CFloatFormatCtrl::SetFloatFormat(const FloatFormat& fmt)
{
	InitControls(&fmt);
}