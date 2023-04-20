#include "stdafx.h"
#include "idpz.h"
#include "datatypes/CompanyInfo.h"
#include "../barcodeimage/Cow2DBarcodeText.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"




IMPLEMENT_DYNCREATE(CTransInfoDocOptDlg, CCustomPrintDocOptDlg)
CTransInfoDocOptDlg::CTransInfoDocOptDlg (CWnd *pParent/* = NULL*/) : 
	CCustomPrintDocOptDlg(IDD,pParent) 
{
	
}

BOOL CTransInfoDocOptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	m_Spiecies  = PrintCfg().GetString(m_nCfgID,CBaseCowTransInfoDoc::CFG_SPIECIES);

	return TRUE;
}
void CTransInfoDocOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_SPIECIES,m_Spiecies);
	
}
void CTransInfoDocOptDlg::Save()
{
	UpdateData();
	PrintCfg().SetString(m_nCfgID,CBaseCowTransInfoDoc::CFG_SPIECIES,m_Spiecies);
}

BEGIN_MESSAGE_MAP(CTransInfoDocOptDlg, CCustomPrintDocOptDlg)
	
END_MESSAGE_MAP()
//#######################################################################################

#define COWTRANSINFOPAGE1_SIZE 16
#define COWTRANSINFOPAGE2_SIZE 38

#define BARCODESCOWTRANSINFOPAGE1_SIZE 10
#define BARCODESCOWTRANSINFOPAGE2_SIZE 19


CBaseCowTransInfoDoc::CBaseCowTransInfoDoc() : 	m_currDoc(NULL),starti(0),endi(0),m_nPages(0),m_currDocPageSize(0)
{
    
}

int CBaseCowTransInfoDoc::GetPrintPages() const
{
	return m_nPages;
}

int CBaseCowTransInfoDoc::LoadInputData(CPtrArray &docInputData)
{

	m_docs.RemoveAll();
	m_docs.Append((CDocDataArray&)docInputData);

	int pages  = 0,size,x;
	size = (int)m_docs.GetSize();
	for(int i = 0;i < size;i++)
	{
		x = ((int)m_docs[i]->GetCowCount())-GetCowsOnPage1();
		pages++; //at least one
		if(x > 0)
		{
			pages += x/GetCowsOnPage2();
			if(x%GetCowsOnPage2()) pages++;
		}
		
	}
    m_nPages = pages;
	
	m_currDoc = NULL;
	m_currDocPageSize = -1;
	m_currDocCows.RemoveAll();

	return RELOAD_OK_CONTINUE;

}

