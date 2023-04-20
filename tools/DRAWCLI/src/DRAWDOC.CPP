// drawdoc.cpp : implementation of the CDrawDoc class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#ifndef FOR_CATTLEBASE
#include "drawcli.h"
#endif



#include "drawdoc.h"
#include "drawvw.h"
#include "drawobj.h"

#ifndef FOR_CATTLEBASE
#include "cntritem.h"
#include "summpage.h"
#include "statpage.h"
#include "XStr.h"
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc

IMPLEMENT_DYNCREATE(CDrawDoc, COleDocument)


BEGIN_MESSAGE_MAP(CDrawDoc, COleDocument)
#ifndef FOR_CATTLEBASE

	//{{AFX_MSG_MAP(CDrawDoc)

	ON_COMMAND(ID_VIEW_PAPERCOLOR, OnViewPaperColor)
	ON_COMMAND(ID_FILE_SUMMARYINFO, OnFileSummaryInfo)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_FIRST, COleDocument::OnUpdateObjectVerbMenu)
		// MAPI support
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)

	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)

#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc construction/destruction

CDrawDoc::CDrawDoc()
{
	EnableCompoundFile();

	m_nMapMode = MM_ANISOTROPIC;
	m_paperColor = RGB(255, 255, 255);
	m_pSummInfo = NULL;

	m_NumPages = 1;
	CDrawObjList *newList = new CDrawObjList;

	m_styles.AddHead(new CTextStyle(CString(USERDEF_STYLENAME),USERDEF_STYLEID));


	m_DocPages.Add(newList);

	m_CurrPage = 0;
	m_orientation = 0;
	ComputePageSize();

#ifdef FOR_CATTLEBASE 
	m_MargOff.SetSize(0,0);
	m_printInForm = false;
	m_nCfgID = 0;
	m_bAsc = true;
#endif
	
}

CDrawDoc::~CDrawDoc()
{
	/*POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
		delete m_objects.GetNext(pos);*/
	
	for(int i=0;i<m_NumPages;i++)
	{
		CDrawObjList *ObjList =	(CDrawObjList*)m_DocPages.GetAt(i);
	
		POSITION posObj = ObjList->GetHeadPosition();
		while (posObj != NULL)	delete ObjList->GetNext(posObj);
	
		delete ObjList;
	}
	
	delete m_pSummInfo;

	POSITION pos = m_groups.GetHeadPosition();
	while(pos)
	{
		delete (CDrawObjGroup*)m_groups.GetNext(pos);
	}

	pos = m_styles.GetHeadPosition();
	while(pos)
	{
		delete (CTextStyle*)m_styles.GetNext(pos);
	}


}


#ifndef FOR_CATTLEBASE
void CDrawDoc::NewPage(void)
{
	CDrawObjList *newList = new CDrawObjList;
	m_DocPages.Add(newList);
	m_NumPages++;
	m_CurrPage++;
}


void CDrawDoc::DelPage(void)
{

	if(m_NumPages > 1)
	{
		m_DocPages.RemoveAt(m_CurrPage);
		m_NumPages--;

		if(m_CurrPage == 0) m_CurrPage++;
		else m_CurrPage--;
	}


}


void CDrawDoc::NextPage(void)
{

	if((m_CurrPage + 1)  < m_NumPages)
		m_CurrPage++;
	else m_CurrPage = 0;
}

void CDrawDoc::PrevPage(void)
{

	if((m_CurrPage - 1) >= 0)
		m_CurrPage--;
	else m_CurrPage = (m_NumPages-1);
}

BOOL CDrawDoc::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;

	// reinitialization code
	// (SDI documents will reuse this document)
	if(m_pSummInfo != NULL)
		delete m_pSummInfo;
	m_pSummInfo = new CSummInfo;
	// Title, Subject, Author, Keywords default to empty string
	// Comments, Template, SavedBy default to empty string
	// LastSave, LastPrint, EditTime, RevNum default to 0
//	m_pSummInfo->StartEditTimeCount();
	m_pSummInfo->RecordCreateDate();
//	m_pSummInfo->SetNumPages(1);
	// NumWords, NumChars default to 0
