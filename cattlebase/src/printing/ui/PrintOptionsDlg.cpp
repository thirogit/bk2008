#include "stdafx.h"
#include "PrintOptionsDlg.h"
#include "../../printdocs/PrintDocsCfg.h"

CPrintDocOptDlg::CPrintDocOptDlg(PRINTDOCS *const pDoc,CWnd* pParent /*=NULL*/)
	: CDialog(CPrintDocOptDlg::IDD, pParent),m_pDoc(pDoc),m_pDocOptDlg(NULL)
{
	ASSERT(pDoc);
}

CPrintDocOptDlg::~CPrintDocOptDlg()
{
	if(m_pDocOptDlg) delete m_pDocOptDlg;
}

void CPrintDocOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PRINTFONTCOMBO,m_PrintFont);
	DDX_Control(pDX, IDC_HEIGHTSPIN, m_HeightSpin);
	DDX_Control(pDX,IDC_MARGINSCTRL,m_OffMargs);
	DDX_Control(pDX,IDC_SORTCOMBO,m_sortCombo);
	DDX_Check(pDX,IDC_SORTORDER,m_bSortOrder);

	DDX_Control(pDX,IDC_MONEYFORMAT, m_MoneyFormat);
	DDX_Control(pDX,IDC_WEIGHTFORMAT, m_WeightFormat);
	
}


BEGIN_MESSAGE_MAP(CPrintDocOptDlg, CDialog)
	
END_MESSAGE_MAP()

