#ifndef __NUMFINDCTRL_H__
#define __NUMFINDCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../FilterEdit/FilterEdit.h"
#include "../classes/match/MatchNullInt.h"
#include "../missing/util.h"
#include "../consts.h"

class CNumFindCtrl : public CStatic
{
// Construction
public:
	CNumFindCtrl(LPCSTR filterExp);
	virtual ~CNumFindCtrl();
	CMP_ACTION_TYPE GetRangeType();
	bool HasValue();
protected:
	virtual void PreSubclassWindow();
	BOOL CreateControls();

	CComboBox m_combo;
	CFilterEdit m_number;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


class CFloatFindCtrl : public CNumFindCtrl
{
public:
	CFloatFindCtrl() : CNumFindCtrl(g_szRegExUnsigned) {};
	double GetValue()
	{
		if(HasValue())
		{

			CString number;
			m_number.GetWindowText(number);
			return CUtil::myatof(number);
		}
		return 0.0;
	}
};

class CIntFindCtrl : public CNumFindCtrl
{
public:
	CIntFindCtrl() : CNumFindCtrl("[0-9]{0,9}") {};
	int GetValue()
	{
		if(HasValue())
		{
			CString number;
			m_number.GetWindowText(number);
			return atoi(number);			
		}
		return 0;
	}


};
#endif