//	m_pSummInfo->SetAppname( _T("DrawCli") );
	// Security defaults to 0
	return TRUE;
}
#endif
BOOL CDrawDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if( m_pSummInfo != NULL)	delete m_pSummInfo;
	m_pSummInfo = new CSummInfo;


	for(int i=0;i<m_NumPages;i++)
	{

		
		CDrawObjList *ObjList =	(CDrawObjList*)m_DocPages.GetAt(i);
	
		POSITION posObj = ObjList->GetHeadPosition();
		while (posObj != NULL)	
		{
			delete ObjList->GetNext(posObj);
		}
		ObjList->RemoveAll();
	
		if(i > 0) 
		{
			delete ObjList; // leave first page
			m_DocPages.RemoveAt(i);
				
		}
	}

	//m_NumPages = 1;
	m_CurrPage = 0;

	

//	m_pSummInfo->StartEditTimeCount();
	return COleDocument::OnOpenDocument(lpszPathName);
}
#ifndef FOR_CATTLEBASE
BOOL CDrawDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	m_pSummInfo->RecordSaveDate();
	m_pSummInfo->IncrRevNum();
	//m_pSummInfo->SetLastAuthor(m_pSummInfo->GetAuthor());
	//m_pSummInfo->AddCountToEditTime();
	//m_pSummInfo->StartEditTimeCount();
	return COleDocument::OnSaveDocument(lpszPathName);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc serialization


