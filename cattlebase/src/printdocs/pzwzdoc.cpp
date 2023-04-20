#include "stdafx.h"
#include "pzwzdoc.h"

#include "../logo/Logo.h"
#include "datatypes/CompanyInfo.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"

IMPLEMENT_DYNCREATE(CPzWzOptDlg, CCustomPrintDocOptDlg)


BEGIN_MESSAGE_MAP(CPzWzOptDlg, CCustomPrintDocOptDlg)
	
END_MESSAGE_MAP()



CPzWzOptDlg::CPzWzOptDlg(CWnd *pParent/* = NULL*/) : 
    CCustomPrintDocOptDlg(IDD,pParent), m_printExtras(TRUE) 
{
}

void CPzWzOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	DDX_Check(pDX,IDC_PRINTEXTRAS,m_printExtras);			
	DDX_Check(pDX,IDC_USEINVVALUES,m_bUseInvValues);
}
BOOL CPzWzOptDlg::OnInitDialog()
{
	CCustomPrintDocOptDlg::OnInitDialog();
	m_printExtras = PrintCfg().GetBool(m_nCfgID,CZDoc::CFG_PRINTEXTRAS);
	m_bUseInvValues = PrintCfg().GetBool(m_nCfgID,CZDoc::CFG_USEINVVALUES);
	UpdateData(FALSE);
	return TRUE;
}

void CPzWzOptDlg::Save()
{
	UpdateData();
	PrintCfg().SetBool(m_nCfgID,CZDoc::CFG_PRINTEXTRAS,m_printExtras);
	PrintCfg().SetBool(m_nCfgID,CZDoc::CFG_USEINVVALUES,m_bUseInvValues);
}

//#######################################################################################

CZDoc::CZDoc() : currDoc(NULL),starti(0),endi(0),printExtras(TRUE),m_nPages(0),bUseInvValues(FALSE)
{
   
}

int CZDoc::GetPrintPages() const
{
	return m_nPages;
}

int CZDoc::LoadInputData(CPtrArray &docInputData)
{
	m_docs.RemoveAll();
	m_docs.Append((CDocDataArray&)docInputData);

	int pages = 0,size,x;
	size = m_docs.GetSize();
	pages = size;
	for(int i = 0;i < size;i++)
	{
		x = m_docs[i]->GetCowCount()-PZWZDOCPAGE_1_SIZE;
		if(x > 0)
		{
			pages += x/PZWZDOCPAGE_2_SIZE;
			if(x%PZWZDOCPAGE_2_SIZE) pages++;
		}
	}
    m_nPages = pages;
	printExtras = PrintCfg().GetBool(GetCfgItemID(),CZDoc::CFG_PRINTEXTRAS);
	bUseInvValues = PrintCfg().GetBool(GetCfgItemID(),CZDoc::CFG_USEINVVALUES);
	m_currDocCows.RemoveAll();
	currDoc = NULL;
	currDocLastPage = -1;
	currDocTotalWeight = 0;

	return RELOAD_OK_CONTINUE;

}

void CZDoc::PreparePage(int nPage)
{
	int p = 0,i = 0,x,fp,grp,nDocLastPage = -1,newCurDocPageSize = 0;
	Doc *pDoc = NULL;
	CString grpname;
		
	do
	{
		p++;
		fp  = p;
		pDoc = m_docs[i];
		x =  pDoc->GetCowCount() - PZWZDOCPAGE_1_SIZE;
		if(x > 0) 
		{
			p += x/PZWZDOCPAGE_2_SIZE;
			if(x%PZWZDOCPAGE_2_SIZE) p++;
		}
		newCurDocPageSize = p-fp+1;
		nDocLastPage = p;
		i++;
	}
	while(p < nPage);

	if(currDoc != pDoc)
	{
		curDocPageSize = newCurDocPageSize;
		currDoc = pDoc;
		currDocLastPage = nDocLastPage;
		currDocTotalWeight = 0;
		
		m_currDocCows.RemoveAll();
		m_currDocCows.AppendPtr(currDoc->GetCows());

		if(GetSortBy() != (COWDATA_MEMBERS)-1)
			m_currDocCows.SortBy(GetSortBy(),GetSortOrder());
		
		Weight cowWeight;
		CCowArray::ArrayIterator cowIt = m_currDocCows.iterator();
		while(cowIt.hasNext())
		{
			cowWeight = GetCowWeight(*(cowIt++));
			currDocTotalWeight += cowWeight;
		}
	}

	int size = m_currDocCows.GetSize();
	
	ResetGroupsVisibility(false);
	if(nPage == fp) 
	{
		starti = 0;
		endi = PZWZDOCPAGE_1_SIZE - 1;
		SetTemplatePage(0);
		pageOff = 0;
		grp = 1;
		if(nPage == currDocLastPage) SetGroupVisible("totalweightp1",true);

		COleStreamFile logoStream;
		if(CLogoImage::GetLogo()->GetOleLogoImage(logoStream))
			DrawImage("LOGO",logoStream);

		

	}
	else
	{
		starti = PZWZDOCPAGE_1_SIZE + (nPage-fp-1)*PZWZDOCPAGE_2_SIZE;
		endi = starti + PZWZDOCPAGE_2_SIZE - 1;
		SetTemplatePage(1);
		pageOff = nPage-fp;
		grp = 41;    
		if(nPage == currDocLastPage) SetGroupVisible("totalweightp2",true);
	}
				
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		SetGroupVisible(grpname,true);
	}		

}

