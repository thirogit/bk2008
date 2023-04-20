// IAsOptionGroupListener.h
//

#if !defined (_CMOPTIONGROUPLISTENER_H_)
#define _CMOPTIONGROUPLISTENER_H_

class cmOptionGroup;

class ICmOptionGroupListener
{
public:

	virtual void OnGroupSizeChanged(bool redraw = true) = 0;
	//virtual void OnSelectionInfoChanged(cmOptionGroup* selectedGroup, bool redraw = true) = 0;
};

#endif // !defined (_CMOPTIONGROUPLISTENER_H_)