void CDrawDoc::Serialize(CArchive& ar)
{
	int i,size;
	POSITION pos;
	CDrawObjGroup *pGrp;
	CTextStyle *pStyle;
	if (ar.IsStoring())
	{
		ar << m_paperColor;
		ar << m_NumPages;
		ar << m_orientation;
		ar << m_paperSize;

		size = (int)m_groups.GetSize();
		ar << size;
		
		pos = m_groups.GetHeadPosition();
		while(pos)
			m_groups.GetNext(pos)->Serialize(ar);
		
		size = (int)m_styles.GetSize();
		ar << size;

		pos = m_styles.GetHeadPosition();
		while(pos)
			m_styles.GetNext(pos)->Serialize(ar);
				
		for(i=0;i<m_NumPages;i++)
		((CDrawObjList*)m_DocPages[i])->Serialize(ar);

		m_pSummInfo->WriteToStorage(m_lpRootStg);
	}
	else
	{
		ar >> m_paperColor;
		ar >> m_NumPages;
		ar >> m_orientation;
		ar >> m_paperSize;

#ifndef READ_OLD_DOC
		ar >> size;

		for(i = 0;i < size;i++)
		{
			pGrp = new CDrawObjGroup();
			pGrp->Serialize(ar);
			m_groups.AddTail(pGrp); 
		}

		pos = m_styles.GetHeadPosition();
		while(pos) delete m_styles.GetNext(pos);
		m_styles.RemoveAll();

		ar >> size;
		

		for(i =0;i < size;i++)
		{
			pStyle = new CTextStyle();
			pStyle->Serialize(ar);
			m_styles.AddTail(pStyle);

		}
#endif
		//i=1 , becouse we already created one page in constructor
		for(i = 1;i< m_NumPages;i++)
			m_DocPages.Add(new CDrawObjList);

			for(i=0;i<m_NumPages;i++)
		((CDrawObjList*)m_DocPages[i])->Serialize(ar);


	CPrintDialog dlg(FALSE);
	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{
		LPDEVMODE pDM = (DEVMODE*)GlobalLock(dlg.m_pd.hDevMode);

		if ( (dlg.m_pd.hDevMode) &&  pDM )
		{     
			pDM->dmOrientation = m_orientation; 
			pDM->dmPaperSize = m_paperSize;
			GlobalUnlock(dlg.m_pd.hDevMode);
		}
	}
	ComputePageSize();

#if 0
	CDrawObjList* page1 = (CDrawObjList*)m_DocPages[1];
	POSITION lastpos,pos = page1->GetHeadPosition();
	while(pos != NULL)
	{
		CDrawObj *pObj = page1->GetNext(pos);
		if(pObj->m_label.Find("PASSNOBC",0) != -1 ||
			pObj->m_label.Find("COWNOBC",0) != -1 ||
			pObj->m_label.Find("PDF",0) != -1)
		{

			((CDrawImg*)pObj)->m_picture.Load("C:\\Users\\root\\Desktop\\nobc.bmp");
		}
	}

#endif
#if 0
			CDrawObjList* page1 = (CDrawObjList*)m_DocPages[1];
			CDrawObj **list = new CDrawObj*[page1->GetCount()];
			int k,idx = 0;
			CRect clip(-975,1233,983,-1138);
	
			POSITION lastpos,pos = page1->GetHeadPosition();
			while(pos != NULL)
			{
				lastpos = pos;
				CDrawObj *pObj = page1->GetNext(pos);
				pObj->NormalizePosition();

				if(pObj->m_position.top <= clip.top && pObj->m_position.top >= clip.bottom &&
					pObj->m_position.left >= clip.left && pObj->m_position.left <= clip.right)
				{
					list[idx] = pObj;
					idx++;
				}
			}
			
			
			int j;
			CDrawObj *o1,*o2;
			for(i=0;i<idx;i++)
			{
				o1 = list[i];
				for(j=0;j<idx;j++)
				{
					o2 = list[j];

						if(o1->m_position.top > o2->m_position.top)							
						{

							list[i] = o2;
							list[j] = o1;
							o1 = o2;
							
						}
				}

			}

			
			
#define ROWSIZE 7

			for( k=0;k<idx;k+=ROWSIZE)
			{

				CDrawObj **listrow = &list[k];
				for(i=0;i<ROWSIZE;i++)
				{
					o1 = listrow[i];
					for(j=0;j<ROWSIZE;j++)
					{
						o2 = listrow[j];

						if(o1->m_position.left < o2->m_position.left)
						{
							listrow[i] = o2;
							listrow[j] = o1;
							o1 = o2;

						}
					}
				}
			}

				int row=33,grpid=33;				
				char label[50];
				for(i=0;i<idx;i+=ROWSIZE)
				{
					
					
							list[i]->m_label.Format("LP%d",row);
							//((CDrawText*)list[i])->m_text.Format("%d",i);
							list[i+1]->m_label.Format("EAN%d",row);
							//((CDrawText*)list[i+1])->m_text.Format("%d",i+1);							
							list[i+2]->m_label.Format("SEX%d",row);

							list[i+3]->m_label.Format("STOCK%d",row);

							list[i+4]->m_label.Format("WEIGHT_DECLARED%d",row);

							list[i+5]->m_label.Format("WEIGTH_ACCEPTED%d",row);

							list[i+6]->m_label.Format("COW_EXTRAS%d",row);
							//((CDrawText*)list[i+2])->m_text.Format("%d",i+2);
																
																	
					
					//list[i+0]->m_label.Format("TITLE%d",row);
					//list[i+1]->m_label.Format("CATEGORY%d",row);				
					//list[i+2]->m_label.Format("TYPE%d",row);
					//list[i+3]->m_label.Format("INVOICE%d",row);
					//list[i+4]->m_label.Format("DATE%d",row);					
					//list[i+5]->m_label.Format("VAT%d",row);
					//list[i+6]->m_label.Format("NETVALUE%d",row);
					//list[i+7]->m_label.Format("VATVALUE%d",row);
					//list[i+8]->m_label.Format("GROSVALUE%d",row);

					
					sprintf(label,"%d",row);
					pGrp = new CDrawObjGroup(CString(label),grpid);
					m_groups.AddTail(pGrp); 
					
					CDrawObj *pText;
					for(k=0;k<ROWSIZE;k++)	
					{						
						list[i+k]->m_group =  pGrp;
					}

					grpid++;				
					row++;									

				}

			delete[] list;
#endif

	
		m_pSummInfo->ReadFromStorage(m_lpRootStg);
	}

	// By calling the base class COleDocument, we enable serialization
	//  of the container document's COleClientItem objects automatically.
	COleDocument::Serialize(ar);
}




/////////////////////////////////////////////////////////////////////////////
// CDrawDoc implementation