void CZDoc::PrintPage(int page)
{
	CString fld,str,seller_str,buyer_str;
	int i,row;
	const Hent *seller = GetSeller(),*buyer = GetBuyer();
	
	DrawText("DOCTYPE",GetZDocTitle());
	DrawText("DOCNUMBER",currDoc->GetDocNoStr());
	DrawText("PAGE",CStringFormat("%d/%d",pageOff+1,curDocPageSize));

	if(currDocTotalWeight > -1)
	{
		DrawText("TOTAL_WEIGHT",currDocTotalWeight.ToString(GetWeightFormat()));
	}


    if(pageOff > 0)
	{
		row = 41;	
	}
	else
	{			
		FormatHentInfo(seller,seller_str);
		FormatHentInfo(buyer,buyer_str);
		
		DrawText("SELLER",seller_str);
		DrawText("HENT",buyer_str);
		
		DrawText("DOCDATE",currDoc->GetDocDate().GetDayDate());
		DrawText("LOADDATE",currDoc->GetLoadDate().GetDayDate());
		DrawText("PLATENO",currDoc->GetPlateNo());

		if(printExtras) DrawText("EXTRAS",currDoc->GetExtras());
		row =1;
	}

		
		for(i = starti;i <= endi;i++,row++)
		{
	        PrintRecord(m_currDocCows[i],row,i);			
		}
}

void CZDoc::PrintRecord(Cow *pCow,int iRow,int iIndex)
{
	CString sCowGroup = GetCowGroup(pCow).ToString("%d","BEZ");
	Weight cowWeight = GetCowWeight(pCow);
	
	DrawText(CStringFormat("EAN%d",iRow),pCow->GetEAN());	
	DrawText(CStringFormat("STOCK%d",iRow),GetCowStock(pCow)->GetStockCode());
	DrawText(CStringFormat("SEX%d",iRow),CString((LPCSTR)pCow->GetSex()));

	DrawText(CStringFormat("WEIGHT%d",iRow),cowWeight.ToString(GetWeightFormat()));

	DrawText(CStringFormat("LP%d",iRow),CIntString(iIndex+1));

	DrawText(CStringFormat("GRP%d",iRow),(LPCSTR)sCowGroup);
	
}

void CZDoc::FormatHentInfo(const Hent* hent,CString &outStr)
{
		outStr.Format("%s\n%s %s, %s %s\n%s",
			(LPCSTR)hent->GetName(),
			(LPCSTR)hent->GetStreet(),
			(LPCSTR)hent->GetPOBox(),
			(LPCSTR)hent->GetZip(),
			(LPCSTR)hent->GetCity(),
			(LPCSTR)hent->GetFarmNo());
		if(hent->HasNIP())
			outStr.AppendFormat(", NIP: %s",(LPCSTR)hent->GetNIP());
		if(hent->HasREGON())
			outStr.AppendFormat(",REGON : %s",(LPCSTR)hent->GetREGON());			
	
}


IMPLEMENT_DYNCREATE(CPzDoc, CZDoc)
CPzDoc::CPzDoc()
{
}

Weight CPzDoc::GetCowWeight(Cow *pCow)
{
	Weight cowWeight = pCow->GetWeight();
	if(pCow->HasAnyBuyInvoice())
		cowWeight = Weight(pCow->GetBuyWeight().GetDouble());
	return cowWeight;
}
Stock* CPzDoc::GetCowStock(Cow *pCow)
{
	Stock* cowStock = pCow->GetStock();
	if(pCow->HasAnyBuyInvoice())
		cowStock = pCow->GetBuyStock();
	return cowStock;
}
NullInt CPzDoc::GetCowGroup(Cow* pCow)
{
	return pCow->GetInGroup();
}
CString CPzDoc::GetZDocTitle()
{
	return "PRZYJÊCIE ZEWNÊTRZNE";
}
const Hent* CPzDoc::GetBuyer()
{
	return Company::GetCompany()->GetAsHent();
}

const Hent* CPzDoc::GetSeller()
{
	return currDoc->GetHent();
}

IMPLEMENT_DYNCREATE(CWzDoc, CZDoc)
CWzDoc::CWzDoc()
{
}
Weight CWzDoc::GetCowWeight(Cow *pCow)
{
	Weight cowWeight = pCow->GetWeight();
	if(pCow->HasVATSellInvoice())
		cowWeight = Weight(pCow->GetSellWeight().GetDouble());
	return cowWeight;
}
Stock* CWzDoc::GetCowStock(Cow *pCow)
{
	Stock* cowStock = pCow->GetStock();
	if(pCow->HasVATSellInvoice())
		cowStock = pCow->GetSellStock();
	return cowStock;
}
NullInt CWzDoc::GetCowGroup(Cow* pCow)
{
	return pCow->GetOutGroup();
}

CString CWzDoc::GetZDocTitle()
{
	return "WYDANIE ZEWNÊTRZNE";
}

const Hent* CWzDoc::GetBuyer()
{
	return currDoc->GetHent();			
}

const Hent* CWzDoc::GetSeller()
{
	return Company::GetCompany()->GetAsHent();
}