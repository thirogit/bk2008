#ifndef __EDITCELLTEMPLATE_H__
#define __EDITCELLTEMPLATE_H__

#include "../FilterEdit/FilterEdit.h"

#if _MSC_VER < 1400

#define BEGIN_TEMPLATE_MESSAGE_MAP(theClass, type_name, baseClass)			\
	template < typename type_name >											\
	const AFX_MSGMAP* theClass< type_name >::GetMessageMap() const			\
		{ return GetThisMessageMap(); }										\
	template < typename type_name >											\
	const AFX_MSGMAP* PASCAL theClass< type_name >::GetThisMessageMap()		\
	{																		\
		typedef theClass< type_name > ThisClass;							\
		typedef baseClass TheBaseClass;										\
		static const AFX_MSGMAP_ENTRY _messageEntries[] =					\
		{

#endif


template<class BASECLASS>
class CBaseEditCell : public BASECLASS
{
public:
	CBaseEditCell (CListCtrl* pCtrl, int iItem, int iSubItem);
	int Create(const RECT &rect,CWnd *pParentWnd,UINT nID);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void    SetListText();

    CListCtrl* pListCtrl;
    int			Item;
    int			SubItem;
   
    //{{AFX_MSG(gxEditCell)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnNcDestroy();
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//template CBaseEditCell<CFilterEdit>;

class CFloatEditCell : public CBaseEditCell<CFilterEdit>
{
public:
	CFloatEditCell(CListCtrl* pCtrl, int iItem, int iSubItem, double initFloat,int prec);
	int Create(const RECT &rect,CWnd *pParentWnd,UINT nID);
private:
	double m_initFloat;
	int m_prec;
};


class CIntEditCell : public CBaseEditCell<CFilterEdit>
{
public:
	CIntEditCell(CListCtrl* pCtrl, int iItem, int iSubItem, int initValue);
	int Create(const RECT &rect,CWnd *pParentWnd,UINT nID);
private:
	int m_initValue;
};

#endif 
