// IAsOptionRowListener.h
//

#if !defined(_IASOPTIONROWLISTENER_H_)
#define _IASOPTIONROWLISTENER_H_

class cmOptionRow;

class ICmOptionRowListener
{
public:
	virtual void OnRowSelection(cmOptionRow* selectedRow, bool redraw = true) = 0;

};

#endif // !defined(_IASOPTIONROWLISTENER_H_)