#ifndef __POPMENUNODE_H__
#define __POPMENUNODE_H__

#include "PtrFlushArray.h"


class CPopMenuNode
{
public:
	CPopMenuNode(UINT nID,UINT nResMenuText,UINT nResSubMenu);

	CPopMenuNode();

	UINT getID();
	UINT getResMenuText();
	UINT getResSubMenu();
	bool IsSpearator();

private:
	UINT m_nID;
	UINT m_nResMenuText;
	UINT m_nResSubMenu;
};

typedef PtrFlushArray<CPopMenuNode> CPopMenuNodeList;	

#endif
