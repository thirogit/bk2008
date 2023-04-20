#include "stdafx.h"
#include "DrawObjGroup.h"
#include "XStr.h"


CDrawObjGroup::CDrawObjGroup() : m_PrintVis(true),m_groupID(0)
{
}

CDrawObjGroup::CDrawObjGroup(CString &name,int id,bool PrintVis) : m_PrintVis(PrintVis),m_groupname(name), m_groupID(id)
{

}

CDrawObjGroup::~CDrawObjGroup()
{
}


#ifndef FOR_CATTLEBASE

void CDrawObjGroup::SerializeToXml(xercesc::DOMElement * pParentElement)
{
	xercesc::DOMDocument* pDOMDoc = pParentElement->getOwnerDocument();

	pParentElement->setAttribute(X("id"),X(m_groupID));

	xercesc::DOMElement* pNameElement = pDOMDoc->createElement(X("name"));
	pNameElement->appendChild(pDOMDoc->createTextNode(X(m_groupname)));
	pParentElement->appendChild(pNameElement);

	xercesc::DOMElement* pVisibilityElement = pDOMDoc->createElement(X("visibility"));
	pVisibilityElement->appendChild(pDOMDoc->createTextNode(X(m_PrintVis)));
	pParentElement->appendChild(pVisibilityElement);
	
}
#endif


void CDrawObjGroup::Serialize(CArchive &ar)
{
	char groupname[MAX_GROUP_NAME];
	memset(groupname,0,sizeof(groupname));
	
	if (ar.IsStoring())
	{
		_snprintf(groupname,MAX_GROUP_NAME,"%s",(LPCSTR)m_groupname);

		ar.Write(groupname,sizeof(groupname));
		ar << m_groupID;
		ar << m_PrintVis;
	}
	else
	{
		ar.Read(groupname,sizeof(groupname));
		m_groupname = groupname;
		ar >> m_groupID;
		ar >> m_PrintVis;
		
	}


}

