// IMainTreeCtrlListener.h
//

#if !defined(_IMAINTREECTRLLISTENER_H_)
#define _IMAINTREECTRLLISTENER_H_

class IMainTreeCtrlListener
{
public:
	virtual void OnTreeNodeSelection(HTREEITEM oldItem, HTREEITEM newItem) = 0;
	virtual void OnTreeNodeExpansion(HTREEITEM item) = 0;

};

#endif // !defined(_IMAINTREECTRLLISTENER_H_)