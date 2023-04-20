#ifndef __UPDATABLEITEM_H__
#define __UPDATABLEITEM_H__

class UpdatableItem 
{
public:	
	UpdatableItem() : m_bDoUpdate(false)
	{
	}
	
	void SetDoUpdate(bool bDoUpdate)
	{
		m_bDoUpdate = bDoUpdate;
	}
	
	bool IsDoUpdate() const
	{
		return m_bDoUpdate;
	}
private:
	bool m_bDoUpdate;
	
};

#endif