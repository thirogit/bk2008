#include "stdafx.h"
#include "DocObj.h"
#include "CowDrwDoc.h"

//#define MAX_TEXT 255

CDocObj::CDocObj() : m_pGroup(NULL)
{
}

CDocObj::~CDocObj()
{
}

CDocObj::CDocObj(const CRect& position)
{
	m_Position = position;
	//m_pDocument = NULL;
	m_pGroup = NULL;

	m_bPen = TRUE;
	m_logpen.lopnStyle = PS_INSIDEFRAME;
	m_logpen.lopnWidth.x = 1;
	m_logpen.lopnWidth.y = 1;
	m_logpen.lopnColor = RGB(0, 0, 0);

	m_bBrush = TRUE;
	m_logbrush.lbStyle = BS_SOLID;
	m_logbrush.lbColor = RGB(192, 192, 192);
	m_logbrush.lbHatch = HS_HORIZONTAL;
}

void CDocObj::Serialize(CArchive& ar)
{
	char szLabel[MAX_DOCOBJ_LABEL+1];
	memset(szLabel,0,sizeof(szLabel));
	int grpId ;

	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		NormalizePosition();
		_snprintf(szLabel,MAX_DOCOBJ_LABEL,"%s",(LPCSTR)m_sLabel);

		ar.Write(szLabel,MAX_DOCOBJ_LABEL);
		ar << m_Position;
		ar << (WORD)m_bPen;
		ar.Write(&m_logpen, sizeof(LOGPEN));
		ar << (WORD)m_bBrush;
		ar.Write(&m_logbrush, sizeof(LOGBRUSH));

		if(m_pGroup)
			grpId = m_pGroup->GetId();
		else 
			grpId = 0;

		ar << grpId;
	
	}
	else
	{
		// get the document back pointer from the archive
		CCowDrwDoc* m_pDocument;
		m_pDocument = (CCowDrwDoc*)ar.m_pDocument;
		//ASSERT_VALID(m_pDocument);
		//ASSERT_KINDOF(CCowDrwDoc, m_pDocument);

		WORD wTemp;
		ar.Read(szLabel,MAX_DOCOBJ_LABEL+1);
		m_sLabel = szLabel;
		ar >> m_Position;
		ar >> wTemp; m_bPen = (BOOL)wTemp;
		ar.Read(&m_logpen,sizeof(LOGPEN));
		ar >> wTemp; m_bBrush = (BOOL)wTemp;
		ar.Read(&m_logbrush, sizeof(LOGBRUSH));

		ar >> grpId;

		if(grpId)
		{
			m_pGroup = m_pDocument->GetGroupList()->GetGroup(grpId);
			ASSERT(m_pGroup);
		}
		NormalizePosition();		
	}
}

void CDocObj::NormalizePosition()
{
	if(m_Position.left > m_Position.right) 
		m_Position.SwapLeftRight();
	if(m_Position.top < m_Position.bottom)
	{
		int iTemp = m_Position.top;
		m_Position.top = m_Position.bottom;
		m_Position.bottom = iTemp;
	}
}


bool CDocObj::IsVisible()
{
	if(m_pGroup)
		return m_pGroup->IsVisible();
	return true;
}

CDocObjGroup * CDocObj::GetGroup()
{
	return m_pGroup;
}

CString CDocObj::GetLabel() const
{
	return m_sLabel;
}

const CRect& CDocObj::GetPosition()
{
	return m_Position;
}