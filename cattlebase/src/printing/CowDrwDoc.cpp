#include "stdafx.h"
#include "CowDrwDoc.h"
#include "../missing/util.h"
#include "../exceptions/StrException.h"
#include "../classes/TempStringLoader.h"
#include "../Utils/StringFormat.h"
#include "DocObjList.h"
#include "Picture.h"
#include "DocImgObj.h"
#include "../printdocs/PrintDocsCfg.h"

#define WHITE_COLOR (RGB(255, 255, 255))

void CCowDrwDoc::CreateCowDocument(const PRINTDOCS* pDoc)
{
	CString templatePath;
	templatePath.Format("%s\\%s.DRW",(LPCSTR)CUtil::GetAppDir(),pDoc->szDocTemplateName);

	if(!OpenDocumentTemplate(templatePath))
	{
		throw new CStrException(CStringFormat(CTempStringLoader(IDS_DOCTEMPLATEMISSING),(LPCSTR)templatePath));
	}
}

int CCowDrwDoc::Load(CPtrArray &docInputData)
{
	SetDocOptions();
	return CCowBasePrintDoc::Load(docInputData);
}

bool CCowDrwDoc::OpenDocumentTemplate(const CString& templatePath)
{
	return OnOpenDocument(templatePath) == TRUE;
}

CCowDrwDoc::CCowDrwDoc()
{
	EnableCompoundFile();

	m_paperColor = WHITE_COLOR;
	m_pDocSummary = NULL;
	
	m_styles.AddHead(new CTextStyle(CString(USERDEF_STYLENAME),USERDEF_STYLEID));
	m_DocPages.Add(new CDocObjList);

	m_CurrPage = 0;
	
	ComputePageSize();

	m_pDC = NULL;

}

CCowDrwDoc::~CCowDrwDoc()
{
	DestroyDocument();
}

void CCowDrwDoc::DestroyDocument()
{
	for(int i=0;i<m_DocPages.GetCount();i++)
	{
		CDocObjList *ObjList =	(CDocObjList*)m_DocPages.GetAt(i);
		POSITION posObj = ObjList->GetHeadPosition();
		while (posObj != NULL)	
			delete ObjList->GetNext(posObj);

		delete ObjList;
	}
	m_DocPages.RemoveAll();

	delete m_pDocSummary;
	m_pDocSummary = NULL;

	POSITION pos = m_groups.GetHeadPosition();
	while(pos)
	{
		delete (CDocObjGroup*)m_groups.GetNext(pos);
	}
	m_groups.RemoveAll();

	pos = m_styles.GetHeadPosition();
	while(pos)
	{
		delete (CTextStyle*)m_styles.GetNext(pos);
	}
	m_styles.RemoveAll();

	
	m_CurrPage = 0;
	m_PaperSize.SetSize(0,0);
	m_PageSize.SetSize(0,0);
	m_PhyMargins.SetSize(0,0);

	m_paperColor = WHITE_COLOR;
	m_orientation = DMORIENT_PORTRAIT;
	m_PaperType = DMPAPER_A4;

}


BOOL CCowDrwDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	DestroyDocument();

	m_pDocSummary = new CDocSummary;
	//m_DocPages.Add(new CDocObjList);
	m_CurrPage = 0;

	return COleDocument::OnOpenDocument(lpszPathName);
}

void CCowDrwDoc::Serialize(CArchive& ar)
{
	int i;
	POSITION pos;
	CDocObjGroup *pGrp;
	CTextStyle *pStyle;
	if (ar.IsStoring())
	{
		ar << m_paperColor;
		ar << (int)m_DocPages.GetCount();
		ar << m_orientation;
		ar << m_PaperType;
		ar << (int)m_groups.GetSize();

		pos = m_groups.GetHeadPosition();
		while(pos)
			m_groups.GetNext(pos)->WriteToArchive(ar);
		
		ar << (int)m_styles.GetSize();

		pos = m_styles.GetHeadPosition();
		while(pos)
			m_styles.GetNext(pos)->Serialize(ar);

		for(i=0;i<m_DocPages.GetCount();i++)
			((CDocObjList*)m_DocPages[i])->Serialize(ar);

		m_pDocSummary->WriteToStorage(m_lpRootStg);
	}
	else
	{
		int NumPages,NumGroups,NumTextStyles;

		ar >> m_paperColor;
		ar >> NumPages;
		ar >> m_orientation;
		ar >> m_PaperType;

		ar >> NumGroups;

		for(i = 0;i < NumGroups;i++)
		{
			pGrp = CDocObjGroup::ReadFromArchive(ar);
			m_groups.AddTail(pGrp); 
		}

		ar >> NumTextStyles;

		for(i =0;i < NumTextStyles;i++)
		{
			pStyle = new CTextStyle();
			pStyle->Serialize(ar);
			m_styles.AddTail(pStyle);

		}
		
		for(i = 0;i < NumPages;i++)
			m_DocPages.Add(new CDocObjList);

		for(i=0;i < NumPages;i++)
			((CDocObjList*)m_DocPages[i])->Serialize(ar);


		CPrintDialog dlg(FALSE);
		if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
		{
			LPDEVMODE pDM;
			if ( (dlg.m_pd.hDevMode) &&  (pDM = (DEVMODE*)GlobalLock(dlg.m_pd.hDevMode)) )
			{     
				pDM->dmOrientation = m_orientation; 
				pDM->dmPaperSize = m_PaperType;
				GlobalUnlock(dlg.m_pd.hDevMode);
			}
		}


		ComputePageSize();
		m_pDocSummary->ReadFromStorage(m_lpRootStg);
	}
	
	COleDocument::Serialize(ar);
}