void CBaseCowTransInfoDoc::PreparePage(int nPage)
{

	int p = 0,i = 0,x,fp,size,row;
	Doc *pDoc = NULL;
	int nDocPageSize = -1;
			
	do
	{
		p++;
		fp  = p;
		pDoc = m_docs[i];
		x =  pDoc->GetCowCount() - GetCowsOnPage1();
		nDocPageSize = 1;
		if(x > 0) 
		{
			p += x/GetCowsOnPage2();
			nDocPageSize += x/GetCowsOnPage2(); 

			if(x%GetCowsOnPage2()) 
			{
				p++;
				nDocPageSize++;
			}
		}
		//curDocLastPage = p;
		i++;
	}
	while(p < nPage);


	if(m_currDoc != pDoc)
	{
		m_currDoc = pDoc;
		m_currDocPageSize = nDocPageSize;
		m_currDocCows.RemoveAll();
		m_currDocCows.AppendPtr(m_currDoc->GetCows());
		
		if(GetSortBy() != (COWDATA_MEMBERS)-1)
			m_currDocCows.SortBy(GetSortBy(),GetSortOrder());
	}
	

	size = m_currDocCows.GetSize();
	
	ResetGroupsVisibility(false);
	
	if(nPage == fp) 
	{
		starti = 0;
		endi = GetCowsOnPage1() - 1;
		SetTemplatePage(0);
		pageOff = 0;
		row = 1;
	}
	else
	{
		starti = GetCowsOnPage1() + (nPage-fp-1)*GetCowsOnPage2();
		endi = starti + GetCowsOnPage2() - 1;
		SetTemplatePage(1);
		pageOff = nPage-fp;
		row =  GetCowsOnPage1()+1;		
	}
				
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,row++)
	{
		SetGroupVisible(CStringFormat("%d",row),true);
		PrepareCow(m_currDocCows[i],row);
	}
}
void CBaseCowTransInfoDoc::PrintPage(int page)
{
	CString fld,str;
	int i,row;
	
	DrawText("PAGES",CStringFormat("%d/%d",pageOff+1,m_currDocPageSize));

	if(pageOff > 0) 
		row = GetCowsOnPage1()+1;
	else
	{
		const Company *pCompany = Company::GetCompany();
		
		CZapisSlowny aamount;
		row = 1;
		aamount.ZapisSlowny(m_currDocCows.GetSize(),str);
		DrawText("SPIECE",PrintCfg().GetString(GetCfgItemID(),CFG_SPIECIES));
		DrawText("ANIMALAMOUNT",(LPCSTR)str);
		DrawText("OWNRNAME",(LPCSTR)pCompany->GetCompanyName());
		DrawText("OWNRADDR",
				CStringFormat("%s %s, %s %s",(LPCSTR)pCompany->GetStreet(),(LPCSTR)pCompany->GetPOBox(),
												(LPCSTR)pCompany->GetZipCode(),(LPCSTR)pCompany->GetCity()));

		Hent* pHentBuyer = m_currDoc->GetHent();
		DrawText("BUYER",
				CStringFormat("%s, %s %s, %s %s",(LPCSTR)pHentBuyer->GetName(),(LPCSTR)pHentBuyer->GetStreet(),
												(LPCSTR)pHentBuyer->GetPOBox(),(LPCSTR)pHentBuyer->GetZip(),
												(LPCSTR)pHentBuyer->GetCity()));
	}
	for(i = starti;i <= endi;i++,row++)
	{		
		PrintCow(row,i+1,m_currDocCows[i]);
	}			
}

void CBaseCowTransInfoDoc::PrintCow(int row,int LP,const Cow* pCow)
{		
	DrawText(CStringFormat("COWNO%d",row),(LPCSTR)pCow->GetEAN());		
	DrawText(CStringFormat("PASSNO%d",row),(LPCSTR)pCow->GetPassNo());		
	DrawText(CStringFormat("LP%d",row),CIntString(LP));
}

void CBaseCowTransInfoDoc::PrepareCow(const Cow* pCow,int row)
{
}

//#######################################################################################
IMPLEMENT_DYNCREATE(CCowTransInfoDoc, CBaseCowTransInfoDoc)

int CCowTransInfoDoc::GetCowsOnPage1()
{
	return COWTRANSINFOPAGE1_SIZE;
}
int CCowTransInfoDoc::GetCowsOnPage2()
{
	return COWTRANSINFOPAGE2_SIZE;
}


//#######################################################################################

#define CODE39_NARROW_BAR (0.012)
#define CODE39_BARCODE_RATIO (4.0)
#define BARCODE_DPI (96)
#define CODE39_BARCODE_HEIGHT_INCH (0.4)
#define PDF417_BARCODE_HEIGHT_INCH (0.6)

CBarcodesCowTransInfoDoc::CowBarcodeImages::CowBarcodeImages(const Cow* pCow)
{
		m_bcCode39EAN.LoadData(pCow->GetEAN(),CODE39_NARROW_BAR,CODE39_BARCODE_HEIGHT_INCH,CODE39_BARCODE_RATIO,BARCODE_DPI);
		
		m_bcCode39EANImage.DrawBarcode(m_bcCode39EAN);
		//m_bcCode39EANImage.save(pCow->GetEAN() + ".bmp");

		m_bcCode39PassNo.LoadData(pCow->GetPassNo(),CODE39_NARROW_BAR,CODE39_BARCODE_HEIGHT_INCH,CODE39_BARCODE_RATIO,BARCODE_DPI);
		m_bcCode39PassNoImage.DrawBarcode(m_bcCode39PassNo);
		//m_bcCode39PassNoImage.save(pCow->GetPassNo() + ".bmp");

		CCow2DBarcodeText bcText(pCow);
		m_bcPFD417.SetMessage(bcText.GetBarcodeText(),BARCODE_DPI,PDF417_BARCODE_HEIGHT_INCH);
		m_bcPFD417Image.DrawBarcode(m_bcPFD417);

//		m_bcPFD417Image.save("PDF417.bmp");

}
CBarcodesCowTransInfoDoc::CowBarcodeImages::~CowBarcodeImages()
{
	//	bcEAN.Close();
	//bcPassNo.Close();
	//bcPDF417.Close();
}

