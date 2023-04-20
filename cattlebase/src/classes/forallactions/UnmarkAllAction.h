#ifndef __UNMARKALLACTION_H__
#define __UNMARKALLACTION_H__

template<class ACTION,class DATA>
class CUnmarkAllAction : public ACTION
{
public:
	
	virtual bool operator()(DATA* pElement)
	{	
		pElement->bMark = false;
		return true;	
	}

	
};



#endif


	
	