void CDrawDoc::Draw(CDC* pDC, CDrawView* pView)
{

	CDrawObjList *l_objects = (CDrawObjList *)m_DocPages.GetAt(m_CurrPage);


		POSITION pos = l_objects->GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = l_objects->GetNext(pos);
//#if  defined(FOR_CATTLEBASE)
		if(pObj->m_group)
		{
			if(pObj->m_group->m_PrintVis)	pObj->Draw(pDC);
		} 
		else pObj->Draw(pDC);
//#else
//			pObj->Draw(pDC);
//#endif
#ifndef FOR_CATTLEBASE
		if (pView->m_bActive && !pDC->IsPrinting() && pView->IsSelected(pObj))
			pObj->DrawTracker(pDC, CDrawObj::selected);
#endif
	}
}
#ifndef FOR_CATTLEBASE
void CDrawDoc::Add(CDrawObj* pObj)
{


	CDrawObjList *l_objects = (CDrawObjList *)m_DocPages.GetAt(m_CurrPage);
	l_objects->AddTail(pObj);
		
	
	pObj->m_pDocument = this;
	SetModifiedFlag();
}

void CDrawDoc::Remove(CDrawObj* pObj)
{
	CDrawObjList *l_objects = (CDrawObjList *)m_DocPages.GetAt(m_CurrPage);
	
	// Find and remove from document
	POSITION pos = l_objects->Find(pObj);
	if (pos != NULL)
		l_objects->RemoveAt(pos);
	// set document modified flag
	SetModifiedFlag();

	// call remove for each view so that the view can remove from m_selection
	pos = GetFirstViewPosition();
	while (pos != NULL)
		((CDrawView*)GetNextView(pos))->Remove(pObj);
}

// point is in logical coordinates
CDrawObj* CDrawDoc::ObjectAt(const CPoint& point)
{
	CRect rect(point, CSize(1, 1));
	
	CDrawObjList *l_objects = (CDrawObjList *)m_DocPages.GetAt(m_CurrPage);
	
	POSITION pos = l_objects->GetTailPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = l_objects->GetPrev(pos);
        
		if (pObj->Intersects(rect))
			if(pObj->m_group)
			{
				if(pObj->m_group->m_PrintVis)
					return pObj;
			}
			else return pObj;
	}

	return NULL;
}
#endif

void CDrawDoc::ComputePageSize()
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
			m_paperSize = pDM->dmPaperSize;
			GlobalUnlock(dlg.m_pd.hDevMode);
		}
		else 
		{
			m_orientation = DMORIENT_LANDSCAPE;//DMORIENT_PORTRAIT;
			m_paperSize = DMPAPER_A4;
		}

		
		// GetPrinterDC returns a HDC so attach it
		CDC dc;
		HDC hDC= dlg.CreatePrinterDC();
		ASSERT(hDC != NULL);
		dc.Attach(hDC);

		// Get the size of the page in loenglish
		//new_size.cx = MulDiv(dc.GetDeviceCaps(HORZSIZE), 1000, 254);
		//new_size.cy = MulDiv(dc.GetDeviceCaps(VERTSIZE), 1000, 254);
		
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

	// if size changed then iterate over views and reset

	if (new_size != m_PaperSize)
	{
		m_PaperSize = new_size;
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
			((CDrawView*)GetNextView(pos))->SetPageSize(m_PaperSize);
	}

}



#ifndef FOR_CATTLEBASE
	void CDrawDoc::OnViewPaperColor()
	{
		CColorDialog dlg;
		if (dlg.DoModal() != IDOK)
			return;

		m_paperColor = dlg.GetColor();
		SetModifiedFlag();
		UpdateAllViews(NULL);
	}
#endif
/////////////////////////////////////////////////////////////////////////////
// CDrawDoc diagnostics

