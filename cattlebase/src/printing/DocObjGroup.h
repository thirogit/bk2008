#ifndef __DOCOBJGROUP_H__
#define __DOCOBJGROUP_H__


#define MAX_GROUP_NAME 32

class CDocObjGroup : public CObject
{
public:
	CDocObjGroup(UINT iId);
	CDocObjGroup(const CString &sGroupName,UINT iId,bool bVisible = true);
	~CDocObjGroup();
	
	void SetVisible(bool bVisible);
	void SetGroupName(const CString& sGroupName);

	UINT GetId() const;
	CString GetGroupName() const;
	bool IsVisible() const;

	static CDocObjGroup* ReadFromArchive(CArchive &ar);
	void WriteToArchive(CArchive &ar);

protected:
	void Serialize(CArchive &ar);

protected:
	UINT m_groupId;
	CString m_sGroupName;
	bool m_bVisible;
    
};

#endif
