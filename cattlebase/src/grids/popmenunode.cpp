#include "stdafx.h"
#include "popmenunode.h"


CPopMenuNode::CPopMenuNode(UINT nID,UINT nResMenuText,UINT nResSubMenu) :
	  m_nID(nID),m_nResMenuText(nResMenuText),m_nResSubMenu(nResSubMenu) 
{
}

CPopMenuNode::CPopMenuNode() : m_nID(0),m_nResMenuText(0),m_nResSubMenu(0) 
{
}



UINT CPopMenuNode::getID()
{
	return m_nID;
}
UINT CPopMenuNode::getResMenuText()
{
	return m_nResMenuText;
}
UINT CPopMenuNode::getResSubMenu()
{
	return m_nResSubMenu;
}
bool CPopMenuNode::IsSpearator()
{
	return !(m_nID || m_nResMenuText || m_nResSubMenu);
}