void CBarcodesCowTransInfoDoc::CowBarcodeImages::GetCOWNOBCImageStream(COleStreamFile& cowNoBcImgStream)
{
	m_bcCode39EANImage.GetImageStream(cowNoBcImgStream);
}
void CBarcodesCowTransInfoDoc::CowBarcodeImages::GetPASSNOBCImageStream(COleStreamFile& passNoBcImgStream)
{
	m_bcCode39PassNoImage.GetImageStream(passNoBcImgStream);
}
void CBarcodesCowTransInfoDoc::CowBarcodeImages::GetPDF417ImageStream(COleStreamFile& pdf417ImgStream)
{
	m_bcPFD417Image.GetImageStream(pdf417ImgStream);
}


IMPLEMENT_DYNCREATE(CBarcodesCowTransInfoDoc, CBaseCowTransInfoDoc)

CBarcodesCowTransInfoDoc::CBarcodesCowTransInfoDoc()
{
}
CBarcodesCowTransInfoDoc::~CBarcodesCowTransInfoDoc()
{
	ClearBarcodesMap();
}
int CBarcodesCowTransInfoDoc::LoadInputData(CPtrArray &docInputData)
{
	ClearBarcodesMap();
	int reloadResult = CBaseCowTransInfoDoc::LoadInputData(docInputData);
	if(reloadResult == RELOAD_OK_CONTINUE)
	{
		Doc* pDoc = NULL;
		Cow* pCow = NULL;
		CDocDataArray::ArrayIterator docIt = m_docs.iterator();
		while(docIt.hasNext())
		{
			pDoc = *(docIt++);
			CCowArray::ArrayIterator cowIt = pDoc->GetCows()->iterator();
			while(cowIt.hasNext())
			{
				pCow = *(cowIt++);
				m_cowBarcodesMap.SetAt(pCow,new CowBarcodeImages(pCow));

			}
		}
		
		return RELOAD_OK_CONTINUE;
	}
	return reloadResult;
}
int CBarcodesCowTransInfoDoc::GetCowsOnPage1()
{
	return BARCODESCOWTRANSINFOPAGE1_SIZE;
}
int CBarcodesCowTransInfoDoc::GetCowsOnPage2()
{
	return BARCODESCOWTRANSINFOPAGE2_SIZE;
}
void CBarcodesCowTransInfoDoc::PrepareCow(const Cow* pCow,int row)
{
	CBaseCowTransInfoDoc::PrepareCow(pCow,row);
	CowBarcodeImages* pCowBarcode = NULL;

	COleStreamFile bcEAN;
	COleStreamFile bcPassNo;
	COleStreamFile bcPDF417;

	if(m_cowBarcodesMap.Lookup(pCow,pCowBarcode))
	{		
		pCowBarcode->GetCOWNOBCImageStream(bcEAN);
		pCowBarcode->GetPASSNOBCImageStream(bcPassNo);
		pCowBarcode->GetPDF417ImageStream(bcPDF417);
		if(!pCow->GetPassNo().IsEmpty())
			DrawImage(CStringFormat("PASSNOBC%d",row),bcPassNo);
		DrawImage(CStringFormat("COWNOBC%d",row),bcEAN);
		DrawImage(CStringFormat("PDF%d",row),bcPDF417);

		bcEAN.Close();
		bcPassNo.Close();
		bcPDF417.Close();
	}

}

void CBarcodesCowTransInfoDoc::ClearBarcodesMap()
{
	const Cow* pCow = NULL;
	CowBarcodeImages* pCowBarcode = NULL;
	POSITION mapPos = m_cowBarcodesMap.GetStartPosition();
	while(mapPos)
	{
		m_cowBarcodesMap.GetNextAssoc(mapPos,pCow,pCowBarcode);
		delete pCowBarcode;
	}	
}