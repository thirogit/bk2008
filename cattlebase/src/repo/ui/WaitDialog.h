#ifndef __WAITDIALOG_H__
#define __WAITDIALOG_H__

#include <boost/function.hpp>


class ExceptionInterceptor
{
public:
	ExceptionInterceptor(boost::function<void()> &fn);
	void Invoke();
	bool HasEception();
	CException* GetException();
private:
	CException* m_pException;
	boost::function<void()> m_Function;
};

class CWaitDialog : public CDialog
{

public:
    CWaitDialog (CWnd* pParentWnd, const CString& sWaitText);
    virtual ~CWaitDialog ();
	void WaitFor(boost::function<void()> &f);
private:
    BOOL Pump(); 
	void Show();
	void Hide();   
private:
	CWnd* m_pParentWnd;
	CString m_sWaitText;
};


#endif