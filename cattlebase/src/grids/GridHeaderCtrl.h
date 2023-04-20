#ifndef __GRIDHEADERCTRL_H__
#define __GRIDHEADERCTRL_H__

class CGridHeaderCtrl : public CHeaderCtrl
{
public:
     CGridHeaderCtrl();
     virtual ~CGridHeaderCtrl();
protected:
     afx_msg LRESULT OnHeaderLayout(WPARAM wp, LPARAM lp);
     DECLARE_MESSAGE_MAP()
};


#endif