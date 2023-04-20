#ifndef __HENTLISTBOX_H__

class CHentListBox : public CListBox
{
public:
	CHentListBox();
	int GetItemCount();
	int SetItemCount( int nCount );

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);	

	
protected:

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );

	DECLARE_MESSAGE_MAP()
};


#endif