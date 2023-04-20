#include "stdafx.h"
#include "DocTextObj.h"
#include "CowDrwDoc.h"

IMPLEMENT_SERIAL(CDrawText, CDocObjText, 0)

IMPLEMENT_SERIAL(CDocObjText, CDocRectObj, 0)

CDocObjText::CDocObjText() : CDocRectObj(),m_bOwnStyle(TRUE),m_OwnStyle(),m_pStyle(NULL)
{
}

CDocObjText::CDocObjText(const CRect& position)
	: CDocRectObj(position),m_bOwnStyle(TRUE),m_OwnStyle(),m_pStyle(NULL)
{
}

CDocObjText::CDocObjText(const CRect& position,const CString& sText, CTextStyle &pStyle)
	: CDocRectObj(position), m_text(sText),m_OwnStyle(pStyle),m_bOwnStyle(TRUE),m_pStyle(NULL)
{
	m_OwnStyle.ResetId();
	m_OwnStyle.SetStyleName(CString());
}


CDocObjText::~CDocObjText() 
{
	
}

void CDocObjText::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	int styleid;
	char text[MAX_TEXT+1];
	memset(text,0,sizeof(text));
	
	CDocRectObj::Serialize(ar);
	
	if (ar.IsStoring())
	{
	
		m_OwnStyle.Serialize(ar);
		ar << m_bOwnStyle;
		styleid = (m_pStyle ? m_pStyle->GetId() : 0);  
		ar << styleid;
		
		_snprintf(text,MAX_TEXT,"%s",(LPCSTR)m_text);
		ar.Write(text,sizeof(text));
	
	}
	else
	{
		CCowDrwDoc* m_pDocument;
		m_pDocument = (CCowDrwDoc*)ar.m_pDocument;

		m_OwnStyle.Serialize(ar);
		ar >> m_bOwnStyle;
		ar >> styleid;
		m_pStyle = NULL;
		if(styleid)
		{
			m_pStyle = m_pDocument->GetStyleList()->GetTextStyle(styleid);
			ASSERT(m_pStyle);
		}
		else 
		{
			//ASSERT(!m_bOwnStyle);
			m_pStyle = NULL;
		}

		ar.Read(text,sizeof(text));
		m_text = text;

	}
}

void CDocObjText::Draw(CDC *pDC)
{
	ASSERT_VALID(this);	
	CDocRectObj::Draw(pDC);
	DrawText(pDC,m_text);
}
void CDocObjText::DrawText(CDC* pDC,const CString &sText)
{
	ASSERT_VALID(this);

	if(!IsVisible()) return;

	CTextStyle *pStyle;
	if(m_bOwnStyle) pStyle = &m_OwnStyle;
	else pStyle = m_pStyle;

	CRect calcrc(GetPosition());
	CRect rc(&calcrc);


	COLORREF oldColor = pDC->GetTextColor();
	pDC->SetTextColor(pStyle->GetTextColor());
	int oldMode = pDC->GetBkMode();
	pDC->SetBkMode(TRANSPARENT);

	HFONT oldFont = (HFONT)::SelectObject(pDC->GetSafeHdc(),pStyle->GetFont());
		
	VER_ALIGN valign = pStyle->GetVerAlign();
	int height = abs(pDC->DrawText(sText,&calcrc,  DT_WORDBREAK | DT_CALCRECT )),
	rcheight = abs((rc.bottom - rc.top));

	if(height < rcheight) 
	{
	switch(valign)
	{
		case VCENTER:
			rc.top -= (rcheight-height)/2; // Y rises up
			break;
		case BOTTOM:
			rc.top -= (rcheight-height);	
			break;
		
	}
	valign = (VER_ALIGN)0; //get rid of VCENTER or BOTTOM
	}
	
	pDC->DrawText(sText,rc, pStyle->GetHorAlign() | DT_WORDBREAK | valign);

	::SelectObject(pDC->GetSafeHdc(),oldFont);

	pDC->SetBkMode(oldMode);
	pDC->SetTextColor(oldColor);
}


CRect CDocObjText::GetRectAlign(CRect &fitRect,CSize &txtSize,HOR_ALIGN halign,VER_ALIGN valign)
{
	CRect txtRect;
	int fitWidth = abs(fitRect.Width()),fitHeight = abs(fitRect.Height());

	if(txtSize.cx >= fitWidth) 
	{
		txtRect.left = fitRect.left;
		txtRect.right = fitRect.right;
	}
	else
	{
		switch(halign)
		{
			case LEFT:
				txtRect.left = fitRect.left;
				txtRect.right = fitRect.left+txtSize.cx;
			break;
			case RIGHT:
				txtRect.left = fitRect.right-txtSize.cx;
				txtRect.right = fitRect.right;
			break;
			case CENTER:
				txtRect.left = fitRect.left+(fitWidth-txtSize.cx)/2;
				txtRect.right = txtRect.left+txtSize.cx;
			break;
		}
	}

	if(txtSize.cy >= fitHeight)
	{
		txtRect.top = fitRect.top;
		txtRect.bottom = fitRect.bottom;
	} 
	else
	{
		switch(valign)
		{
			case TOP:
				txtRect.top = fitRect.top;
				txtRect.bottom = fitRect.top-txtSize.cy;
			break;
			case BOTTOM:
				txtRect.bottom = fitRect.bottom;
				txtRect.top = fitRect.bottom+txtSize.cy;
			break;
			case VCENTER:
				txtRect.top = fitRect.top-(fitHeight-txtSize.cy)/2;
				txtRect.bottom = txtRect.top-txtSize.cy;
			break;
		}
	}

	return txtRect;

}
	