BOOL CPrintDocOptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString docOptGrpBoxText,docName;

	m_PrintFont.InitFonts();
	m_HeightSpin.SetRange(1,25);		
	m_PrintFont.SelectString(-1,(LPCSTR)(PrintCfg().GetString(m_pDoc->iCfgItemId,CPrintCfg::CFG_FONTFAMILY)));
	m_HeightSpin.SetPos(MulDiv(PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_FONTHEIGHT),25,72));
	m_OffMargs.SetOffX(PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_OFFX));
	m_OffMargs.SetOffY(PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_OFFY));
	m_bSortOrder = PrintCfg().GetBool(m_pDoc->iCfgItemId,CPrintCfg::CFG_SORTORDER);
	m_MoneyFormat.SetFloatFormat(FloatFormat(PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_MONEYPRECISION),(char)PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_MONEYDECIMALPT)));
	m_WeightFormat.SetFloatFormat(FloatFormat(PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_WEIGHTPRECISION),(char)PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_WEIGHTDECIMALPT)));
	//m_PricePerKgFormat.SetFloatFormat(FloatFormat(PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_PRICEPERKGPRECISION),(char)PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_PRICEPERKGDECIMALPT)));


	docName.LoadString(m_pDoc->nResDocName);
	docOptGrpBoxText.Format(IDS_DOCOPTFMT,(LPCSTR)docName);
	SetDlgItemText(IDC_GROUPBOX_DOCOPT,(LPCSTR)docOptGrpBoxText);


	if(m_pDoc->pDocOptDlgClass != NULL)
	{
		CWnd *pNoDocLabel = GetDlgItem(IDC_DOCOPTIONS);
		CRect docOptRect;		
		CRuntimeClass *pDocOptDlgClass = m_pDoc->pDocOptDlgClass;
		ASSERT(pDocOptDlgClass->IsDerivedFrom(RUNTIME_CLASS(CCustomPrintDocOptDlg))); 
		m_pDocOptDlg = (CCustomPrintDocOptDlg*)pDocOptDlgClass->CreateObject();
		m_pDocOptDlg->SetCfgItemID(m_pDoc->iCfgItemId);
		m_pDocOptDlg->Create(m_pDocOptDlg->GetTemplateID(),this);		
		pNoDocLabel->GetWindowRect(docOptRect);
		ScreenToClient(docOptRect);	
		m_pDocOptDlg->SetWindowPos(&m_OffMargs, docOptRect.left, docOptRect.top, 
							0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
		pNoDocLabel->ShowWindow(SW_HIDE);
	}
	else
	{
		CString docOptNoText;
		docOptNoText.LoadString(IDS_NODOCOPTIONS);
		SetDlgItemText(IDC_DOCOPTIONS,(LPCSTR)docOptNoText);
	}


	 
	UINT cowsSortBy[][2] = 
	{
		{IDS_HEADERCOWNO,COWS_SORT_BY_EAN},
		{IDS_HEADERPASSNO,COWS_SORT_BY_PASSNO},
		{IDS_HEADERDELIVER,COWS_SORT_BY_DELIVER},
		{IDS_HEADERFSTOWNER,COWS_SORT_BY_FSTOWNER},
		{IDS_HEADERBIRTHDATE,COWS_SORT_BY_BIRTHDATE},
		{IDS_HEADERWEIGHT,COWS_SORT_BY_WEIGHT},
		{IDS_HEADERSEX,COWS_SORT_BY_SEX},
		{IDS_HEADERSTOCK,COWS_SORT_BY_STOCK},
		{IDS_HEADERBUYER,COWS_SORT_BY_BUYER},
		{IDS_HEADERINDOCNO,COWS_SORT_BY_INDOCNO},
		{IDS_HEADEROUTDOCNO,COWS_SORT_BY_OUTDOCNO},
		{IDS_HEADERMYPRICE,COWS_SORT_BY_MYPRICE},
		{IDS_HEADERBUYPRICE,COWS_SORT_BY_BUYPRICE},
		{IDS_HEADERSELLPRICE,COWS_SORT_BY_SELLPRICE},
		{IDS_HEADERINDATE,COWS_SORT_BY_INDATE},
		{IDS_HEADEROUTDATE,COWS_SORT_BY_OUT_DATE},
		{IDS_HEADEROUTGRP,COWS_SORT_BY_OUTGRP},
		{IDS_HEADERBUYSTOCK,COWS_SORT_BY_BUYSTOCK},
		{IDS_HEADERSELLSTOCK,COWS_SORT_BY_SELLSTOCK},
		{IDS_HEADERBUYWEIGHT,COWS_SORT_BY_BUYWEIGHT},
		{IDS_HEADERSELLWEIGHT,COWS_SORT_BY_SELLWEIGHT},
		{0,0}
		
	};


	if(m_pDoc->bSortCow)
	{
		CString sortCriteriumText;
		int i = 0,item,isel = -1;
		UINT nCfgCriterium = PrintCfg().GetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_COWSSORTBY);


		while(cowsSortBy[i][0] || cowsSortBy[i][1])
		{
			sortCriteriumText.LoadString(cowsSortBy[i][0]);
			item = m_sortCombo.AddString((LPCSTR)sortCriteriumText);
			if(nCfgCriterium == cowsSortBy[i][1])
				isel = item;
			else
			{
				if(item <= isel)
					isel++;
			}
			m_sortCombo.SetItemData(item,cowsSortBy[i][1]);
			i++;
		}

		m_sortCombo.SetCurSel(isel);
	}
	m_sortCombo.EnableWindow(m_pDoc->bSortCow);
	UpdateData(FALSE);
	return TRUE;
}
void CPrintDocOptDlg::OnOK()
{
	int currFont;
	CString lFaceName;

	UpdateData();

	if((currFont = m_PrintFont.GetCurSel()) == -1) 
	{
		AfxMessageBox(IDS_ERRCHOOSEDOCFONT,MB_OK,0);
		return;
	}		
	

	
	if(m_pDocOptDlg)
	{
		if(!m_pDocOptDlg->Validate())
			return;

		m_pDocOptDlg->Save();
	}



	m_PrintFont.GetLBText(currFont,lFaceName);

	PrintCfg().SetString(m_pDoc->iCfgItemId,CPrintCfg::CFG_FONTFAMILY,(LPCSTR)lFaceName);
	PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_FONTHEIGHT,MulDiv(m_HeightSpin.GetPos(),72,25));
	PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_OFFX,m_OffMargs.GetOffX());
	PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_OFFY,m_OffMargs.GetOffY());
	PrintCfg().SetBool(m_pDoc->iCfgItemId,CPrintCfg::CFG_SORTORDER,m_bSortOrder);
	
	FloatFormat moneyFmt = m_MoneyFormat.GetFloatFormat();
	FloatFormat weightFmt = m_WeightFormat.GetFloatFormat();
	//FloatFormat pricePerKgFmt = m_PricePerKgFormat.GetFloatFormat();

	PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_MONEYPRECISION,moneyFmt.GetPrecision());
	PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_MONEYDECIMALPT,moneyFmt.GetDecimalPoint());
	PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_WEIGHTPRECISION,weightFmt.GetPrecision());
	PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_WEIGHTDECIMALPT,weightFmt.GetDecimalPoint());
	//PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_PRICEPERKGPRECISION,pricePerKgFmt.GetPrecision());
	//PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_PRICEPERKGDECIMALPT,pricePerKgFmt.GetDecimalPoint());


	if(m_pDoc->bSortCow)
	{
		int currSort = m_sortCombo.GetCurSel();
		if(currSort >= 0)
			PrintCfg().SetNum(m_pDoc->iCfgItemId,CPrintCfg::CFG_COWSSORTBY,m_sortCombo.GetItemData(currSort));
	}

	
	EndDialog(IDOK);

}