#ifdef _DEBUG
void CDrawDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CDrawDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc commands
#ifndef FOR_CATTLEBASE

	void CDrawDoc::OnFileSummaryInfo()
	{
		ASSERT_VALID(this);

		CPropertySheet sheet( _T("Document Properties") );
		CSummPage summ;
		CStatPage stat;
		sheet.AddPage( &summ );
		sheet.AddPage( &stat );

	//	summ.m_strAppname = m_pSummInfo->GetAppname();
	//	summ.m_strTitle   = m_pSummInfo->GetTitle();
	//	summ.m_strSubj    = m_pSummInfo->GetSubject();
		summ.m_strAuthor  = m_pSummInfo->GetAuthor();
	//	summ.m_strKeywd   = m_pSummInfo->GetKeywords();
		summ.m_strCmt     = m_pSummInfo->GetComments();
	//	summ.m_strTempl   = m_pSummInfo->GetTemplate();

	//	stat.m_strSavedBy    = m_pSummInfo->GetLastAuthor();
		stat.m_strRevNum     = m_pSummInfo->GetRevNum();
	//	stat.m_strEditTime   = m_pSummInfo->GetEditTime();
	//	stat.m_strLastPrint  = m_pSummInfo->GetLastPrintDate();
		stat.m_strCreateDate = m_pSummInfo->GetCreateDate();
		stat.m_strLastSave   = m_pSummInfo->GetLastSaveDate();
	//	stat.m_strNumPages   = m_pSummInfo->GetNumPages();
	//	stat.m_strNumWords   = m_pSummInfo->GetNumWords();
	//	stat.m_strNumChars   = m_pSummInfo->GetNumChars();
	//	stat.m_strSecurity   = m_pSummInfo->GetSecurity();

		if (sheet.DoModal() != IDOK)
			return;

		m_pSummInfo->SetAuthor(summ.m_strAuthor);
		//m_pSummInfo->SetKeywords(summ.m_strKeywd);
		//m_pSummInfo->SetSubject(summ.m_strSubj);
		m_pSummInfo->SetComments(summ.m_strCmt);
		//m_pSummInfo->SetTemplate(summ.m_strTempl);
		//m_pSummInfo->SetTitle(summ.m_strTitle);

		SetModifiedFlag();
	}


	void CDrawDoc::OnFilePrintSetup()
	{
		((CDrawApp*)AfxGetApp())->OnFilePrintSetup();
		ComputePageSize();
		//GetFirstFrame()->GetActiveView()->Invalidate();

		//CDrawDoc::Se
		
		return;
	}
#endif

CDrawObjList* CDrawDoc::GetObjects()
{ 
	return (CDrawObjList*)m_DocPages.GetAt(m_CurrPage);
}
CDrawObjList* CDrawDoc::GetPageObjects(int nPage)
{
	if(nPage >= 0 && nPage < m_NumPages)
	return (CDrawObjList*)m_DocPages.GetAt(nPage);
	return NULL;
}


CDrawObj* CDrawDoc::GetObjByLabel(const char *label)
{

CDrawObjList *l_objects = (CDrawObjList *)m_DocPages.GetAt(m_CurrPage);

POSITION pos = l_objects->GetHeadPosition();
while (pos != NULL)
{
	CDrawObj* pObj = l_objects->GetNext(pos);
	if(!pObj->m_label.Compare(label)) return pObj;

}
return NULL;
}

void CDrawDoc::SerializeXml(xercesc::DOMElement * pParentElement)
{
	xercesc::DOMDocument * pDOMDocument = pParentElement->getOwnerDocument();

	pParentElement->setAttribute(X("paper"),X(m_paperSize));
	pParentElement->setAttribute(X("orientation"),X(m_orientation));

	xercesc::DOMElement* pStylesElement = pDOMDocument->createElement(xercesc::XMLString::transcode("styles"));
	pParentElement->appendChild(pStylesElement);

	POSITION pos  = m_styles.GetHeadPosition();
	while(pos)
	{
		xercesc::DOMElement* pStyleElement = pDOMDocument->createElement(xercesc::XMLString::transcode("style"));
		pStylesElement->appendChild(pStyleElement);
		CTextStyle* pStyle = m_styles.GetNext(pos);
		pStyle->SerializeToXml(pStyleElement);
	}


	xercesc::DOMElement* pGroupsElement = pDOMDocument->createElement(xercesc::XMLString::transcode("groups"));
	pParentElement->appendChild(pGroupsElement);

	pos  = m_groups.GetHeadPosition();
	while(pos)
	{
		xercesc::DOMElement* pGroupElement = pDOMDocument->createElement(xercesc::XMLString::transcode("group"));
		pGroupsElement->appendChild(pGroupElement);
		CDrawObjGroup* pGroup = m_groups.GetNext(pos);
		pGroup->SerializeToXml(pGroupElement);
	}

	xercesc::DOMElement* pPagesElement = pDOMDocument->createElement(xercesc::XMLString::transcode("pages"));
	pParentElement->appendChild(pPagesElement);
	
	for(int i=0;i<m_DocPages.GetCount();i++)
	{
		CDrawObjList *ObjList =	(CDrawObjList*)m_DocPages.GetAt(i);
		xercesc::DOMElement* pPageElement = pDOMDocument->createElement(xercesc::XMLString::transcode("page"));
		pPagesElement->appendChild(pPageElement);
	
		xercesc::DOMElement* pFieldsElement = pDOMDocument->createElement(xercesc::XMLString::transcode("fields"));
		pPageElement->appendChild(pFieldsElement);

		POSITION posObj = ObjList->GetHeadPosition();
		while(posObj)
		{
			CDrawObj* pObj = ObjList->GetNext(posObj);
			pObj->SerializeToXml(pFieldsElement);
		}			
	}	
	
}

