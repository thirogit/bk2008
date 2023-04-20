#ifndef __DRAWOBJGROUP_H__
#define __DRAWOBJGROUP_H__

#include "DrawObj.h"
#define MAX_GROUP_NAME MAX_LABEL

class CDrawObj;

class CDrawObjGroup : public CObject
{
public:
	CDrawObjGroup();
	CDrawObjGroup(CString &name,int id,bool PrintVis = true);
	~CDrawObjGroup();

	void Serialize(CArchive &ar);
#ifndef FOR_CATTLEBASE
	void SerializeToXml(xercesc::DOMElement * pParentElement);
#endif
	int m_groupID;
	CString m_groupname;
	bool m_PrintVis;
    //CTypedPtrList<CObList, CDrawObj*> m_members;
};

typedef CTypedPtrList<CObList, CDrawObjGroup*> CGroupList;




#endif