void CCowDrwDoc::Draw(CDC* pDC)
{

	CDocObjList *objects = (CDocObjList *)m_DocPages.GetAt(m_CurrPage);

	POSITION pos = objects->GetHeadPosition();
	while (pos != NULL)
	{
		CDocObj* pObj = objects->GetNext(pos);
		CDocObjGroup * pGroup = pObj->GetGroup();
		if(pGroup)
		{
			if(pGroup->IsVisible())	pObj->Draw(pDC);
		} 
		else 
			pObj->Draw(pDC);
	}
}


void CCowDrwDoc::ComputePageSize()
{

	CSize new_size(2100, 2970);  // 8.5" x 11" default

	CPrintDialog dlg(FALSE);

	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{
		LPDEVMODE pDM;
		if ( (dlg.m_pd.hDevMode) &&
			(pDM = (DEVMODE*)GlobalLock(dlg.m_pd.hDevMode)) )
		{     
			m_orientation = pDM->dmOrientation;
			m_PaperType = pDM->dmPaperSize;
			GlobalUnlock(dlg.m_pd.hDevMode);
		}
		else 
		{
			m_orientation = DMORIENT_LANDSCAPE;//DMORIENT_PORTRAIT;
			m_PaperType = DMPAPER_A4;
		}


		// GetPrinterDC returns a HDC so attach it
		CDC dc;
		HDC hDC= dlg.CreatePrinterDC();
		ASSERT(hDC != NULL);
		dc.Attach(hDC);

		m_PageSize.cx = dc.GetDeviceCaps(HORZSIZE)*10;
		m_PageSize.cy = dc.GetDeviceCaps(VERTSIZE)*10;

		int LOGPInchX = dc.GetDeviceCaps(LOGPIXELSX);
		int LOGPInchY= dc.GetDeviceCaps(LOGPIXELSY);

		int PhyWidth = dc.GetDeviceCaps(PHYSICALWIDTH);
		int PhyHeight = dc.GetDeviceCaps(PHYSICALHEIGHT);

		int PhyOffsetX = dc.GetDeviceCaps(PHYSICALOFFSETX);
		int PhyOffsetY = dc.GetDeviceCaps(PHYSICALOFFSETY);


		new_size.cx = MulDiv(PhyWidth,254,LOGPInchX);
		new_size.cy = MulDiv(PhyHeight,254,LOGPInchY);

		m_PhyMargins.cx = MulDiv(PhyOffsetX,254,LOGPInchX);
		m_PhyMargins.cy = MulDiv(PhyOffsetY,254,LOGPInchY);
	}

	if (new_size != m_PaperSize)
	{
		m_PaperSize = new_size;		
	}
}

CDocObjList* CCowDrwDoc::GetObjects()
{ 
	return GetPageObjects(m_CurrPage);
}
CDocObjList* CCowDrwDoc::GetPageObjects(int nPage)
{
	if(nPage >= 0 && nPage < m_DocPages.GetCount())
		return (CDocObjList*)m_DocPages.GetAt(nPage);
	return NULL;
}

CDocObj* CCowDrwDoc::GetObjByLabel(const CString& sObjLabel)
{
	CDocObjList *l_objects = (CDocObjList *)m_DocPages.GetAt(m_CurrPage);
	POSITION pos = l_objects->GetHeadPosition();
	while (pos != NULL)
	{
		CDocObj* pObj = l_objects->GetNext(pos);
		if(!pObj->GetLabel().Compare(sObjLabel)) return pObj;

	}
	return NULL;
}