#ifdef FOR_CATTLEBASE

bool CDrawDoc::CreateDocument(CPtrArray &inputDataArr,LPCSTR templatePath,UINT nCfgID) 
{
	m_nCfgID = nCfgID;
	return OnOpenDocument(templatePath) == TRUE;	
}

bool CDrawDoc::SetUserDefStyle(LPCSTR szFaceName,int Height)
{
	POSITION pos;
	CTextStyle *pStyle;
	if(szFaceName)
		if(szFaceName[0] != '\0')
		{
			pos = m_styles.GetHeadPosition();
			while(pos)
			{
				pStyle = m_styles.GetNext(pos);
				if(pStyle->m_styleid == USER_DEF_STYLE_ID)
				{
					strncpy(pStyle->m_lf.lfFaceName,szFaceName,LF_FACESIZE);
					pStyle->m_lf.lfHeight = Height;
					pStyle->RealizeStyle(true);
					return true;
				}

			}		

		}
	
return false;
}
	
/*	bool CDrawDoc::GetObjRect(const char *szLabel,CRect &rect)
	{
		int xy;
		CDrawObj* pObj = GetObjByLabel(szLabel);
		if(!pObj) return false;
		rect = pObj->m_position;
		//rect.NormalizeRect();
		//X axis rises left, Y axis rises up

		if(rect.top < rect.bottom)
		{
			xy = rect.top;
			rect.top = rect.bottom;
			rect.bottom = xy;
		}

		if(rect.left > rect.right)
		{
			xy = rect.left;
			rect.left = rect.right;
			rect.right = xy;
		}
	
		return true;
	}

*/	
void CDrawDoc::PutChar(/*CDC *pDC,*/const char *szLabel,char chr)
{
	DrawText(/*pDC,*/szLabel,CString(chr));
}

void CDrawDoc::ResetPrintVis(bool bPrintVis)
{
	POSITION pos = m_groups.GetHeadPosition();
	while(pos)
		m_groups.GetNext(pos)->m_PrintVis = bPrintVis;
}

bool CDrawDoc::SetGroupPrintVis(int groupId,bool bPrintVis)
{
	CDrawObjGroup *pGrp;
	POSITION pos = m_groups.GetHeadPosition();
	while(pos)
	{
		pGrp = m_groups.GetNext(pos);
		if(pGrp->m_groupID == groupId)
		{
			pGrp->m_PrintVis = bPrintVis;
			return true;
		}
	}
	return false;

}
bool CDrawDoc::SetGroupPrintVis(LPCSTR groupName,bool bPrintVis)
{
	CDrawObjGroup *pGrp;
	POSITION pos = m_groups.GetHeadPosition();
	while(pos)
	{
		pGrp = m_groups.GetNext(pos);
		if(pGrp->m_groupname == groupName)
		{
			pGrp->m_PrintVis = bPrintVis;
			return true;
		}
	}
	return false;
}

void CDrawDoc::DrawText(/*CDC *pDC,*/const char *szLabel,int iNumber)
{
	CString s;
	s.Format("%d",iNumber);
	DrawText(/*pDC,*/szLabel,s);
}

void CDrawDoc::DrawText(const char *szLabel,int row,LPCSTR szStrFmt,...)
{
	CString s,fld;
	fld.Format(szLabel,row);
	va_list va;
	va_start(va,szStrFmt);
	s.FormatV(szStrFmt,va);
	DrawText(fld,s);
	va_end(va);
}

