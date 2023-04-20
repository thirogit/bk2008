#ifndef __CLUSTER_H__
#define __CLUSTER_H__

template<class ITEM>
class Cluster
{
public:	
	virtual int GetCount() const = 0;
	virtual ITEM* GetItem(int iIndex) = 0;	
};

#endif