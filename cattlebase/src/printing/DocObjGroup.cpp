#include "stdafx.h"
#include "DocObjGroup.h"


CDocObjGroup::CDocObjGroup(UINT iId) : m_bVisible(true),m_groupId(iId)
{
}

CDocObjGroup::CDocObjGroup(const CString &sGroupName,UINT iId,bool bVisible/* = true*/) :
					m_bVisible(bVisible),m_sGroupName(sGroupName), m_groupId(iId)
{

}


void CDocObjGroup::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}
void CDocObjGroup::SetGroupName(const CString& sGroupName)
{
	m_sGroupName = sGroupName;
}


CString CDocObjGroup::GetGroupName() const
{
	return m_sGroupName;
}
bool CDocObjGroup::IsVisible() const
{
	return m_bVisible;
}

UINT  CDocObjGroup::GetId() const
{
	return m_groupId;
}

CDocObjGroup::~CDocObjGroup()
{

}

CDocObjGroup* CDocObjGroup::ReadFromArchive(CArchive &ar)
{
	if (ar.IsStoring())
		return NULL;
	
	CDocObjGroup* pNewGroup = new CDocObjGroup(0);
	pNewGroup->Serialize(ar);
	return pNewGroup;
}
void CDocObjGroup::WriteToArchive(CArchive &ar)
{
	if (!ar.IsStoring())
		return;
	Serialize(ar);
}


void CDocObjGroup::Serialize(CArchive &ar)
{
	char szGroupName[MAX_GROUP_NAME+1];
	memset(szGroupName,0,sizeof(szGroupName));
	
	if (ar.IsStoring())
	{
		_snprintf(szGroupName,MAX_GROUP_NAME,"%s",(LPCSTR)m_sGroupName);

		ar.Write(szGroupName,MAX_GROUP_NAME);
		ar << m_groupId;
		ar << m_bVisible;
	}
	else
	{
		ar.Read(szGroupName,MAX_GROUP_NAME);
		m_sGroupName = szGroupName;
		ar >> m_groupId;
		ar >> m_bVisible;
		
	}


}