void CDrawDoc::DrawText(/*CDC *pDC,*/const char *szLabel,LPCSTR szStrFmt,...)
{
	CString s;

	va_list va;
	va_start(va,szStrFmt);
	s.FormatV(szStrFmt,va);
	DrawText(/*pDC,*/szLabel,s);
	va_end(va);
}


void CDrawDoc::DrawText(/*CDC *pDC,*/const char *szLabel,CString &strText)
{
CRect rc,calcrc;
CDrawObj *pObj = GetObjByLabel(szLabel);
if(!pObj) return;

if(!m_pDC) return;

if(!pObj->IsKindOf(RUNTIME_CLASS(CDrawText))) return;

CDrawText *pText = (CDrawText*)pObj;
pText->DrawText(m_pDC,strText);

}

bool CDrawDoc::SetCurrPage(int nPage) 
{
	if(nPage >= 0 && nPage < m_NumPages) 
	{
		m_CurrPage = nPage;
		return TRUE;
	}
	return FALSE;
}

void CDrawDoc::SetPrintDC(CDC *pDC)
{
	m_pDC = pDC;
}

void CDrawDoc::DrawImage(const char *szLabel,COleStreamFile &pictureStream)
{
	CPicture *pPicture = NULL;
	CDrawObj *pObj = GetObjByLabel(szLabel);
	if(!(pObj && m_pDC)) return;
	if(!pObj->IsKindOf(RUNTIME_CLASS(CDrawImg))) return;
	CDrawImg *pImg = (CDrawImg*)pObj;
	pPicture = pImg->GetPicture();
	pPicture->ReadFromStream(pictureStream.GetStream());
}
#endif


/*
			RECT clip;

			CDrawObjList* page1 = (CDrawObjList*)m_DocPages[1];
			SetRect(&clip,-841,1293,-5,-461);


			POINT p;
			

			
			CDrawObj **list = new CDrawObj*[page1->GetCount()];
			int idx = 0;
	
			POSITION pos = page1->GetHeadPosition();
			while(pos != NULL)
			{
				CDrawObj *pObj = page1->GetNext(pos);

				p.x = pObj->m_position.left;
				p.y = pObj->m_position.top;

				//TRACE("POINT x = %d, y = %d, label = %s\n",p.x,p.y,(LPCSTR)pObj->m_label);

				if( pObj->m_position.left >= clip.left &&
					pObj->m_position.top <= clip.top &&
					pObj->m_position.right <= clip.right &&
					pObj->m_position.bottom >= clip.bottom &&




					abs(pObj->m_position.Width()) < 50 &&
					abs(pObj->m_position.Height()) < 50)
				{

					list[idx] = pObj;
					idx++;
				

				}

			}

		
			int j;

			CDrawObj *o1,*o2;
			for(i=0;i<idx;i++)
			{
				o1 = list[i];
				for(j=0;j<idx;j++)
				{
					o2 = list[j];

						if(o1->m_position.top > o2->m_position.top)
						{

							list[i] = o2;
							list[j] = o1;
							o1 = o2;
							
						}
				}

			}


			for(int k=0;k<idx;k+=14)
			{

				CDrawObj **list14 = &list[k];


				for(i=0;i<14;i++)
				{
					o1 = list14[i];
					for(j=0;j<14;j++)
					{
						o2 = list14[j];

						if(o1->m_position.left < o2->m_position.left)
						{
							list14[i] = o2;
							list14[j] = o1;
							o1 = o2;

						}
					}
				}






			}




				int col = 1,row=10;
				char label[20];
				bool ean = true;

				for(i=0;i<idx;i++)
				{
					if(ean) sprintf(label,"EAN%d_%d",row,col);
					else sprintf(label,"AIMRNO%d_%d",row,col);
					list[i]->m_label =  label;
				
					
				

					if(col == 14)
					{
						col = 0;
						ean = !ean;
						if(ean)	row++;
					}
						col++;

				}

			delete[] list;*/
	//	CDrawObjList* page1 = (CDrawObjList*)m_DocPages[0];
	//	page1->Serialize(ar);
