#ifndef __DIFFERENCEITEM_H__
#define __DIFFERENCEITEM_H__

class DifferenceItem
{
public:
	DifferenceItem(const CString& sItemName,UINT uiMemberId);
	UINT GetMemberId() const;
	const CString& GetItemName() const;
	
private:
	UINT m_uiMemberId;
	CString m_sItemName;
	
};



#endif