bool CCowDrwDoc::SetUserDefStyle(const CString& sFaceName,int Height)
{
	POSITION pos;
	CTextStyle *pStyle;
	if(!sFaceName.IsEmpty())
	{	
		pos = m_styles.GetHeadPosition();
		while(pos)
		{
			pStyle = m_styles.GetNext(pos);
			if(pStyle->IsUserDefined())
			{
				pStyle->SetStyleFont(sFaceName,Height);				
				return true;
			}

		}		
	}	
	return false;
}

void CCowDrwDoc::ResetGroupsVisibility(bool bVisible)
{
	POSITION pos = m_groups.GetHeadPosition();
	while(pos)
		m_groups.GetNext(pos)->SetVisible(bVisible);
}

bool CCowDrwDoc::SetGroupVisible(const CString& sGroupName,bool bVisible)
{
	CDocObjGroup *pGrp;
	POSITION pos = m_groups.GetHeadPosition();
	while(pos)
	{
		pGrp = m_groups.GetNext(pos);
		if(pGrp->GetGroupName() == sGroupName)
		{
			pGrp->SetVisible(bVisible);
			return true;
		}
	}
	return false;
}

void CCowDrwDoc::DrawText(const CString& sLabel,const CString &sText)
{	
	CDocObj *pObj = GetObjByLabel(sLabel);
	if(!pObj) return;
	if(!m_pDC) return;

	if(!pObj->IsKindOf(RUNTIME_CLASS(CDocObjText))) return;

	CDocObjText *pText = (CDocObjText*)pObj;
	pText->DrawText(m_pDC,sText);

}

bool CCowDrwDoc::SetTemplatePage(int nPage) 
{
	if(nPage >= 0 && nPage < m_DocPages.GetCount()) 
	{
		m_CurrPage = nPage;
		return true;
	}
	return false;
}

void CCowDrwDoc::SetPrintDC(CDC *pDC)
{
	m_pDC = pDC;
}

void CCowDrwDoc::DrawImage(const CString& sLabel,COleStreamFile &pictureStream)
{
	CPicture *pPicture = NULL;
	CDocObj *pObj = GetObjByLabel(sLabel);
	if(!(pObj && m_pDC)) return;
	if(!pObj->IsKindOf(RUNTIME_CLASS(CDocImgObj))) return;
	CDocImgObj *pImg = (CDocImgObj*)pObj;
	pPicture = pImg->GetPicture();
	pPicture->ReadFromStream(pictureStream.GetStream());
}

CGroupList* CCowDrwDoc::GetGroupList()
{
	return &m_groups;
}

CTextStyleList* CCowDrwDoc::GetStyleList()
{
	return &m_styles;
}

int CCowDrwDoc::GetTemplatePages() const
{
	return m_DocPages.GetCount();
}

COLORREF CCowDrwDoc::GetPaperColor() const
{
	return m_paperColor;
}

const CSize& CCowDrwDoc::GetPageSize() const
{
	return m_PageSize;
}

CSize CCowDrwDoc::GetPaperSize() const
{
	return m_PaperSize;
}

const CSize& CCowDrwDoc::GetPhyMarginsSize() const
{
	return m_PhyMargins;
}

void CCowDrwDoc::PreparePage(int nPage)
{

}


void CCowDrwDoc::SetDocOptions()
{
	UINT cfgItemId = GetCfgItemID(); 

	SetUserDefStyle(PrintCfg().GetString(cfgItemId,CPrintCfg::CFG_FONTFAMILY),-PrintCfg().GetNum( cfgItemId,CPrintCfg::CFG_FONTHEIGHT));
	
	SetMargOffets(PrintCfg().GetNum( cfgItemId,CPrintCfg::CFG_OFFX),PrintCfg().GetNum( cfgItemId,CPrintCfg::CFG_OFFY));
	
	SetSortBy((COWDATA_MEMBERS)PrintCfg().GetNum( cfgItemId,CPrintCfg::CFG_COWSSORTBY));
	
	SetSortOrder(PrintCfg().GetBool( cfgItemId,CPrintCfg::CFG_SORTORDER) == TRUE);

	SetMoneyFormat(FloatFormat(PrintCfg().GetNum(cfgItemId,CPrintCfg::CFG_MONEYPRECISION),(char)PrintCfg().GetNum(cfgItemId,CPrintCfg::CFG_MONEYDECIMALPT)));
	SetWeightFormat(FloatFormat(PrintCfg().GetNum(cfgItemId,CPrintCfg::CFG_WEIGHTPRECISION),(char)PrintCfg().GetNum(cfgItemId,CPrintCfg::CFG_WEIGHTDECIMALPT)));
	//SetPricePerKgFormat(FloatFormat(PrintCfg().GetNum(cfgItemId,CPrintCfg::CFG_PRICEPERKGPRECISION),(char)PrintCfg().GetNum(cfgItemId,CPrintCfg::CFG_PRICEPERKGDECIMALPT)));
}



