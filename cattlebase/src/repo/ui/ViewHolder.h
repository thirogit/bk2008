#ifndef __VIEWHOLDER_H__
#define __VIEWHOLDER_H__



class RepoAccountSessionView;

class ViewHolder : public CStatic
{
public:
	ViewHolder();
	void Show(RepoAccountSessionView* pSessionView);
	BOOL Create(CWnd* pParentWnd,const RECT& rect,UINT nID);
protected:
	RepoAccountSessionView* m_pActiveSessionView;
	
};


#endif